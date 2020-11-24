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
#include "bbchoosetoolaction.h"
#include "bbextensions.h"

enum
{
    PROP_0,
    PROP_ENABLED,
    PROP_NAME,
    PROP_PARAMETER_TYPE,
    PROP_STACK,
    PROP_STATE,
    PROP_STATE_HINT,
    PROP_STATE_TYPE,
    PROP_WINDOW,
    N_PROPERTIES
};


enum
{
    SIG_ACTIVATE,
    SIG_CHANGE_STATE,
    N_SIGNALS
};


struct _BbChooseToolAction
{
    GObject parent;

    GtkStack *stack;
    BbMainWindow *window;
};


static void
bb_choose_tool_action_action_init(GActionInterface *iface);

static void
bb_choose_tool_action_activate(GAction *action, GVariant *parameter);

static void
bb_choose_tool_action_change_state(GAction *action, GVariant *value);

static void
bb_choose_tool_action_dispose(GObject *object);

static void
bb_choose_tool_action_finalize(GObject *object);

static gboolean
bb_choose_tool_action_get_enabled(GAction *action);

static const gchar*
bb_choose_tool_action_get_name(GAction *action);

static const GVariantType*
bb_choose_tool_action_get_parameter_type(GAction *action);

static void
bb_choose_tool_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant *
bb_choose_tool_action_get_state(GAction *action);

static GVariant *
bb_choose_tool_action_get_state_hint(GAction *action);

static const GVariantType *
bb_choose_tool_action_get_state_type(GAction *action);

static void
bb_choose_tool_action_stack_notify_cb(GtkStack *stack, GParamSpec *pspec, BbChooseToolAction *action);

static void
bb_choose_tool_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_choose_tool_action_set_stack(BbChooseToolAction *action, GtkStack *stack);

static void
bb_choose_tool_action_set_state(BbChooseToolAction *action, GVariant *state);

static void
bb_choose_tool_action_set_window(BbChooseToolAction *action, BbMainWindow *window);


GParamSpec *properties[N_PROPERTIES];
guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbChooseToolAction,
    bb_choose_tool_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_choose_tool_action_action_init)
    )


static void
bb_choose_tool_action_action_init(GActionInterface *iface)
{
    iface->activate = bb_choose_tool_action_activate;
    iface->change_state = bb_choose_tool_action_change_state;
    iface->get_enabled = bb_choose_tool_action_get_enabled;
    iface->get_name = bb_choose_tool_action_get_name;
    iface->get_parameter_type = bb_choose_tool_action_get_parameter_type;
    iface->get_state = bb_choose_tool_action_get_state;
    iface->get_state_hint = bb_choose_tool_action_get_state_hint;
    iface->get_state_type = bb_choose_tool_action_get_state_type;
}


static void
bb_choose_tool_action_activate(GAction *action, GVariant *parameter)
{
    bb_choose_tool_action_change_state(action, parameter);

    // TODO SIGSEGV
    // g_signal_emit(action, signals[SIG_ACTIVATE], 0);
}


static void
bb_choose_tool_action_change_state(GAction *action, GVariant *value)
{
    bb_choose_tool_action_set_state(BB_CHOOSE_TOOL_ACTION(action), value);

    g_signal_emit(action, signals[SIG_CHANGE_STATE], 0, value);
}


