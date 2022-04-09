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
#include "bbopenaction.h"
#include "bbgeneralopener.h"
#include "gedaplugin/bbgedaeditor.h"


enum
{
    PROP_0,
    
    /* From GAction */
    
    PROP_ENABLED,
    PROP_NAME,
    PROP_PARAMETER_TYPE,
    PROP_STATE,
    PROP_STATE_HINT,
    PROP_STATE_TYPE,
    
    /* From BbOpenAction */

    PROP_RECEIVER,
    
    N_PROPERTIES
};


struct _BbOpenAction
{
    GObject parent;

    BbMainWindow *window;
};


typedef struct _TaskData TaskData;

struct _TaskData
{
    GFile *file;
    BbGeneralOpener *opener;
    BbMainWindow *window;
};


static void
bb_open_action_action_init(GActionInterface *iface);

static void
bb_open_action_activate(GAction *action, GVariant *parameter);

static void
bb_open_action_change_state(GAction *action, GVariant *value);

GtkWidget*
bb_open_action_create_dialog(BbOpenAction *open_action);

static void
bb_open_action_dispose(GObject *object);

static void
bb_open_action_finalize(GObject *object);

static gboolean
bb_open_action_get_enabled(GAction *action);

static const gchar*
bb_open_action_get_name(GAction *action);

static const GVariantType*
bb_open_action_get_parameter_type(GAction *action);

static void
bb_open_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant*
bb_open_action_get_state(GAction *action);

static GVariant*
bb_open_action_get_state_hint(GAction *action);

static const GVariantType*
bb_open_action_get_state_type(GAction *action);

static void
bb_open_action_open_uris(BbOpenAction *open_action, GSList *uris);

static void
bb_open_action_open_uris_lambda(const char *uri, BbOpenAction *open_action);

static void
bb_open_action_open_uris_lambda_ready(BbGeneralOpener *opener, GAsyncResult *result, TaskData *task_data);

static void
bb_open_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_open_action_set_window(BbOpenAction *open_action, BbMainWindow *window);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbOpenAction,
    bb_open_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_open_action_action_init)
    )


static void
bb_open_action_action_init(GActionInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->activate = bb_open_action_activate;
    iface->change_state = bb_open_action_change_state;
    iface->get_enabled = bb_open_action_get_enabled;
    iface->get_name = bb_open_action_get_name;
    iface->get_parameter_type = bb_open_action_get_parameter_type;
    iface->get_state = bb_open_action_get_state;
    iface->get_state_hint = bb_open_action_get_state_hint;
    iface->get_state_type = bb_open_action_get_state_type;
}


static void
bb_open_action_activate(GAction *action, GVariant *parameter)
{
    BbOpenAction *open_action = BB_OPEN_ACTION(action);
    g_return_if_fail(BB_IS_OPEN_ACTION(open_action));

    GtkWidget *dialog = bb_open_action_create_dialog(open_action);
    g_return_if_fail(dialog != NULL);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_ACCEPT)
    {
        GSList *uris = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(dialog));

        bb_open_action_open_uris(open_action, uris);

        g_slist_free_full(g_steal_pointer(&uris), g_free);
    }

    gtk_widget_destroy(dialog);
}


static void
bb_open_action_change_state(GAction *action, GVariant *value)
{
    g_warn_if_reached();
}


static void
bb_open_action_class_init(BbOpenActionClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(G_IS_OBJECT_CLASS(object_class));
    
    object_class->dispose = bb_open_action_dispose;
    object_class->finalize = bb_open_action_finalize;
    object_class->get_property = bb_open_action_get_property;
    object_class->set_property = bb_open_action_set_property;

    /* From GAction */

    g_object_class_override_property(
        object_class,
        PROP_ENABLED,
        "enabled"
        );

    g_object_class_override_property(
        object_class,
        PROP_NAME,
        "name"
        );

    g_object_class_override_property(
        object_class,
        PROP_PARAMETER_TYPE,
        "parameter-type"
        );

    g_object_class_override_property(
        object_class,
        PROP_STATE,
        "state"
        );

    g_object_class_override_property(
        object_class,
        PROP_STATE_TYPE,
        "state-type"
        );
        
    /* From BbOpenAction */

    properties[PROP_RECEIVER] = bb_object_class_install_property(
            object_class,
            PROP_RECEIVER,
            g_param_spec_object(
            "window",
            "",
            "",
            BB_TYPE_MAIN_WINDOW,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );
}


GtkWidget*
bb_open_action_create_dialog(BbOpenAction *open_action)
{
    g_return_val_if_fail(BB_IS_OPEN_ACTION(open_action), NULL);

    GtkWidget *dialog = gtk_file_chooser_dialog_new (
        "Open Files",
        GTK_WINDOW(open_action->window),
        GTK_FILE_CHOOSER_ACTION_OPEN,
        "Cancel",
        GTK_RESPONSE_CANCEL,
        "Open",
        GTK_RESPONSE_ACCEPT,
        NULL
        );

    gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(dialog), TRUE);

    return dialog;
}


static void
bb_open_action_dispose(GObject *object)
{
    BbOpenAction *open_action = BB_OPEN_ACTION(object);
    g_return_if_fail(BB_IS_OPEN_ACTION(open_action));

    g_clear_object(&open_action->window);
}


