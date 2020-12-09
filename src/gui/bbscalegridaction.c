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
#include "bbscalegridaction.h"
#include "bbgridsubject.h"

enum
{
    PROP_0,
    PROP_ENABLED,
    PROP_NAME,
    PROP_PARAMETER_TYPE,
    PROP_STATE,
    PROP_STATE_HINT,
    PROP_STATE_TYPE,

    PROP_DIRECTION,
    PROP_SUBJECT,

    N_PROPERTIES
};


struct _BbScaleGridAction
{
    GObject parent;

    BbScaleGridDirection direction;

    GObject *subject;
};


static void
bb_scale_grid_action_action_init(GActionInterface *iface);

static void
bb_scale_grid_action_activate(GAction *action, GVariant *parameter);

static void
bb_scale_grid_action_change_state(GAction *action, GVariant *value);

static void
bb_scale_grid_action_dispose(GObject *object);

static void
bb_scale_grid_action_finalize(GObject *object);

static gboolean
bb_scale_grid_action_get_enabled(GAction *action);

static const gchar*
bb_scale_grid_action_get_name(GAction *action);

static const GVariantType*
bb_scale_grid_action_get_parameter_type(GAction *action);

static void
bb_scale_grid_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant*
bb_scale_grid_action_get_state(GAction *action);

static GVariant*
bb_scale_grid_action_get_state_hint(GAction *action);

static const GVariantType*
bb_scale_grid_action_get_state_type(GAction *action);

static void
bb_scale_grid_action_set_direction(BbScaleGridAction *action, BbScaleGridDirection direction);

static void
bb_scale_grid_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_scale_grid_action_set_subject(BbScaleGridAction *action, GObject* subject);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbScaleGridAction,
    bb_scale_grid_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_scale_grid_action_action_init)
    )


static void
bb_scale_grid_action_action_init(GActionInterface *iface)
{
    iface->activate = bb_scale_grid_action_activate;
    iface->change_state = bb_scale_grid_action_change_state;
    iface->get_enabled = bb_scale_grid_action_get_enabled;
    iface->get_name = bb_scale_grid_action_get_name;
    iface->get_parameter_type = bb_scale_grid_action_get_parameter_type;
    iface->get_state = bb_scale_grid_action_get_state;
    iface->get_state_hint = bb_scale_grid_action_get_state_hint;
    iface->get_state_type = bb_scale_grid_action_get_state_type;
}


static void
bb_scale_grid_action_activate(GAction *action, GVariant *parameter)
{
    g_return_if_fail(BB_IS_SCALE_GRID_ACTION(action));
    g_return_if_fail(parameter == NULL);

    GObject *subject = bb_scale_grid_action_get_subject(BB_SCALE_GRID_ACTION(action));

    if (BB_IS_GRID_SUBJECT(subject))
    {
        bb_grid_subject_scale(
            BB_GRID_SUBJECT(subject),
            bb_scale_grid_action_get_direction(BB_SCALE_GRID_ACTION(action))
            );
    }
}


static void
bb_scale_grid_action_change_state(GAction *action, GVariant *value)
{

}


static void
bb_scale_grid_action_class_init(BbScaleGridActionClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);

    object_class->dispose = bb_scale_grid_action_dispose;
    object_class->finalize = bb_scale_grid_action_finalize;
    object_class->get_property = bb_scale_grid_action_get_property;
    object_class->set_property = bb_scale_grid_action_set_property;

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

    properties[PROP_DIRECTION] = bb_object_class_install_property(
        object_class,
        PROP_DIRECTION,
        g_param_spec_int(
            "direction",
            "",
            "",
            0,
            G_MAXINT,
            0,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_SUBJECT] = bb_object_class_install_property(
        object_class,
        PROP_SUBJECT,
        g_param_spec_object(
            "subject",
            "",
            "",
            G_TYPE_OBJECT,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );
}


static void
bb_scale_grid_action_dispose(GObject *object)
{
    BbScaleGridAction *scale_action = BB_SCALE_GRID_ACTION(object);
    g_return_if_fail(scale_action != NULL);

    g_clear_object(&scale_action->subject);
}


static void
bb_scale_grid_action_finalize(GObject *object)
{
}


BbScaleGridDirection
bb_scale_grid_action_get_direction(BbScaleGridAction *action)
{
    BbScaleGridAction *scale_action = BB_SCALE_GRID_ACTION(action);
    g_return_val_if_fail(scale_action != NULL, BB_SCALE_GRID_DIRECTION_RESET);

    return scale_action->direction;
}


static gboolean
bb_scale_grid_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(BB_IS_SCALE_GRID_ACTION(action), FALSE);

    GObject *subject = bb_scale_grid_action_get_subject(BB_SCALE_GRID_ACTION(action));

    return
        BB_IS_GRID_SUBJECT(subject) &&
        bb_grid_subject_get_can_scale(
            BB_GRID_SUBJECT(subject),
            bb_scale_grid_action_get_direction(BB_SCALE_GRID_ACTION(action))
            );
}


