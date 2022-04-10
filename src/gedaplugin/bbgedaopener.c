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
#include <bbextensions.h>
#include "actions/bbzoomreceiver.h"
#include "bbgedaopener.h"
#include "bbgedareader.h"
#include "bbspecificopener.h"
#include "bbgedaeditor.h"


enum
{
    PROP_0,

    PROP_MAIN_WINDOW,
    PROP_READER,

    N_PROPERTIES
};


struct _BbGedaOpener
{
    GObject parent;

    BbMainWindow *main_window;

    BbGedaReader *reader;
};


// region Function prototypes

static void
bb_geda_opener_dispose(
    GObject *object
    );

static void
bb_geda_opener_finalize(
    GObject *object
    );

static void
bb_geda_opener_get_property(
    GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_opener_open_ready_1(GFile *file, GAsyncResult *result, GTask *task);

static void
bb_geda_opener_open_ready_2(BbGedaReader *reader, GAsyncResult *result, GTask *task);

static void
bb_geda_opener_set_property(
    GObject *object,
    guint property_id,
    const GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_opener_set_reader(
    BbGedaOpener *opener,
    BbGedaReader *reader
    );

static void
bb_geda_opener_specific_opener_init(
    BbSpecificOpenerInterface *iface
    );

// endregion


GParamSpec *properties[N_PROPERTIES];

G_DEFINE_TYPE_EXTENDED(
    BbGedaOpener,
    bb_geda_opener,
    G_TYPE_OBJECT,
    0,
    G_IMPLEMENT_INTERFACE(BB_TYPE_SPECIFIC_OPENER, bb_geda_opener_specific_opener_init)
    )

// region From BbSpecificOpener

static void
bb_geda_opener_open_async(
    BbSpecificOpener *specific_opener,
    GFile *file,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    GTask *task = g_task_new(
        specific_opener,
        cancellable,
        callback,
        user_data
        );

    g_file_read_async(
        file,
        G_PRIORITY_DEFAULT,
        NULL,
        (GAsyncReadyCallback) bb_geda_opener_open_ready_1,
        task
        );
}


static void
bb_geda_opener_open_ready_1(GFile *file, GAsyncResult *result, GTask *task)
{
    GError *local_error = NULL;

    GFileInputStream *file_stream = g_file_read_finish(file, result, &local_error);

    if (g_task_return_error_if_cancelled(task))
    {
        g_object_unref(task);
    }
    else if (local_error != NULL)
    {
        g_object_unref(task);
    }
    else if (file_stream == NULL)
    {
        g_object_unref(task);
    }
    else
    {
        GDataInputStream *data_stream = g_data_input_stream_new(G_INPUT_STREAM(file_stream));
        BbGedaOpener *opener = BB_GEDA_OPENER(g_task_get_source_object(task));
        BbSchematic *schematic = bb_schematic_new();

        BbGedaEditor *editor = bb_geda_editor_new(
            file,
            schematic,
            bb_main_window_get_tool_changer(opener->main_window)
            );

        bb_main_window_add_page(
            opener->main_window,
            BB_DOCUMENT_WINDOW(editor)
            );

        bb_geda_reader_read_async(
            bb_geda_opener_get_reader(BB_GEDA_OPENER(opener)),
            data_stream,
            schematic,
            NULL,
            (GAsyncReadyCallback) bb_geda_opener_open_ready_2,
            task
            );
    }
}


static void
bb_geda_opener_open_ready_2(BbGedaReader *reader, GAsyncResult *result, GTask *task)
{
    GError *local_error = NULL;

    gboolean success = TRUE; bb_geda_reader_read_finish(reader, result, &local_error);    // TODO

    if (g_task_return_error_if_cancelled(task))
    {
        g_object_unref(task);
    }
    else if (local_error != NULL)
    {
        g_task_return_error(task, local_error);
        g_object_unref(task);
    }
    else if (!success)
    {
        g_task_return_boolean(task, success);
        g_object_unref(task);
    }
    else
    {
        g_task_return_boolean(task, success);
        g_object_unref(task);
    }
}


static gboolean
bb_geda_opener_open_finish(BbSpecificOpener *opener, GAsyncResult *result, GError **error)
{
    g_assert(BB_IS_GEDA_OPENER(opener));
    g_assert(g_task_is_valid(result, opener));

    return g_task_propagate_boolean(G_TASK(result), error);
}


static void
bb_geda_opener_specific_opener_init(BbSpecificOpenerInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->open_async = bb_geda_opener_open_async;
    iface->open_finish = bb_geda_opener_open_finish;
}

// endregion


static void
bb_geda_opener_class_init(BbGedaOpenerClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_geda_opener_dispose;
    object_class->finalize = bb_geda_opener_finalize;
    object_class->get_property = bb_geda_opener_get_property;
    object_class->set_property = bb_geda_opener_set_property;

    properties[PROP_MAIN_WINDOW] = bb_object_class_install_property(
        object_class,
        PROP_MAIN_WINDOW,
        g_param_spec_object(
            "main-window",
            "",
            "",
            BB_TYPE_MAIN_WINDOW,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_READER] = bb_object_class_install_property(
        object_class,
        PROP_READER,
        g_param_spec_object(
            "reader",
            "",
            "",
            BB_TYPE_GEDA_READER,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );
}


static void
bb_geda_opener_class_finalize(BbGedaOpenerClass *opener_class)
{
}


static void
bb_geda_opener_dispose(GObject *object)
{
    bb_geda_opener_set_reader(BB_GEDA_OPENER(object), NULL);
}


static void
bb_geda_opener_finalize(GObject *object)
{
}


static void
bb_geda_opener_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_READER:
            g_value_set_object(value, bb_geda_opener_get_reader(BB_GEDA_OPENER(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


BbGedaReader*
bb_geda_opener_get_reader(BbGedaOpener *opener)
{
    g_return_val_if_fail(BB_IS_GEDA_OPENER(opener), NULL);

    return opener->reader;
}


static void
bb_geda_opener_init(BbGedaOpener *opener)
{
}


BbGedaOpener*
bb_geda_opener_new(BbMainWindow *main_window)
{
    BbGedaReader *reader = BB_GEDA_READER(g_object_new(
        BB_TYPE_GEDA_READER,
        NULL
        ));

    return BB_GEDA_OPENER(g_object_new(
        BB_TYPE_GEDA_OPENER,
        "main-window", main_window,
        "reader", reader,
        NULL
        ));
}

#if 0
void
bb_geda_opener_register(GTypeModule *module)
{
    bb_geda_opener_register_type(module);
}
#endif


static void
bb_geda_opener_set_main_window(BbGedaOpener *geda_opener, BbMainWindow *main_window)
{
    g_return_if_fail(BB_IS_GEDA_OPENER(geda_opener));
    g_return_if_fail(BB_IS_MAIN_WINDOW(main_window));

    if (geda_opener->main_window != main_window)
    {
        if (geda_opener->main_window != NULL)
        {
            g_object_unref(geda_opener->main_window);
        }

        geda_opener->main_window = main_window;

        if (geda_opener->main_window != NULL)
        {
            g_object_ref(geda_opener->main_window);
        }

        g_object_notify_by_pspec(G_OBJECT(geda_opener), properties[PROP_MAIN_WINDOW]);
    }
}


static void
bb_geda_opener_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_READER:
            bb_geda_opener_set_reader(BB_GEDA_OPENER(object), g_value_get_object(value));
            break;

        case PROP_MAIN_WINDOW:
            bb_geda_opener_set_main_window(BB_GEDA_OPENER(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_opener_set_reader(BbGedaOpener *opener, BbGedaReader *reader)
{
    g_return_if_fail(BB_IS_GEDA_OPENER(opener));

    if (opener->reader != reader)
    {
        if (opener->reader != NULL)
        {
            g_object_unref(opener->reader);
        }

        opener->reader = reader;

        if (opener->reader != NULL)
        {
            g_object_ref(opener->reader);
        }

        g_object_notify_by_pspec(G_OBJECT(opener), properties[PROP_READER]);
    }
}
