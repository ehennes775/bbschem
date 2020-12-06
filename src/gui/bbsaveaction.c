/*
 * bbsch
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
#include "bbsaveaction.h"
#include "bbschematicwindow.h"
#include "bbsavesubject.h"


enum
{
    PROP_0,
    PROP_ENABLED,
    PROP_NAME,
    PROP_PARAMETER_TYPE,
    PROP_STATE,
    PROP_STATE_HINT,
    PROP_STATE_TYPE,
    PROP_WINDOW,
    N_PROPERTIES
};


struct _BbSaveAction
{
    GObject parent;

    BbMainWindow* window;
};


static void
bb_save_action_action_init(GActionInterface *iface);

static void
bb_save_action_activate(GAction *action, GVariant *parameter);

static void
bb_save_action_change_state(GAction *action, GVariant *value);

static void
bb_save_action_dispose(GObject *object);

static void
bb_save_action_finalize(GObject *object);

static gboolean
bb_save_action_get_enabled(GAction *action);

static const gchar *
bb_save_action_get_name(GAction *action);

static const GVariantType *
bb_save_action_get_parameter_type(GAction *action);

static void
bb_save_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant *
bb_save_action_get_state(GAction *action);

static GVariant *
bb_save_action_get_state_hint(GAction *action);

static const GVariantType *
bb_save_action_get_state_type(GAction *action);

static void
bb_save_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbSaveAction,
    bb_save_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_save_action_action_init)
    )


static void
bb_save_action_action_init(GActionInterface *iface)
{
    iface->activate = bb_save_action_activate;
    iface->change_state = bb_save_action_change_state;
    iface->get_enabled = bb_save_action_get_enabled;
    iface->get_name = bb_save_action_get_name;
    iface->get_parameter_type = bb_save_action_get_parameter_type;
    iface->get_state = bb_save_action_get_state;
    iface->get_state_hint = bb_save_action_get_state_hint;
    iface->get_state_type = bb_save_action_get_state_type;
}


static void
bb_save_action_activate(GAction *action, GVariant *parameter)
{
    GObject *subject = G_OBJECT(bb_main_window_get_current_document_window(
        bb_save_action_get_window(BB_SAVE_ACTION(action))
        ));

    if (BB_IS_SAVE_SUBJECT(subject))
    {
        GError *error = NULL;

        bb_save_subject_save(
            BB_SAVE_SUBJECT(subject),
            &error
            );

        // TODO: error message dialog

        g_clear_error(&error);
    }
}


static void
bb_save_action_change_state(GAction *action, GVariant *value)
{

}


static void
bb_save_action_class_init(BbSaveActionClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_save_action_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_save_action_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_save_action_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_save_action_set_property;

    properties[PROP_ENABLED] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_ENABLED,
        "enabled"
        );

    properties[PROP_NAME] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_NAME,
        "name"
        );

    properties[PROP_PARAMETER_TYPE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_PARAMETER_TYPE,
        "parameter-type"
        );

    properties[PROP_STATE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_STATE,
        "state"
        );

    properties[PROP_STATE_TYPE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_STATE_TYPE,
        "state-type"
        );

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_WINDOW,
        properties[PROP_WINDOW] = g_param_spec_object(
            "window",
            "",
            "",
            BB_TYPE_MAIN_WINDOW,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

}


static void
bb_save_action_dispose(GObject *object)
{
}


static void
bb_save_action_finalize(GObject *object)
{
}


static gboolean
bb_save_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(action != NULL, FALSE);

    GObject *subject = G_OBJECT(bb_main_window_get_current_document_window(
        bb_save_action_get_window(BB_SAVE_ACTION(action))
        ));

    return
        BB_IS_SAVE_SUBJECT(subject) &&
        bb_save_subject_get_can_save(BB_SAVE_SUBJECT(subject));
}


static const gchar *
bb_save_action_get_name(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return "file-save";
}


static const GVariantType *
bb_save_action_get_parameter_type(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return NULL;
}


static void
bb_save_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, bb_save_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_save_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_save_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, bb_save_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_save_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_save_action_get_state_type(G_ACTION(object)));
            break;

        case PROP_WINDOW:
            g_value_set_object(value, bb_save_action_get_window(BB_SAVE_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static GVariant *
bb_save_action_get_state(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return NULL;
}


static GVariant *
bb_save_action_get_state_hint(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return NULL;
}


static const GVariantType *
bb_save_action_get_state_type(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return NULL;
}


BbMainWindow*
bb_save_action_get_window(BbSaveAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return action->window;
}


static void
bb_save_action_init(BbSaveAction *action)
{
    action->window = NULL;
}


BbSaveAction*
bb_save_action_new(BbMainWindow *window)
{
    return BB_SAVE_ACTION(g_object_new(
        BB_TYPE_SAVE_ACTION,
        "window", window,
        NULL
        ));
}


__attribute__((constructor)) void
bb_save_action_register()
{
    bb_save_action_get_type();
}


static void
bb_save_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WINDOW:
            bb_save_action_set_window(BB_SAVE_ACTION(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_save_action_set_window(BbSaveAction *action, BbMainWindow* window)
{
    g_return_if_fail(action != NULL);

    if (action->window != window)
    {
        if (action->window != NULL)
        {
            g_object_unref(action->window);
        }

        action->window = window;

        if (action->window != NULL)
        {
            g_object_ref(action->window);
        }

        g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_WINDOW]);
    }
}