static void
bb_open_action_finalize(GObject *object)
{
    BbOpenAction *open_action = BB_OPEN_ACTION(object);
    g_return_if_fail(BB_IS_OPEN_ACTION(open_action));
}


static gboolean
bb_open_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(action != NULL, FALSE);

    return TRUE;
}


static const gchar*
bb_open_action_get_name(GAction *action)
{
    g_warn_if_fail(BB_IS_OPEN_ACTION(action));

    return "file-open";
}


static const GVariantType*
bb_open_action_get_parameter_type(GAction *action)
{
    g_warn_if_fail(BB_IS_OPEN_ACTION(action));

    return NULL;
}


static void
bb_open_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, bb_open_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_open_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_open_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, bb_open_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_open_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_open_action_get_state_type(G_ACTION(object)));
            break;

        case PROP_RECEIVER:
            g_value_set_object(value, bb_open_action_get_window(BB_OPEN_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static GVariant *
bb_open_action_get_state(GAction *action)
{
    g_warn_if_fail(BB_IS_OPEN_ACTION(action));

    return NULL;
}


static GVariant*
bb_open_action_get_state_hint(GAction *action)
{
    g_warn_if_fail(BB_IS_OPEN_ACTION(action));

    return NULL;
}


static const GVariantType*
bb_open_action_get_state_type(GAction *action)
{
    g_warn_if_fail(BB_IS_OPEN_ACTION(action));

    return NULL;
}


BbMainWindow*
bb_open_action_get_window(BbOpenAction *open_action)
{
    g_return_val_if_fail(BB_IS_OPEN_ACTION(open_action), NULL);

    return open_action->window;
}


static void
bb_open_action_init(BbOpenAction *open_action)
{
    g_return_if_fail(BB_IS_OPEN_ACTION(open_action));
}


BbOpenAction*
bb_open_action_new(BbMainWindow *window)
{
    return BB_OPEN_ACTION(g_object_new(
        BB_TYPE_OPEN_ACTION,
        "window", window,
        NULL
        ));
}


static void
bb_open_action_open_uris(BbOpenAction *open_action, GSList *uris)
{
    g_slist_foreach(uris, (GFunc) bb_open_action_open_uris_lambda, open_action);
}


/**
 * @brief Initiate identification of the content type
 *
 * @param uri
 * @param open_action
 */
static void
bb_open_action_open_uris_lambda(const char *uri, BbOpenAction *open_action)
{
    g_return_if_fail(uri != NULL);
    g_return_if_fail(BB_IS_OPEN_ACTION(open_action));

    TaskData *task_data = g_new0(TaskData, 1);

    // g_file_new_for_uri() gets an 'Operation not supported' error
    task_data->file = g_file_new_for_path(uri);
    task_data->opener = g_object_ref(bb_main_window_get_opener(open_action->window));
    task_data->window = g_object_ref(open_action->window);

    bb_general_opener_read_content_type_async(
        task_data->opener,
        task_data->file,
        NULL,
        (GAsyncReadyCallback) bb_open_action_open_uris_lambda_ready,
        task_data
        );
}


static void
bb_open_action_open_uris_lambda_ready(BbGeneralOpener *opener, GAsyncResult *result, TaskData *task_data)
{
    g_warn_if_fail(BB_IS_GENERAL_OPENER(opener));
    g_warn_if_fail(G_IS_ASYNC_RESULT(result));
    g_warn_if_fail(task_data != NULL);

    GError *local_error = NULL;

    gboolean success = bb_general_opener_read_content_type_finish(opener, result, &local_error);

    if (local_error != NULL)
    {
        gchar *basename = g_file_get_basename(task_data->file);

        GtkWidget *dialog = gtk_message_dialog_new_with_markup(
            GTK_WINDOW(task_data->window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "<b>%s</b>\n\nCannot open file: \n\n    %s",
            local_error->message,
            basename
            );

        g_free(basename);

        gtk_dialog_run(GTK_DIALOG(dialog));

        gtk_widget_destroy(dialog);
        g_clear_error(&local_error);
    }
    else if (!success)
    {
        gchar *basename = g_file_get_basename(task_data->file);

        GtkWidget *dialog = gtk_message_dialog_new(
            GTK_WINDOW(task_data->window),
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "<b>%s</b>\n\nCannot open file: \n\n    %s",
            "Internal Error",
            basename
            );

        g_free(basename);

        gtk_dialog_run(GTK_DIALOG(dialog));

        gtk_widget_destroy(dialog);
    }

    if (task_data != NULL)
    {
        g_clear_object(&task_data->file);
        g_clear_object(&task_data->opener);
        g_clear_object(&task_data->window);
        g_free(task_data);
    }
}


static void
bb_open_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_RECEIVER:
            bb_open_action_set_window(BB_OPEN_ACTION(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_open_action_set_window(BbOpenAction *open_action, BbMainWindow *window)
{
    g_return_if_fail(BB_IS_OPEN_ACTION(open_action));

    if (open_action->window != window)
    {
        if (open_action->window != NULL)
        {
            g_object_unref(open_action->window);
        }

        open_action->window = window;

        if (open_action->window != NULL)
        {
            g_object_ref(open_action->window);
        }

        g_object_notify_by_pspec(G_OBJECT(open_action), properties[PROP_RECEIVER]);
    }
}