static void
bb_choose_tool_action_class_init(BbChooseToolActionClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_choose_tool_action_dispose;
    object_class->finalize = bb_choose_tool_action_finalize;
    object_class->get_property = bb_choose_tool_action_get_property;
    object_class->set_property = bb_choose_tool_action_set_property;

    properties[PROP_ENABLED] = bb_object_class_override_property(
        object_class,
        PROP_ENABLED,
        "enabled"
        );

    properties[PROP_NAME] = bb_object_class_override_property(
        object_class,
        PROP_NAME,
        "name"
        );

    properties[PROP_PARAMETER_TYPE] = bb_object_class_override_property(
        object_class,
        PROP_PARAMETER_TYPE,
        "parameter-type"
        );

    properties[PROP_STATE] = bb_object_class_override_property(
        object_class,
        PROP_STATE,
        "state"
        );

    properties[PROP_STATE_TYPE] = bb_object_class_override_property(
        object_class,
        PROP_STATE_TYPE,
        "state-type"
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_STACK,
        properties[PROP_STACK] = g_param_spec_object(
            "stack",
            "",
            "",
            GTK_TYPE_STACK,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_WINDOW,
        properties[PROP_WINDOW] = g_param_spec_object(
            "window",
            "",
            "",
            BB_TYPE_MAIN_WINDOW,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );

    signals[SIG_ACTIVATE] = g_signal_new(
        "activate",
        BB_TYPE_CHOOSE_TOOL_ACTION,
        G_SIGNAL_RUN_LAST | G_SIGNAL_MUST_COLLECT,
        0,
        NULL,
        NULL,
        NULL,
        G_TYPE_NONE,
        1,
        G_TYPE_VARIANT
        );

    signals[SIG_CHANGE_STATE] = g_signal_new(
        "change-state",
        BB_TYPE_CHOOSE_TOOL_ACTION,
        G_SIGNAL_RUN_LAST | G_SIGNAL_MUST_COLLECT,
        0,
        NULL,
        NULL,
        NULL,
        G_TYPE_NONE,
        1,
        G_TYPE_VARIANT
        );
}


static void
bb_choose_tool_action_dispose(GObject *object)
{
    BbChooseToolAction *action = BB_CHOOSE_TOOL_ACTION(object);
    g_return_if_fail(action != NULL);

    g_clear_object(&action->stack);
    g_clear_object(&action->window);
}


static void
bb_choose_tool_action_finalize(GObject *object)
{
}


static gboolean
bb_choose_tool_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(action != NULL, FALSE);

    return TRUE;
}


static const gchar*
bb_choose_tool_action_get_name(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return "select-tool";
}


static const GVariantType*
bb_choose_tool_action_get_parameter_type(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return G_VARIANT_TYPE_STRING;
}


static void
bb_choose_tool_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, bb_choose_tool_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_choose_tool_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_choose_tool_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STACK:
            g_value_set_object(value, bb_choose_tool_action_get_stack(BB_CHOOSE_TOOL_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_take_variant(value, bb_choose_tool_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_choose_tool_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_choose_tool_action_get_state_type(G_ACTION(object)));
            break;

        case PROP_WINDOW:
            g_value_set_object(value, bb_choose_tool_action_get_window(BB_CHOOSE_TOOL_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


GtkStack*
bb_choose_tool_action_get_stack(BbChooseToolAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return action->stack;
}


static GVariant*
bb_choose_tool_action_get_state(GAction *action)
{
    BbChooseToolAction *choose_action = BB_CHOOSE_TOOL_ACTION(action);
    g_return_val_if_fail(choose_action != NULL, NULL);
    g_return_val_if_fail(choose_action->stack != NULL, NULL);

    const gchar* name = gtk_stack_get_visible_child_name(choose_action->stack);

    return name != NULL ? g_variant_ref(g_variant_new_string(name)) : NULL;
}


static GVariant*
bb_choose_tool_action_get_state_hint(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static const GVariantType*
bb_choose_tool_action_get_state_type(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return G_VARIANT_TYPE_STRING;
}


BbMainWindow*
bb_choose_tool_action_get_window(BbChooseToolAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return action->window;
}


static void
bb_choose_tool_action_init(BbChooseToolAction *window)
{
}


BbChooseToolAction*
bb_choose_tool_action_new(BbMainWindow *window, GtkStack *stack)
{
    return BB_CHOOSE_TOOL_ACTION(g_object_new(
        BB_TYPE_CHOOSE_TOOL_ACTION,
        "stack", stack,
        //"window", window,
        NULL
        ));
}


static void
bb_choose_tool_action_stack_notify_cb(GtkStack *stack, GParamSpec *pspec, BbChooseToolAction *action)
{
    g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_STATE]);
}


static void
bb_choose_tool_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_STACK:
            bb_choose_tool_action_set_stack(BB_CHOOSE_TOOL_ACTION(object), GTK_STACK(g_value_get_object(value)));
            break;

        case PROP_WINDOW:
            bb_choose_tool_action_set_window(BB_CHOOSE_TOOL_ACTION(object), BB_MAIN_WINDOW(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_choose_tool_action_set_stack(BbChooseToolAction *action, GtkStack *stack)
{
    g_return_if_fail(action != NULL);

    if (action->stack != stack)
    {
        if (action->stack != NULL)
        {
            g_signal_handlers_disconnect_by_data(action->stack, action);

            g_object_unref(action->stack);
        }

        action->stack = stack;

        if (action->stack != NULL)
        {
            g_object_ref(action->stack);

            g_signal_connect_object(
                action->stack,
                "notify::visible-child",
                G_CALLBACK(bb_choose_tool_action_stack_notify_cb),
                action,
                0
                );
        }

        g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_STACK]);
    }
}


static void
bb_choose_tool_action_set_state(BbChooseToolAction *action, GVariant *state)
{
    BbChooseToolAction *choose_tool_action = BB_CHOOSE_TOOL_ACTION(action);

    g_return_if_fail(choose_tool_action != NULL);
    g_return_if_fail(g_variant_is_of_type(state, G_VARIANT_TYPE_STRING));

    const gchar *name = g_variant_get_string(state, NULL);

    gtk_stack_set_visible_child_name(choose_tool_action->stack, name);
}


static void
bb_choose_tool_action_set_window(BbChooseToolAction *action, BbMainWindow *window)
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
