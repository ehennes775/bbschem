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
#include "bbselectallaction.h"
#include "bbselectreceiver.h"


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


struct _BbSelectAllAction
{
    GObject parent;

    GObject* receiver;
};


static void
bb_select_all_action_action_init(GActionInterface *iface);

static void
bb_select_all_action_activate(GAction *action, GVariant *parameter);

static void
bb_select_all_action_change_state(GAction *action, GVariant *value);

static void
bb_select_all_action_dispose(GObject *object);

static void
bb_select_all_action_finalize(GObject *object);

static void
bb_select_all_action_generic_receiver_init(BbGenericReceiverInterface *iface);

static gboolean
bb_select_all_action_get_enabled(GAction *action);

static const gchar *
bb_select_all_action_get_name(GAction *action);

static const GVariantType *
bb_select_all_action_get_parameter_type(GAction *action);

static void
bb_select_all_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant *
bb_select_all_action_get_state(GAction *action);

static GVariant *
bb_select_all_action_get_state_hint(GAction *action);

static const GVariantType *
bb_select_all_action_get_state_type(GAction *action);

static void
bb_select_all_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbSelectAllAction,
    bb_select_all_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_select_all_action_action_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_GENERIC_RECEIVER, bb_select_all_action_generic_receiver_init)
    )

// region From BbGenericReceiver interface

static GObject *
bb_select_all_action_generic_receiver_get_receiver(BbGenericReceiver *object)
{
    return bb_select_all_action_get_receiver(BB_SELECT_ALL_ACTION(object));
}

static void
bb_select_all_action_generic_receiver_set_receiver(BbGenericReceiver *object, GObject *receiver)
{
    bb_select_all_action_set_receiver(BB_SELECT_ALL_ACTION(object), receiver);
}

static void
bb_select_all_action_generic_receiver_init(BbGenericReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_receiver = bb_select_all_action_generic_receiver_get_receiver;
    iface->set_receiver = bb_select_all_action_generic_receiver_set_receiver;
}

// endregion

static void
bb_select_all_action_action_init(GActionInterface *iface)
{
    iface->activate = bb_select_all_action_activate;
    iface->change_state = bb_select_all_action_change_state;
    iface->get_enabled = bb_select_all_action_get_enabled;
    iface->get_name = bb_select_all_action_get_name;
    iface->get_parameter_type = bb_select_all_action_get_parameter_type;
    iface->get_state = bb_select_all_action_get_state;
    iface->get_state_hint = bb_select_all_action_get_state_hint;
    iface->get_state_type = bb_select_all_action_get_state_type;
}


static void
bb_select_all_action_activate(GAction *action, GVariant *parameter)
{
    GObject *receiver = bb_select_all_action_get_receiver(BB_SELECT_ALL_ACTION(action));

    if (BB_IS_SELECT_RECEIVER(receiver))
    {
        bb_select_receiver_select_all(BB_SELECT_RECEIVER(receiver));
    }
}


static void
bb_select_all_action_change_state(GAction *action, GVariant *value)
{

}


static void
bb_select_all_action_class_init(BbSelectAllActionClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_select_all_action_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_select_all_action_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_select_all_action_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_select_all_action_set_property;

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
            PROP_RECEIVER,
            properties[PROP_RECEIVER] = g_param_spec_object(
            "receiver",
            "",
            "",
            G_TYPE_OBJECT,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

}


static void
bb_select_all_action_dispose(GObject *object)
{
    BbSelectAllAction *action = BB_SELECT_ALL_ACTION(object);

    g_return_if_fail(action != NULL);

    bb_select_all_action_set_receiver(action, NULL);

    G_OBJECT_GET_CLASS(bb_select_all_action_parent_class)->dispose(object);
}


static void
bb_select_all_action_finalize(GObject *object)
{
}


static gboolean
bb_select_all_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(action != NULL, FALSE);

    GObject *receiver = bb_select_all_action_get_receiver(BB_SELECT_ALL_ACTION(action));

    return
        BB_IS_SELECT_RECEIVER(receiver) &&
        bb_select_receiver_can_select_all(BB_SELECT_RECEIVER(receiver));
}


static const gchar *
bb_select_all_action_get_name(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return "edit-select-all";
}


static const GVariantType *
bb_select_all_action_get_parameter_type(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static void
bb_select_all_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, bb_select_all_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_select_all_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_select_all_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, bb_select_all_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_select_all_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_select_all_action_get_state_type(G_ACTION(object)));
            break;

        case PROP_RECEIVER:
            g_value_set_object(value, bb_select_all_action_get_receiver(BB_SELECT_ALL_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


GObject*
bb_select_all_action_get_receiver(BbSelectAllAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return action->receiver;
}


static GVariant *
bb_select_all_action_get_state(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static GVariant *
bb_select_all_action_get_state_hint(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static const GVariantType *
bb_select_all_action_get_state_type(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static void
bb_select_all_action_init(BbSelectAllAction *action)
{
    action->receiver = NULL;
}


BbSelectAllAction*
bb_select_all_action_new()
{
    return BB_SELECT_ALL_ACTION(g_object_new(
        BB_TYPE_SELECT_ALL_ACTION,
        NULL
        ));
}


static void
bb_copy_action_notify_can_select_all(GObject *unused, GParamSpec *pspec, GObject *action)
{
    g_object_notify_by_pspec(action, properties[PROP_ENABLED]);
}


static void
bb_select_all_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_RECEIVER:
            bb_select_all_action_set_receiver(BB_SELECT_ALL_ACTION(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_select_all_action_set_receiver(BbSelectAllAction *action, GObject* receiver)
{
    g_return_if_fail(action != NULL);

    if (action->receiver != receiver)
    {
        if (action->receiver != NULL)
        {
            g_signal_handlers_disconnect_by_data(receiver, action);

            g_object_unref(action->receiver);
        }

        action->receiver = receiver;

        if (action->receiver != NULL)
        {
            g_object_ref(action->receiver);

            g_signal_connect(
                receiver,
                "notify::can-select-all",
                G_CALLBACK(bb_copy_action_notify_can_select_all),
                action
                );
        }

        g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_ENABLED]);
        g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_RECEIVER]);
    }
}
