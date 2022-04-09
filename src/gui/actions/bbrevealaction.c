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
#include "bbextensions.h"
#include "bbrevealaction.h"
#include "bbrevealreceiver.h"

enum
{
    PROP_0,
    PROP_ENABLED,
    PROP_NAME,
    PROP_PARAMETER_TYPE,
    PROP_STATE,
    PROP_STATE_HINT,
    PROP_STATE_TYPE,
    PROP_RECEIVER,
    N_PROPERTIES
};


struct _BbRevealAction
{
    GObject parent;

    GObject* receiver;
};


static void
bb_reveal_action_action_init(GActionInterface *iface);

static void
bb_reveal_action_activate(GAction *action, GVariant *parameter);

static void
bb_reveal_action_change_state(GAction *action, GVariant *value);

static void
bb_reveal_action_dispose(GObject *object);

static void
bb_reveal_action_finalize(GObject *object);

static gboolean
bb_reveal_action_get_enabled(GAction *action);

static const gchar*
bb_reveal_action_get_name(GAction *action);

static const GVariantType*
bb_reveal_action_get_parameter_type(GAction *action);

static void
bb_reveal_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant*
bb_reveal_action_get_state(GAction *action);

static GVariant*
bb_reveal_action_get_state_hint(GAction *action);

static const GVariantType*
bb_reveal_action_get_state_type(GAction *action);

static void
bb_reveal_action_notify(gpointer unused, GParamSpec *pspec, BbRevealAction *action);

static void
bb_reveal_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbRevealAction,
    bb_reveal_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_reveal_action_action_init)
    )


static void
bb_reveal_action_action_init(GActionInterface *iface)
{
    iface->activate = bb_reveal_action_activate;
    iface->change_state = bb_reveal_action_change_state;
    iface->get_enabled = bb_reveal_action_get_enabled;
    iface->get_name = bb_reveal_action_get_name;
    iface->get_parameter_type = bb_reveal_action_get_parameter_type;
    iface->get_state = bb_reveal_action_get_state;
    iface->get_state_hint = bb_reveal_action_get_state_hint;
    iface->get_state_type = bb_reveal_action_get_state_type;
}


static void
bb_reveal_action_activate(GAction *action, GVariant *parameter)
{
    g_return_if_fail(BB_IS_REVEAL_ACTION(action));

    GObject *receiver = bb_reveal_action_get_receiver(BB_REVEAL_ACTION(action));

    g_return_if_fail(BB_IS_REVEAL_RECEIVER(receiver));

    bb_reveal_receiver_set_reveal(
        BB_REVEAL_RECEIVER(receiver),
        !bb_reveal_receiver_get_reveal(BB_REVEAL_RECEIVER(receiver))
        );
}


static void
bb_reveal_action_change_state(GAction *action, GVariant *value)
{
}


static void
bb_reveal_action_class_init(BbRevealActionClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);

    object_class->dispose = bb_reveal_action_dispose;
    object_class->finalize = bb_reveal_action_finalize;
    object_class->get_property = bb_reveal_action_get_property;
    object_class->set_property = bb_reveal_action_set_property;

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

    properties[PROP_RECEIVER] = bb_object_class_install_property(
            object_class,
            PROP_RECEIVER,
            g_param_spec_object(
            "receiver",
            "",
            "",
            BB_TYPE_REVEAL_RECEIVER,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
        )
    );

}


static void
bb_reveal_action_dispose(GObject *object)
{
    BbRevealAction *action = BB_REVEAL_ACTION(object);

    g_return_if_fail(action != NULL);

    bb_reveal_action_set_receiver(action, NULL);

    G_OBJECT_GET_CLASS(bb_reveal_action_parent_class)->dispose(object);
}


static void
bb_reveal_action_finalize(GObject *object)
{
}


static gboolean
bb_reveal_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(BB_IS_REVEAL_ACTION(action), FALSE);

    GObject *receiver = bb_reveal_action_get_receiver(BB_REVEAL_ACTION(action));

    return BB_IS_REVEAL_RECEIVER(receiver);
}


static const gchar*
bb_reveal_action_get_name(GAction *action)
{
    g_warn_if_fail(BB_IS_REVEAL_ACTION(action));

    return "view-reveal";
}


static const GVariantType*
bb_reveal_action_get_parameter_type(GAction *action)
{
    g_return_val_if_fail(BB_IS_REVEAL_ACTION(action), NULL);

    return NULL;
}


static void
bb_reveal_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, bb_reveal_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_reveal_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_reveal_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, bb_reveal_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_reveal_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_reveal_action_get_state_type(G_ACTION(object)));
            break;

        case PROP_RECEIVER:
            g_value_set_object(value, bb_reveal_action_get_receiver(BB_REVEAL_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static GVariant *
bb_reveal_action_get_state(GAction *action)
{
    g_return_val_if_fail(BB_IS_REVEAL_ACTION(action), NULL);

    GObject *receiver = bb_reveal_action_get_receiver(BB_REVEAL_ACTION(action));

    return g_variant_ref(g_variant_new_boolean(
        BB_REVEAL_RECEIVER(receiver) ? bb_reveal_receiver_get_reveal(BB_REVEAL_RECEIVER(receiver)) : FALSE
        ));
}


static GVariant*
bb_reveal_action_get_state_hint(GAction *action)
{
    g_warn_if_fail(BB_IS_REVEAL_ACTION(action));

    return NULL;
}


static const GVariantType*
bb_reveal_action_get_state_type(GAction *action)
{
    g_warn_if_fail(BB_IS_REVEAL_ACTION(action));

    return G_VARIANT_TYPE_BOOLEAN;
}


GObject*
bb_reveal_action_get_receiver(BbRevealAction *action)
{
    g_return_val_if_fail(BB_IS_REVEAL_ACTION(action), NULL);

    BbRevealAction *reveal_action = BB_REVEAL_ACTION(action);
    g_return_val_if_fail(reveal_action != NULL, NULL);

    return reveal_action->receiver;
}


static void
bb_reveal_action_init(BbRevealAction *action)
{
    g_signal_connect(
        action,
        "notify::subject",
        G_CALLBACK(bb_reveal_action_notify),
        action
        );
}


BbRevealAction*
bb_reveal_action_new()
{
    BbRevealAction *reveal_action = BB_REVEAL_ACTION(g_object_new(
        BB_TYPE_REVEAL_ACTION,
        NULL
        ));

    return reveal_action;
}


static void
bb_reveal_action_notify(gpointer unused, GParamSpec *pspec, BbRevealAction *action)
{
    g_return_if_fail(BB_IS_REVEAL_ACTION(action));

    g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_ENABLED]);
    g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_STATE]);
}


static void
bb_reveal_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_RECEIVER:
            bb_reveal_action_set_receiver(BB_REVEAL_ACTION(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_reveal_action_set_receiver(BbRevealAction *action, GObject* receiver)
{
    g_return_if_fail(BB_IS_REVEAL_ACTION(action));

    if (action->receiver != receiver)
    {
        if (action->receiver != NULL)
        {
            g_signal_handlers_disconnect_by_func(
                receiver,
                "notify::reveal",
                G_CALLBACK(bb_reveal_action_notify)
                );

            g_object_unref(action->receiver);
        }

        action->receiver = receiver;

        if (action->receiver != NULL)
        {
            g_object_ref(action->receiver);

            g_signal_connect(
                receiver,
                "notify::reveal",
                G_CALLBACK(bb_reveal_action_notify),
                action
                );
        }

        g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_RECEIVER]);
    }
}
