/*
 * bbschem
 * Copyright (C) 2020 Edward C. Hennessy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <bblibrary.h>
#include <bbgedaitemfactory.h>
#include "bbgedaschematicreader.h"


#define VERSION_TOKEN "v"
#define OPEN_ATTRIBUTES_TOKEN "{"
#define CLOSE_ATTRIBUTES_TOKEN "}"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


struct _BbGedaSchematicReader
{
    GObject parent;

    BbGedaFactory *factory;
};


typedef struct _BbTaskData BbTaskData;

struct _BbTaskData
{
    BbGedaFactory *factory;

    BbSchematic *schematic;

    GDataInputStream *stream;
};


G_DEFINE_TYPE(BbGedaSchematicReader, bb_geda_schematic_reader, G_TYPE_OBJECT);


static void
bb_geda_schematic_reader_dispose(GObject *object);

static void
bb_geda_schematic_reader_finalize(GObject *object);

static void
bb_geda_schematic_reader_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_schematic_reader_read_item_line_ready(GDataInputStream *stream, GAsyncResult *result, GTask *task);

static void
bb_geda_schematic_reader_read_version_ready(GDataInputStream *stream, GAsyncResult *result, GTask *task);

static void
bb_geda_schematic_reader_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


static void
bb_geda_schematic_reader_class_init(BbGedaSchematicReaderClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_geda_schematic_reader_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_geda_schematic_reader_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_geda_schematic_reader_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_geda_schematic_reader_set_property;
}


static void
bb_geda_schematic_reader_dispose(GObject *object)
{
}


static void
bb_geda_schematic_reader_finalize(GObject *object)
{
}


static void
bb_geda_schematic_reader_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_1:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_schematic_reader_init(BbGedaSchematicReader *reader)
{
    reader->factory = bb_geda_factory_new();
}


void
bb_geda_schematic_reader_read_async(
    BbGedaSchematicReader *reader,
    GDataInputStream *stream,
    BbSchematic *schematic,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    GTask *task = g_task_new(
        reader,
        cancellable,
        callback,
        user_data
        );

    BbTaskData *task_data = g_new0(BbTaskData, 1);

    task_data->factory = reader->factory;
    task_data->schematic = schematic;
    task_data->stream = stream;

    g_task_set_task_data(
        task,
        task_data,
        g_free
        );

    g_data_input_stream_read_line_async(
        stream,
        G_PRIORITY_DEFAULT,
        cancellable,
        (GAsyncReadyCallback) bb_geda_schematic_reader_read_version_ready,
        task
        );
}


/**
 *
 *
 * @param stream
 * @param result
 * @param task
 */
static void
bb_geda_schematic_reader_read_attribute_ready(GDataInputStream *stream, GAsyncResult *result, GTask *task)
{
    g_assert(G_IS_DATA_INPUT_STREAM(stream));
    g_assert(G_IS_ASYNC_RESULT(result));
    g_assert(G_IS_TASK(task));

    GError *error = NULL;
    gsize length;

    char *line = g_data_input_stream_read_line_finish_utf8(stream, result, &length, &error);

    if (g_task_return_error_if_cancelled(task))
    {
        g_object_unref(task);
    }
    else if (error != NULL)
    {
        g_task_return_error(task, error);
        g_object_unref(task);
    }
    else if (line == NULL)
    {
        g_task_return_pointer(task, NULL, NULL);
        g_object_unref(task);
    }
    else if (length == 0)
    {
        g_task_return_new_error(task, BB_ERROR_DOMAIN, ERROR_UNEXPECTED_EMPTY_LINE, "Unexpected empty line");
        g_object_unref(task);
    }
    else
    {
        BbParams *params = bb_params_new_with_line(line, &error);

        if (error != NULL)
        {
            g_task_return_error(task, error);
            g_object_unref(task);
        }
        else if (bb_params_token_matches(params, CLOSE_ATTRIBUTES_TOKEN))
        {
            g_data_input_stream_read_line_async(
                stream,
                G_PRIORITY_DEFAULT,
                g_task_get_cancellable(task),
                (GAsyncReadyCallback) bb_geda_schematic_reader_read_item_line_ready,
                task
            );
        }
        else
        {
            g_data_input_stream_read_line_async(
                stream,
                G_PRIORITY_DEFAULT,
                g_task_get_cancellable(task),
                (GAsyncReadyCallback) bb_geda_schematic_reader_read_attribute_ready,
                task
                );
        }

        bb_params_free(params);
    }

    g_free(line);
}


/**
 *
 *
 * @param stream
 * @param result
 * @param task
 */
