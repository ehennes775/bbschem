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
#include "bbnewaction.h"


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
    
    /* From BbNewAction */

    PROP_WINDOW,

    N_PROPERTIES
};


struct _BbNewAction
{
    GObject parent;

    BbMainWindow *window;
};


static void
bb_new_action_action_init(GActionInterface *iface);

static void
bb_new_action_activate(GAction *action, GVariant *parameter);

static void
bb_new_action_change_state(GAction *action, GVariant *value);

static void
bb_new_action_dispose(GObject *object);

static void
bb_new_action_finalize(GObject *object);

static gboolean
bb_new_action_get_enabled(GAction *action);

static const gchar*
bb_new_action_get_name(GAction *action);

static const GVariantType*
bb_new_action_get_parameter_type(GAction *action);

static void
bb_new_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant*
bb_new_action_get_state(GAction *action);

static GVariant*
bb_new_action_get_state_hint(GAction *action);

static const GVariantType*
bb_new_action_get_state_type(GAction *action);

BbMainWindow*
bb_new_action_get_window(BbNewAction *new_action);

static void
bb_new_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_new_action_set_window(BbNewAction *new_action, BbMainWindow *window);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbNewAction,
    bb_new_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_new_action_action_init)
    )


static void
bb_new_action_action_init(GActionInterface *iface)
{
    iface->activate = bb_new_action_activate;
    iface->change_state = bb_new_action_change_state;
    iface->get_enabled = bb_new_action_get_enabled;
    iface->get_name = bb_new_action_get_name;
    iface->get_parameter_type = bb_new_action_get_parameter_type;
    iface->get_state = bb_new_action_get_state;
    iface->get_state_hint = bb_new_action_get_state_hint;
    iface->get_state_type = bb_new_action_get_state_type;
}


static void
bb_new_action_activate(GAction *action, GVariant *parameter)
{
    g_return_if_fail(BB_IS_NEW_ACTION(action));
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_STRING));

    g_message("bb_new_action_activate = %s", g_variant_get_string(parameter, NULL));
}


static void
bb_new_action_change_state(GAction *action, GVariant *value)
{
    g_warn_if_reached();
}


static void
bb_new_action_class_init(BbNewActionClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(G_IS_OBJECT_CLASS(object_class));
    
    object_class->dispose = bb_new_action_dispose;
    object_class->finalize = bb_new_action_finalize;
    object_class->get_property = bb_new_action_get_property;
    object_class->set_property = bb_new_action_set_property;

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
        
    /* From BbNewAction */

    properties[PROP_WINDOW] = bb_object_class_install_property(
        object_class,
        PROP_WINDOW,
        g_param_spec_object(
            "window",
            "",
            "",
            BB_TYPE_MAIN_WINDOW,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );
}


static void
bb_new_action_dispose(GObject *object)
{
    BbNewAction *new_action = BB_NEW_ACTION(object);
    g_return_if_fail(BB_IS_NEW_ACTION(new_action));

    g_clear_object(&new_action->window);
}


static void
bb_new_action_finalize(GObject *object)
{
    BbNewAction *new_action = BB_NEW_ACTION(object);
    g_return_if_fail(BB_IS_NEW_ACTION(new_action));
}


static gboolean
bb_new_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(BB_IS_NEW_ACTION(action), FALSE);

    return TRUE;
}


static const gchar*
bb_new_action_get_name(GAction *action)
{
    g_warn_if_fail(BB_IS_NEW_ACTION(action));

    return "file-new";
}


static const GVariantType*
bb_new_action_get_parameter_type(GAction *action)
{
    g_warn_if_fail(BB_IS_NEW_ACTION(action));

    return G_VARIANT_TYPE_STRING;
}


static void
bb_new_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, bb_new_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_new_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_new_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, bb_new_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_new_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_new_action_get_state_type(G_ACTION(object)));
            break;

        case PROP_WINDOW:
            g_value_set_object(value, bb_new_action_get_window(BB_NEW_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static GVariant *
bb_new_action_get_state(GAction *action)
{
    g_warn_if_fail(BB_IS_NEW_ACTION(action));

    return NULL;
}


static GVariant*
bb_new_action_get_state_hint(GAction *action)
{
    g_warn_if_fail(BB_IS_NEW_ACTION(action));

    return NULL;
}


static const GVariantType*
bb_new_action_get_state_type(GAction *action)
{
    g_warn_if_fail(BB_IS_NEW_ACTION(action));

    return NULL;
}


BbMainWindow*
bb_new_action_get_window(BbNewAction *new_action)
{
    g_return_val_if_fail(BB_IS_NEW_ACTION(new_action), NULL);

    return new_action->window;
}


static void
bb_new_action_init(BbNewAction *new_action)
{
    g_return_if_fail(BB_IS_NEW_ACTION(new_action));
}


BbNewAction*
bb_new_action_new(BbMainWindow *window)
{
    return BB_NEW_ACTION(g_object_new(
        BB_TYPE_NEW_ACTION,
        "window", window,
        NULL
        ));
}


static void
bb_new_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WINDOW:
            bb_new_action_set_window(BB_NEW_ACTION(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_new_action_set_window(BbNewAction *new_action, BbMainWindow *window)
{
    g_return_if_fail(BB_IS_NEW_ACTION(new_action));

    if (new_action->window != window)
    {
        if (new_action->window != NULL)
        {
            g_object_unref(new_action->window);
        }

        new_action->window = window;

        if (new_action->window != NULL)
        {
            g_object_ref(new_action->window);
        }

        g_object_notify_by_pspec(G_OBJECT(new_action), properties[PROP_WINDOW]);
    }
}
