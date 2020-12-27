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
#include "bbpathcommand.h"
#include "bbrelativemoveto.h"
#include "bbcoord.h"


enum
{
    PROP_0,
    PROP_DX,
    PROP_DY,
    N_PROPERTIES
};


struct _BbRelativeMoveTo
{
    BbPathCommand parent_instance;

    int dx;
    int dy;
};


G_DEFINE_TYPE(BbRelativeMoveTo, bb_relative_move_to, BB_TYPE_PATH_COMMAND);


static BbPathCommand*
bb_relative_move_to_clone(BbPathCommand *command);

static void
bb_relative_move_to_dispose(GObject *object);

static void
bb_relative_move_to_finalize(GObject *object);

static void
bb_relative_move_to_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_relative_move_to_mirror_x(BbPathCommand *command, int cx);

static void
bb_relative_move_to_mirror_y(BbPathCommand *command, int cy);

static void
bb_relative_move_to_render(BbPathCommand *command, BbItemRenderer *renderer);

static void
bb_relative_move_to_rotate(BbPathCommand *command, int cx, int cy, int angle);

static void
bb_relative_move_to_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_relative_move_to_translate(BbPathCommand *command, int dx, int dy);


static GParamSpec *properties[N_PROPERTIES];


static void
bb_relative_move_to_class_init(BbRelativeMoveToClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_relative_move_to_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_relative_move_to_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_relative_move_to_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_relative_move_to_set_property;

    BB_PATH_COMMAND_CLASS(klasse)->clone = bb_relative_move_to_clone;
    BB_PATH_COMMAND_CLASS(klasse)->mirror_x = bb_relative_move_to_mirror_x;
    BB_PATH_COMMAND_CLASS(klasse)->mirror_y = bb_relative_move_to_mirror_y;
    BB_PATH_COMMAND_CLASS(klasse)->render = bb_relative_move_to_render;
    BB_PATH_COMMAND_CLASS(klasse)->rotate = bb_relative_move_to_rotate;
    BB_PATH_COMMAND_CLASS(klasse)->translate = bb_relative_move_to_translate;

    properties[PROP_DX] = g_param_spec_int(
        "dx",
        "dx",
        "",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_DY] = g_param_spec_int(
        "dy",
        "dy",
        "",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    for (int index = PROP_0 + 1; index < N_PROPERTIES; ++index)
    {
        bb_object_class_install_property(
            G_OBJECT_CLASS(klasse),
            index,
            properties[index]
        );
    }

}


static BbPathCommand*
bb_relative_move_to_clone(BbPathCommand *command)
{
    BbRelativeMoveTo *move_to = BB_RELATIVE_MOVE_TO(command);

    return BB_PATH_COMMAND(bb_relative_move_to_new(
        bb_relative_move_to_get_dx(move_to),
        bb_relative_move_to_get_dy(move_to)
        ));
}


static void
bb_relative_move_to_dispose(GObject *object)
{
}


static void
bb_relative_move_to_finalize(GObject *object)
{
}


static void
bb_relative_move_to_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_DX:
            g_value_set_int(value, bb_relative_move_to_get_dx(BB_RELATIVE_MOVE_TO(object)));
            break;

        case PROP_DY:
            g_value_set_int(value, bb_relative_move_to_get_dy(BB_RELATIVE_MOVE_TO(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_relative_move_to_get_dx(const BbRelativeMoveTo *command)
{
    g_return_val_if_fail(command != NULL, 0);

    return command->dx;
}


int
bb_relative_move_to_get_dy(const BbRelativeMoveTo *command)
{
    g_return_val_if_fail(command != NULL, 0);

    return command->dy;
}


static void
bb_relative_move_to_init(BbRelativeMoveTo *window)
{
}


static void
bb_relative_move_to_mirror_x(BbPathCommand *command, int cx)
{
    bb_relative_move_to_set_dx(
        BB_RELATIVE_MOVE_TO(command),
        - bb_relative_move_to_get_dx(BB_RELATIVE_MOVE_TO(command))
        );
}


static void
bb_relative_move_to_mirror_y(BbPathCommand *command, int cy)
{
    bb_relative_move_to_set_dy(
        BB_RELATIVE_MOVE_TO(command),
        - bb_relative_move_to_get_dy(BB_RELATIVE_MOVE_TO(command))
        );
}


BbRelativeMoveTo*
bb_relative_move_to_new(int dx, int dy)
{
    return (BbRelativeMoveTo*) g_object_new(
        BB_TYPE_RELATIVE_MOVE_TO,
        "dx", dx,
        "dy", dy,
        NULL
        );
}


__attribute__((constructor)) void
bb_relative_move_to_register()
{
    bb_relative_move_to_get_type();
}


static void
bb_relative_move_to_render(BbPathCommand *command, BbItemRenderer *renderer)
{
    BbRelativeMoveTo *move_to = BB_RELATIVE_MOVE_TO(command);

    bb_item_renderer_render_relative_move_to(
        renderer,
        bb_relative_move_to_get_dx(move_to),
        bb_relative_move_to_get_dy(move_to)
        );
}


static void
bb_relative_move_to_rotate(BbPathCommand *command, int cx, int cy, int angle)
{
    BbRelativeMoveTo* instance = BB_RELATIVE_MOVE_TO(command);

    g_return_if_fail(instance != NULL);

    bb_coord_rotate(0, 0, angle, &instance->dx, &instance->dy);

    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_DX]);
    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_DY]);
}


static void
bb_relative_move_to_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_DX:
            bb_relative_move_to_set_dx(BB_RELATIVE_MOVE_TO(object), g_value_get_int(value));
            break;

        case PROP_DY:
            bb_relative_move_to_set_dy(BB_RELATIVE_MOVE_TO(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_relative_move_to_set_dx(BbRelativeMoveTo *command, int dx)
{
    g_return_if_fail(command != NULL);

    command->dx = dx;

    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_DX]);
}


void
bb_relative_move_to_set_dy(BbRelativeMoveTo *command, int dy)
{
    g_return_if_fail(command != NULL);

    command->dy = dy;

    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_DY]);
}


static void
bb_relative_move_to_translate(BbPathCommand *command, int dx, int dy)
{
    g_return_if_fail(command != NULL);
}