static void
bb_geda_schematic_reader_read_item_ready(BbGedaItemFactory *factory, GAsyncResult *result, GTask *task)
{
    g_assert(BB_IS_GEDA_FACTORY(factory));
    g_assert(G_IS_ASYNC_RESULT(result));
    g_assert(G_IS_TASK(task));

    BbTaskData *task_data = g_task_get_task_data(task);

    g_assert(task_data != NULL);

    GError *local_error = NULL;

    BbSchematicItem *item = bb_geda_item_factory_create_finish(factory, result, &local_error);

    if (local_error != NULL)
    {
        g_task_return_error(task, local_error);
        g_object_unref(task);
    }
    else if (item == NULL)
    {
        g_task_return_new_error(
            task,
            BB_ERROR_DOMAIN,
            0,
            "Internal error"
            );

        g_object_unref(task);
    }
    else
    {
        bb_schematic_add_item(task_data->schematic, g_object_ref(item));

        g_data_input_stream_read_line_async(
            task_data->stream,
            G_PRIORITY_DEFAULT,
            g_task_get_cancellable(task),
            (GAsyncReadyCallback) bb_geda_schematic_reader_read_item_line_ready,
            task
            );
    }
}


/**
 *
 *
 * @param stream
 * @param result
 * @param task
 */
static void
bb_geda_schematic_reader_read_item_line_ready(GDataInputStream *stream, GAsyncResult *result, GTask *task)
{
    g_assert(G_IS_DATA_INPUT_STREAM(stream));
    g_assert(G_IS_ASYNC_RESULT(result));
    g_assert(G_IS_TASK(task));

    BbTaskData *task_data = g_task_get_task_data(task);

    g_assert(task_data != NULL);

    GError *error = NULL;
    gsize length;

    char *line = g_data_input_stream_read_line_finish_utf8(stream, result, &length, &error);

    if (g_task_return_error_if_cancelled(task))
    {
        g_object_unref(task);
    }
    else if (error != NULL)
    {
        g_task_return_error(task, error);
        g_object_unref(task);
    }
    else if (line == NULL)
    {
        g_task_return_pointer(task, NULL, NULL);
        g_object_unref(task);
    }
    else if (length == 0)
    {
        g_task_return_new_error(task, BB_ERROR_DOMAIN, ERROR_UNEXPECTED_EMPTY_LINE, "Unexpected empty line");
        g_object_unref(task);
    }
    else
    {
        BbParams *params = bb_params_new_with_line(line, &error);

        if (error != NULL)
        {
            g_task_return_error(task, error);
            g_object_unref(task);
        }
        else if (bb_params_token_matches(params, OPEN_ATTRIBUTES_TOKEN))
        {
            g_data_input_stream_read_line_async(
                stream,
                G_PRIORITY_DEFAULT,
                g_task_get_cancellable(task),
                (GAsyncReadyCallback) bb_geda_schematic_reader_read_attribute_ready,
                task
                );
        }
        else
        {
            g_message("Item: %s", line);

            bb_geda_item_factory_create_async(
                BB_GEDA_ITEM_FACTORY(task_data->factory),
                NULL,
                params,
                stream,
                g_task_get_cancellable(task),
                (GAsyncReadyCallback) bb_geda_schematic_reader_read_item_ready,
                task
                );
        }

        bb_params_free(params);
    }

    g_free(line);
}


/**
 *
 *
 * @param stream
 * @param result
 * @param task
 */
static void
bb_geda_schematic_reader_read_version_ready(GDataInputStream *stream, GAsyncResult *result, GTask *task)
{
    g_assert(G_IS_DATA_INPUT_STREAM(stream));
    g_assert(G_IS_ASYNC_RESULT(result));
    g_assert(G_IS_TASK(task));

    GError *error = NULL;
    gsize length;

    char *line = g_data_input_stream_read_line_finish_utf8(stream, result, &length, &error);

    if (g_task_return_error_if_cancelled(task))
    {
        g_object_unref(task);
    }
    else if (error != NULL)
    {
        g_task_return_error(task, error);
        g_object_unref(task);
    }
    else if (line == NULL)
    {
        g_task_return_new_error(task, BB_ERROR_DOMAIN, ERROR_UNEXPECTED_EOF, "Unexpected EOF");
        g_object_unref(task);
    }
    else if (length == 0)
    {
        g_task_return_new_error(task, BB_ERROR_DOMAIN, ERROR_UNEXPECTED_EMPTY_LINE, "Unexpected empty line");
        g_object_unref(task);
    }
    else
    {
        BbParams *params = bb_params_new_with_line(line, &error);

        if (error != NULL)
        {
            g_task_return_error(task, error);
            g_object_unref(task);
        }
        else if (bb_params_token_matches(params, VERSION_TOKEN))
        {
            g_message("Version: %s", line);

            g_data_input_stream_read_line_async(
                stream,
                G_PRIORITY_DEFAULT,
                g_task_get_cancellable(task),
                (GAsyncReadyCallback) bb_geda_schematic_reader_read_item_line_ready,
                task
            );
        }
        else
        {
            g_task_return_new_error(task, BB_ERROR_DOMAIN, ERROR_EXPECTED_VERSION, "Expected version");
            g_object_unref(task);
        }

        bb_params_free(params);
    }

    g_free(line);
}


void*
bb_geda_schematic_reader_read_finish(
    BbGedaSchematicReader *reader,
    GAsyncResult *result,
    GError **error
    )
{
    g_assert(BB_IS_GEDA_SCHEMATIC_READER(reader));
    g_assert(g_task_is_valid(result, reader));

    return g_task_propagate_pointer(G_TASK(result), error);
}


static void
bb_geda_schematic_reader_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_1:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
