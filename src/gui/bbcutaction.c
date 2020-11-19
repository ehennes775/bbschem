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
#include "bbcutaction.h"

enum
{
    PROP_0,
    PROP_ENABLED,
    PROP_NAME,
    PROP_PARAMETER_TYPE,
    PROP_STATE,
    PROP_STATE_HINT,
    PROP_STATE_TYPE,
    N_PROPERTIES
};


struct _BbCutAction
{
    GObject parent;
};


static void
bb_cut_action_action_init(GActionInterface *iface);

static void
bb_cut_action_activate(GAction *action, GVariant *parameter);

static void
bb_cut_action_change_state(GAction *action, GVariant *value);

static void
bb_cut_action_dispose(GObject *object);

static void
bb_cut_action_finalize(GObject *object);

static gboolean
bb_cut_action_get_enabled(GAction *action);

static const gchar *
bb_cut_action_get_name(GAction *action);

static const GVariantType *
bb_cut_action_get_parameter_type(GAction *action);

static void
bb_cut_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant *
bb_cut_action_get_state(GAction *action);

static GVariant *
bb_cut_action_get_state_hint(GAction *action);

static const GVariantType *
bb_cut_action_get_state_type(GAction *action);

static void
bb_cut_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbCutAction,
    bb_cut_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_cut_action_action_init)
    )


static void
bb_cut_action_action_init(GActionInterface *iface)
{
    iface->activate = bb_cut_action_activate;
    iface->change_state = bb_cut_action_change_state;
    iface->get_enabled = bb_cut_action_get_enabled;
    iface->get_name = bb_cut_action_get_name;
    iface->get_parameter_type = bb_cut_action_get_parameter_type;
    iface->get_state = bb_cut_action_get_state;
    iface->get_state_hint = bb_cut_action_get_state_hint;
    iface->get_state_type = bb_cut_action_get_state_type;
}


static void
bb_cut_action_activate(GAction *action, GVariant *parameter)
{
    g_message("bb_cut_action_activate");
}


static void
bb_cut_action_change_state(GAction *action, GVariant *value)
{

}


static void
bb_cut_action_class_init(BbCutActionClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_cut_action_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_cut_action_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_cut_action_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_cut_action_set_property;

    g_object_class_override_property(
        klasse,
        PROP_ENABLED,
        "enabled"
    );

    g_object_class_override_property(
        klasse,
        PROP_NAME,
        "name"
    );

    g_object_class_override_property(
        klasse,
        PROP_PARAMETER_TYPE,
        "parameter-type"
    );

    g_object_class_override_property(
        klasse,
        PROP_STATE,
        "state"
    );

    //g_object_class_override_property(
    //    klasse,
    //    PROP_STATE_HINT,
    //    "state-hint"
    //    );

    g_object_class_override_property(
        klasse,
        PROP_STATE_TYPE,
        "state-type"
    );
}


static void
bb_cut_action_dispose(GObject *object)
{
    // BbCutAction* privat = BbCut_ACTION_GET_PRIVATE(object);
}


static void
bb_cut_action_finalize(GObject *object)
{
    // BbCutAction* privat = BbCut_ACTION_GET_PRIVATE(object);
}


static gboolean
bb_cut_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(action != NULL, FALSE);

    return TRUE;
}


static const gchar *
bb_cut_action_get_name(GAction *action)
{
    const gchar *name = "Cut";

    g_return_val_if_fail(action != NULL, name);

    return name;
}


static const GVariantType *
bb_cut_action_get_parameter_type(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static void
bb_cut_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, bb_cut_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_cut_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_cut_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, bb_cut_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_cut_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_cut_action_get_state_type(G_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static GVariant *
bb_cut_action_get_state(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static GVariant *
bb_cut_action_get_state_hint(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static const GVariantType *
bb_cut_action_get_state_type(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static void
bb_cut_action_init(BbCutAction *window)
{
}


BbCutAction*
bb_cut_action_new()
{
    return BB_CUT_ACTION(g_object_new(BB_TYPE_CUT_ACTION, NULL));
}


__attribute__((constructor)) void
bb_cut_action_register()
{
    bb_cut_action_get_type();
}


static void
bb_cut_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
