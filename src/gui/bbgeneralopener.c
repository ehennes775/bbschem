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
#include "bbgeneralopener.h"
#include "bbgedaschematicreader.h"
#include "bbschematicwindow.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


struct _BbGeneralOpener
{
    GObject parent;
};


G_DEFINE_TYPE(BbGeneralOpener, bb_general_opener, G_TYPE_OBJECT);


static void
bb_general_opener_dispose(GObject *object);

static void
bb_general_opener_finalize(GObject *object);

static void
bb_general_opener_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_general_opener_open_open_ready(GFile *file, GAsyncResult *result, GTask *task);

static void
bb_general_opener_open_read_ready(BbGedaSchematicReader *reader, GAsyncResult *result, GTask *task);

static void
bb_general_opener_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


static void
bb_general_opener_class_init(BbGeneralOpenerClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_general_opener_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_general_opener_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_general_opener_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_general_opener_set_property;
}


static void
bb_general_opener_dispose(GObject *object)
{
    // BbGeneralOpener* privat = BBGENERAL_OPENER_GET_PRIVATE(object);
}


static void
bb_general_opener_finalize(GObject *object)
{
    // BbGeneralOpener* privat = BBGENERAL_OPENER_GET_PRIVATE(object);
}


static void
bb_general_opener_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
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
bb_general_opener_init(BbGeneralOpener *window)
{
}


BbGeneralOpener*
bb_general_opener_new()
{
    return BB_GENERAL_OPENER(g_object_new(
        BB_TYPE_GENERAL_OPENER,
        NULL
        ));
}


static void*
bb_general_opener_open_ready(BbGedaSchematicReader *reader, GAsyncResult *result, BbGeneralOpener *opener)
{
    GError *local_error = NULL;

    bb_geda_schematic_reader_read_finish(
        reader,
        result,
        &local_error
        );

    if (local_error != NULL)
    {
        g_message("Error: %s", local_error->message);
    }

    g_message("Done");
}


BbDocumentWindow*
bb_general_opener_open_async(
    BbGeneralOpener *opener,
    GFile *file,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    GTask *task = g_task_new(
        opener,
        cancellable,
        callback,
        user_data
        );

    g_file_read_async(
        file,
        G_PRIORITY_DEFAULT,
        cancellable,
        (GAsyncReadyCallback) bb_general_opener_open_open_ready,
        task
        );

    return g_object_new(
        BB_TYPE_SCHEMATIC_WINDOW,
        "grid-control", NULL,  // TODO
        "tool-changer", NULL,    // TODO
        NULL
        );
}


static void
bb_general_opener_open_open_ready(GFile *file, GAsyncResult *result, GTask *task)
{
    GError *error = NULL;

    GFileInputStream *file_stream = g_file_read_finish(file, result, &error);

    if (g_task_return_error_if_cancelled(task))
    {
        g_object_unref(task);
    }
    else if (error != NULL)
    {
        g_task_return_error(task, error);
        g_object_unref(task);
    }
    else if (file_stream == NULL)
    {
        g_task_return_new_error(task, BB_ERROR_DOMAIN, 0, "Internal Error");
        g_object_unref(task);
    }
    else
    {
        GDataInputStream *data_stream = g_data_input_stream_new(G_INPUT_STREAM(file_stream));
        BbGedaSchematicReader *reader = BB_GEDA_SCHEMATIC_READER(g_object_new(BB_TYPE_GEDA_SCHEMATIC_READER, NULL));

        bb_geda_schematic_reader_read_async(
            reader,
            data_stream,
            g_task_get_cancellable(task),
            (GAsyncReadyCallback) bb_general_opener_open_read_ready,
            task
            );
    }

}


static void
bb_general_opener_open_read_ready(BbGedaSchematicReader *reader, GAsyncResult *result, GTask *task)
{
    GError *error = NULL;

    bb_geda_schematic_reader_read_finish(reader, result, &error);

    g_object_unref(reader);

    if (g_task_return_error_if_cancelled(task))
    {
        g_object_unref(task);
    }
    else if (error != NULL)
    {
        g_task_return_error(task, error);
        g_object_unref(task);
    }
    else
    {
        g_task_return_pointer(task, NULL, NULL);
        g_object_unref(task);
    }
}


void*
bb_general_opener_open_finish(
    BbGeneralOpener *opener,
    GAsyncResult *result,
    GError **error
    )
{
    g_assert(BB_IS_GENERAL_OPENER(opener));
    g_assert(g_task_is_valid(result, opener));

    return g_task_propagate_pointer(G_TASK(result), error);
}


static void
bb_general_opener_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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
