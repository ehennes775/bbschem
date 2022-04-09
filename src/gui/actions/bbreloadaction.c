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
#include "bbreloadaction.h"
#include "gedaplugin/bbgedaeditor.h"

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


struct _BbReloadAction
{
    GObject parent;

    GObject* receiver;
};


static void
bb_reload_action_action_init(GActionInterface *iface);

static void
bb_reload_action_activate(GAction *action, GVariant *parameter);

static void
bb_reload_action_change_state(GAction *action, GVariant *value);

static void
bb_reload_action_dispose(GObject *object);

static void
bb_reload_action_finalize(GObject *object);

static gboolean
bb_reload_action_get_enabled(GAction *action);

static const gchar *
bb_reload_action_get_name(GAction *action);

static const GVariantType *
bb_reload_action_get_parameter_type(GAction *action);

static void
bb_reload_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant *
bb_reload_action_get_state(GAction *action);

static GVariant *
bb_reload_action_get_state_hint(GAction *action);

static const GVariantType *
bb_reload_action_get_state_type(GAction *action);

static void
bb_reload_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbReloadAction,
    bb_reload_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_reload_action_action_init)
    )


static void
bb_reload_action_action_init(GActionInterface *iface)
{
    iface->activate = bb_reload_action_activate;
    iface->change_state = bb_reload_action_change_state;
    iface->get_enabled = bb_reload_action_get_enabled;
    iface->get_name = bb_reload_action_get_name;
    iface->get_parameter_type = bb_reload_action_get_parameter_type;
    iface->get_state = bb_reload_action_get_state;
    iface->get_state_hint = bb_reload_action_get_state_hint;
    iface->get_state_type = bb_reload_action_get_state_type;
}


static void
bb_reload_action_activate(GAction *action, GVariant *parameter)
{
    //GtkWidget *receiver = bb_main_receiver_get_current_document_receiver(
    //    bb_reload_action_get_receiver(BB_RELOAD_ACTION(action))
    //    );

    //if (BB_IS_SCHEMATIC_RECEIVER(receiver))
    //{
    //    GError *error = NULL/;
//
//        bb_schematic_receiver_reload(BB_SCHEMATIC_RECEIVER(receiver), &error);
//
//        g_clear_error(&error);
//    }
}


static void
bb_reload_action_change_state(GAction *action, GVariant *value)
{

}


static void
bb_reload_action_class_init(BbReloadActionClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_reload_action_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_reload_action_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_reload_action_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_reload_action_set_property;

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
bb_reload_action_dispose(GObject *object)
{
}


static void
bb_reload_action_finalize(GObject *object)
{
}


static gboolean
bb_reload_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(action != NULL, FALSE);

    //GtkWidget *receiver = bb_main_receiver_get_current_document_receiver(
   ///     bb_reload_action_get_receiver(BB_RELOAD_ACTION(action))
     //   );

    return FALSE; // TODO
        //BB_IS_SCHEMATIC_RECEIVER(receiver) &&
        //bb_schematic_receiver_get_can_reload(BB_SCHEMATIC_RECEIVER(receiver));
}


static const gchar *
bb_reload_action_get_name(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return "file-reload";
}


static const GVariantType *
bb_reload_action_get_parameter_type(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return NULL;
}


static void
bb_reload_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, bb_reload_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_reload_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_reload_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, bb_reload_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_reload_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_reload_action_get_state_type(G_ACTION(object)));
            break;

        case PROP_RECEIVER:
            g_value_set_object(value, bb_reload_action_get_receiver(BB_RELOAD_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static GVariant *
bb_reload_action_get_state(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return NULL;
}


static GVariant *
bb_reload_action_get_state_hint(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return NULL;
}


static const GVariantType *
bb_reload_action_get_state_type(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return NULL;
}


GObject *
bb_reload_action_get_receiver(BbReloadAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return action->receiver;
}


static void
bb_reload_action_init(BbReloadAction *action)
{
    action->receiver = NULL;
}


BbReloadAction*
bb_reload_action_new()
{
    return BB_RELOAD_ACTION(g_object_new(
        BB_TYPE_RELOAD_ACTION,
        NULL
        ));
}


static void
bb_reload_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_RECEIVER:
            bb_reload_action_set_receiver(BB_RELOAD_ACTION(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_reload_action_set_receiver(BbReloadAction *action, GObject* receiver)
{
    g_return_if_fail(action != NULL);

    if (action->receiver != receiver)
    {
        if (action->receiver != NULL)
        {
            g_object_unref(action->receiver);
        }

        action->receiver = receiver;

        if (action->receiver != NULL)
        {
            g_object_ref(action->receiver);
        }

        g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_RECEIVER]);
    }
}
