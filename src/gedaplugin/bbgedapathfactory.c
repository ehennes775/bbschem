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
#include "bbgedapath.h"
#include "bbgedapathfactory.h"
#include "bbgedaitemfactory.h"
#include "bberror.h"
#include "bbpathparser.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


struct _BbGedaPathFactory
{
    GObject parent;
};


typedef struct _TaskData TaskData;

struct _TaskData
{
    int index;
    gchar **lines;
    int line_count;
    BbParams *params;
};


BbGedaItem*
bb_geda_path_factory_create(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GError **error
    );

static void
bb_geda_path_factory_create_async(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );

static void
bb_geda_path_factory_create_ready_line(GDataInputStream *stream, GAsyncResult *result, GTask *task);

static void
bb_geda_path_factory_dispose(GObject *object);

static void
bb_geda_path_factory_finalize(GObject *object);

static void
bb_geda_path_factory_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_path_factory_item_factory_init(BbGedaItemFactoryInterface *iface);

static void
bb_geda_path_factory_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_geda_path_factory_task_data_free(TaskData *task_data);

TaskData*
bb_geda_path_factory_task_data_new(BbParams *params, GError **error);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_DYNAMIC_TYPE_EXTENDED(
    BbGedaPathFactory,
    bb_geda_path_factory,
    G_TYPE_OBJECT,
    0,
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_GEDA_ITEM_FACTORY, bb_geda_path_factory_item_factory_init)
    )


BbGedaItem*
bb_geda_path_factory_create(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GError **error
    )
{
    return BB_GEDA_ITEM(bb_geda_path_new_with_params(params, NULL, error));
}


static void
bb_geda_path_factory_create_async(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    GError *local_error = NULL;

    g_assert(BB_IS_GEDA_ITEM_FACTORY(factory));
    g_assert(params != NULL);
    g_assert(G_IS_DATA_INPUT_STREAM(stream));
    g_assert(cancellable == NULL || G_IS_CANCELLABLE(cancellable));
    g_assert(callback != NULL);

    GTask *task = g_task_new(factory, cancellable, callback, user_data);

    TaskData *task_data = bb_geda_path_factory_task_data_new(params, &local_error);

    if (local_error == NULL && task_data == NULL)
    {
        local_error = g_error_new(BB_ERROR_DOMAIN, 0, "Internal error");
    }

    if (local_error == NULL)
    {
        g_task_set_task_data(task, task_data, (GDestroyNotify) bb_geda_path_factory_task_data_free);

        g_data_input_stream_read_line_async(
            stream,
            G_PRIORITY_DEFAULT,
            cancellable,
            (GAsyncReadyCallback) bb_geda_path_factory_create_ready_line,
            task
            );
    }

    if (local_error != NULL)
    {
        g_task_return_error(task, local_error);
        g_object_unref(task);
    }
}


static void
bb_geda_path_factory_create_ready_line(GDataInputStream *stream, GAsyncResult *result, GTask *task)
{
    GError *local_error = NULL;

    g_assert(G_IS_DATA_INPUT_STREAM(stream));
    g_assert(G_IS_ASYNC_RESULT(result));
    g_assert(G_IS_TASK(task));

    TaskData *task_data = g_task_get_task_data(task);

    g_assert(task_data != NULL);

    gchar *line = g_data_input_stream_read_line_finish(stream, result, NULL, &local_error);

    if (local_error == NULL && line == NULL)
    {
        local_error = g_error_new(BB_ERROR_DOMAIN, 0, "Internal error");
    }

    if (local_error == NULL)
    {
        *(task_data->lines + task_data->index) = line;

        task_data->index++;

        if (task_data->index < task_data->line_count)
        {
            g_data_input_stream_read_line_async(
                stream,
                G_PRIORITY_DEFAULT,
                g_task_get_cancellable(task),
                (GAsyncReadyCallback) bb_geda_path_factory_create_ready_line,
                task
                );
        }
        else
        {
            BbGedaPath *path = NULL;
            gchar *merged = g_strjoinv(" ", task_data->lines);
            GSList *commands = bb_path_parser_parse(merged, &local_error);
            g_free(merged);

            if (local_error == NULL)
            {
                path = bb_geda_path_new_with_params(task_data->params, commands, &local_error);

                if (local_error == NULL && path == NULL)
                {
                    local_error = g_error_new(
                        BB_ERROR_DOMAIN,
                        0,
                        "Internal error: " __FILE__ " line %d", __LINE__
                        );
                }
            }


            if (local_error == NULL)
            {
                g_task_return_pointer(task, path, NULL);
                g_object_unref(task);
            }
        }
    }

    if (local_error != NULL)
    {
        g_task_return_error(task, local_error);
        g_object_unref(task);
    }
}



static void
bb_geda_path_factory_class_init(BbGedaPathFactoryClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_geda_path_factory_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_geda_path_factory_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_geda_path_factory_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_geda_path_factory_set_property;
}


static void
bb_geda_path_factory_class_finalize(BbGedaPathFactoryClass *klasse)
{
}


static void
bb_geda_path_factory_dispose(GObject *object)
{
}


static void
bb_geda_path_factory_finalize(GObject *object)
{
}


static void
bb_geda_path_factory_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
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
bb_geda_path_factory_init(BbGedaPathFactory *window)
{
}


static void
bb_geda_path_factory_item_factory_init(BbGedaItemFactoryInterface *iface)
{
    iface->create = bb_geda_path_factory_create;
    iface->create_async = bb_geda_path_factory_create_async;
}

BbGedaItemFactory*
bb_geda_path_factory_new()
{
    return BB_GEDA_ITEM_FACTORY(g_object_new(
        BB_TYPE_GEDA_PATH_FACTORY,
        NULL
        ));
}

void
bb_geda_path_factory_register(GTypeModule *module)
{
    bb_geda_path_factory_register_type(module);
}


static void
bb_geda_path_factory_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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
bb_geda_path_factory_task_data_free(TaskData *task_data)
{
    if (task_data != NULL)
    {
        bb_params_free(task_data->params);
        g_strfreev(task_data->lines);

        g_free(task_data);
    }
}


TaskData*
bb_geda_path_factory_task_data_new(BbParams *params, GError **error)
{
    GError *local_error = NULL;
    TaskData *task_data = NULL;

    int line_count = bb_geda_path_get_line_count(params, &local_error);

    if (local_error == NULL && line_count <= 0)
    {
        local_error = g_error_new(BB_ERROR_DOMAIN, 0, "Internal Error");
    }

    if (local_error == NULL)
    {
        task_data = g_new0(TaskData, 1);

        task_data->index = 0;
        task_data->line_count = line_count;
        task_data->lines = g_new0(gchar*, line_count + 1);
        task_data->params = bb_params_copy(params);
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        bb_geda_path_factory_task_data_free(task_data);
        task_data = NULL;
    }

    return task_data;
}