static const gchar*
bb_scale_grid_action_get_name(GAction *action)
{
    BbScaleGridAction *scale_action = BB_SCALE_GRID_ACTION(action);
    g_return_val_if_fail(scale_action != NULL, NULL);

    switch (scale_action->direction)
    {
        case BB_SCALE_GRID_DIRECTION_DOWN:
            return "scale-down";

        case BB_SCALE_GRID_DIRECTION_RESET:
            return "scale-reset";

        case BB_SCALE_GRID_DIRECTION_UP:
            return "scale-up";

        default:
            g_return_val_if_reached(NULL);
    }
}


static const GVariantType*
bb_scale_grid_action_get_parameter_type(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return NULL;
}


static void
bb_scale_grid_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_DIRECTION:
            g_value_set_int(value, bb_scale_grid_action_get_direction(BB_SCALE_GRID_ACTION(object)));
            break;

        case PROP_ENABLED:
            g_value_set_boolean(value, bb_scale_grid_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_scale_grid_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_scale_grid_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, bb_scale_grid_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_scale_grid_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_scale_grid_action_get_state_type(G_ACTION(object)));
            break;

        case PROP_SUBJECT:
            g_value_set_object(value, bb_scale_grid_action_get_subject(BB_SCALE_GRID_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static GVariant *
bb_scale_grid_action_get_state(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static GVariant*
bb_scale_grid_action_get_state_hint(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static const GVariantType*
bb_scale_grid_action_get_state_type(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


GObject*
bb_scale_grid_action_get_subject(BbScaleGridAction *action)
{
    BbScaleGridAction *scale_action = BB_SCALE_GRID_ACTION(action);
    g_return_val_if_fail(scale_action != NULL, NULL);

    return scale_action->subject;
}


static void
bb_scale_grid_action_init(BbScaleGridAction *window)
{
}

BbScaleGridAction*
bb_scale_grid_action_new(BbMainWindow *main_window, BbScaleGridDirection direction)
{
    BbScaleGridAction *scale_grid_action = BB_SCALE_GRID_ACTION(g_object_new(
        BB_TYPE_SCALE_GRID_ACTION,
        "direction", direction,
        NULL
        ));

    g_object_bind_property(
        main_window,
        "current-document-window",
        scale_grid_action,
        "subject",
        G_BINDING_SYNC_CREATE
        );


    return scale_grid_action;
}


__attribute__((constructor)) void
bb_scale_grid_action_register()
{
    bb_scale_grid_action_get_type();
}


static void
bb_scale_grid_action_set_direction(BbScaleGridAction *action, BbScaleGridDirection direction)
{
    g_return_if_fail(BB_IS_SCALE_GRID_ACTION(action));

    if (action->direction != direction)
    {
        action->direction = direction;

        g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_DIRECTION]);
    }
}


static void
bb_scale_grid_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_DIRECTION:
            bb_scale_grid_action_set_direction(BB_SCALE_GRID_ACTION(object), g_value_get_int(value));
            break;

        case PROP_SUBJECT:
            bb_scale_grid_action_set_subject(BB_SCALE_GRID_ACTION(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_scale_grid_action_set_subject(BbScaleGridAction *action, GObject* subject)
{
    g_return_if_fail(BB_IS_SCALE_GRID_ACTION(action));

    if (action->subject != subject)
    {
        if (action->subject != NULL)
        {
            g_object_unref(action->subject);
        }

        action->subject = subject;

        if (action->subject != NULL)
        {
            g_object_ref(action->subject);
        }

        g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_SUBJECT]);
    }
}
