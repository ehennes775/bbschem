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
    N_PROPERTIES
};


struct _BbGeneralOpener
{
    GObject parent;

    /**
     * The key contains the content type string, dynamically allocated
     * The value contains the specific opener
     */
    GHashTable *openers;
};


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
bb_general_opener_read_content_type_ready_1(GFile *file, GAsyncResult *result, GTask *task);

static void
bb_general_opener_read_content_type_ready_2(BbSpecificOpener *specific_opener, GAsyncResult *result, GTask *task);

static void
bb_general_opener_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE(BbGeneralOpener, bb_general_opener, G_TYPE_OBJECT);


G_MODULE_EXPORT void
bb_general_opener_add_specific_opener(
    BbGeneralOpener *general_opener,
    gchar *content_type,
    BbSpecificOpener *specific_opener
    )
{
    g_return_if_fail(BB_IS_GENERAL_OPENER(general_opener));
    g_return_if_fail(content_type != NULL);
    g_return_if_fail(BB_IS_SPECIFIC_OPENER(specific_opener));

    g_message("count: %d", g_hash_table_size(general_opener->openers));

    g_hash_table_insert(
        general_opener->openers,
        g_strdup(content_type),
        g_object_ref(specific_opener)
        );

    g_message("count: %d", g_hash_table_size(general_opener->openers));
}


static void
bb_general_opener_class_init(BbGeneralOpenerClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(G_IS_OBJECT_CLASS(object_class));

    object_class->dispose = bb_general_opener_dispose;
    object_class->finalize = bb_general_opener_finalize;
    object_class->get_property = bb_general_opener_get_property;
    object_class->set_property = bb_general_opener_set_property;
}


static void
bb_general_opener_dispose(GObject *object)
{
    BbGeneralOpener *general_opener = BB_GENERAL_OPENER(object);
    g_return_if_fail(BB_IS_GENERAL_OPENER(general_opener));

    if (general_opener->openers != NULL)
    {
        g_hash_table_destroy(g_steal_pointer(&general_opener->openers));
    }
}


static void
bb_general_opener_finalize(GObject *object)
{
}


static void
bb_general_opener_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_general_opener_init(BbGeneralOpener *general_opener)
{
    g_return_if_fail(BB_IS_GENERAL_OPENER(general_opener));

    general_opener->openers = g_hash_table_new_full(
        g_str_hash,
        g_str_equal,
        g_free,
        g_object_unref
        );
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
    BbSchematic *schematic = bb_schematic_new();

    GTask *task = g_task_new(
        opener,
        cancellable,
        callback,
        user_data
        );

    g_task_set_task_data(
        task,
        schematic,
        NULL
        );

    g_file_read_async(
        file,
        G_PRIORITY_DEFAULT,
        cancellable,
        (GAsyncReadyCallback) bb_general_opener_open_open_ready,
        task
        );

    return BB_DOCUMENT_WINDOW(bb_schematic_window_new(
        schematic
        ));
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
            BB_SCHEMATIC(g_task_get_task_data(task)),
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


void
bb_general_opener_read_content_type_async(
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

    g_file_query_info_async(
        file,
        G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE,
        G_FILE_QUERY_INFO_NONE,
        G_PRIORITY_DEFAULT,
        cancellable,
        (GAsyncReadyCallback) bb_general_opener_read_content_type_ready_1,
        task
        );
}


static void
bb_general_opener_read_content_type_ready_1(GFile *file, GAsyncResult *result, GTask *task)
{
    g_assert(G_IS_FILE(file));
    g_assert(G_IS_TASK(task));
    g_assert(g_task_is_valid(result, file));

    GError *local_error = NULL;

    GFileInfo *file_info = g_file_query_info_finish(file, result, &local_error);

    if (g_task_return_error_if_cancelled(task))
    {
        g_object_unref(task);
    }
    else if (local_error != NULL)
    {
        g_task_return_error(task, local_error);
        g_object_unref(task);
    }
    else if (file_info == NULL)
    {
        g_task_return_boolean(task, FALSE);
        g_object_unref(task);
    }
    else
    {
        const gchar *content_type = g_file_info_get_attribute_string(
            file_info,
            G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE
            );

        BbGeneralOpener *general_opener = BB_GENERAL_OPENER(g_task_get_source_object(task));

        BbSpecificOpener *specific_opener = BB_SPECIFIC_OPENER(g_hash_table_lookup(
            general_opener->openers,
            content_type
            ));

        g_message("count: %d", g_hash_table_size(general_opener->openers));

        if (specific_opener == NULL)
        {
            g_task_return_new_error(
                task,
                BB_ERROR_DOMAIN,
                0,
                "Unknown content type: '%s'",
                content_type
                );

            g_object_unref(task);
        }
        else
        {
            bb_specific_opener_open_async(
                specific_opener,
                file,
                g_task_get_cancellable(task),
                (GAsyncReadyCallback) bb_general_opener_read_content_type_ready_2,
                task
                );
        }
    }
}


static void
bb_general_opener_read_content_type_ready_2(BbSpecificOpener *specific_opener, GAsyncResult *result, GTask *task)
{
    GError *local_error = NULL;

    gboolean success = bb_specific_opener_open_finish(specific_opener, result, &local_error);

    if (g_task_return_error_if_cancelled(task))
    {
        g_object_unref(task);
    }
    else if (local_error != NULL)
    {
        g_task_return_error(task, local_error);
        g_object_unref(task);
    }
    else
    {
        g_task_return_boolean(task, success);
        g_object_unref(task);
    }
}


gboolean
bb_general_opener_read_content_type_finish(
    BbGeneralOpener *opener,
    GAsyncResult *result,
    GError **error
    )
{
    g_assert(BB_IS_GENERAL_OPENER(opener));
    g_assert(g_task_is_valid(result, opener));

    return g_task_propagate_boolean(G_TASK(result), error);
}


static void
bb_general_opener_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
