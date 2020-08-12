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
#include "bbpathcommand.h"
#include "bbabsolutelineto.h"
#include "bbcoord.h"


enum
{
    PROP_0,
    PROP_X,
    PROP_Y,
    N_PROPERTIES
};


struct _BbAbsoluteLineTo
{
    BbPathCommand parent_instance;

    int x;
    int y;
};


G_DEFINE_TYPE(BbAbsoluteLineTo, bb_absolute_line_to, BB_TYPE_PATH_COMMAND)


static BbPathCommand*
bb_absolute_line_to_clone(BbPathCommand *command);

static void
bb_absolute_line_to_dispose(GObject *object);

static void
bb_absolute_line_to_finalize(GObject *object);

static void
bb_absolute_line_to_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_absolute_line_to_mirror_x(BbPathCommand *command, int cx);

static void
bb_absolute_line_to_mirror_y(BbPathCommand *command, int cy);

static void
bb_absolute_line_to_render(BbPathCommand *command, BbItemRenderer *renderer);

static void
bb_absolute_line_to_rotate(BbPathCommand *command, int cx, int cy, int angle);

static void
bb_absolute_line_to_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_absolute_line_to_translate(BbPathCommand *command, int dx, int dy);


static GParamSpec *properties[N_PROPERTIES];


static void
bb_absolute_line_to_class_init(BbAbsoluteLineToClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_absolute_line_to_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_absolute_line_to_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_absolute_line_to_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_absolute_line_to_set_property;

    BB_PATH_COMMAND_CLASS(klasse)->clone = bb_absolute_line_to_clone;
    BB_PATH_COMMAND_CLASS(klasse)->mirror_x = bb_absolute_line_to_mirror_x;
    BB_PATH_COMMAND_CLASS(klasse)->mirror_y = bb_absolute_line_to_mirror_y;
    BB_PATH_COMMAND_CLASS(klasse)->render = bb_absolute_line_to_render;
    BB_PATH_COMMAND_CLASS(klasse)->rotate = bb_absolute_line_to_rotate;
    BB_PATH_COMMAND_CLASS(klasse)->translate = bb_absolute_line_to_translate;

    properties[PROP_X] = g_param_spec_int(
        "x",
        "x",
        "",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_Y] = g_param_spec_int(
        "y",
        "y",
        "",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    for (int index = PROP_0 + 1; index < N_PROPERTIES; ++index)
    {
        g_object_class_install_property(
            G_OBJECT_CLASS(klasse),
            index,
            properties[index]
            );
    }

}


static BbPathCommand*
bb_absolute_line_to_clone(BbPathCommand *command)
{
    return BB_PATH_COMMAND(bb_absolute_line_to_new(
        bb_absolute_line_to_get_x(BB_ABSOLUTE_LINE_TO(command)),
        bb_absolute_line_to_get_y(BB_ABSOLUTE_LINE_TO(command))
        ));
}


static void
bb_absolute_line_to_dispose(GObject *object)
{
}


static void
bb_absolute_line_to_finalize(GObject *object)
{
}


static void
bb_absolute_line_to_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_X:
            g_value_set_int(value, bb_absolute_line_to_get_x(BB_ABSOLUTE_LINE_TO(object)));
            break;

        case PROP_Y:
            g_value_set_int(value, bb_absolute_line_to_get_y(BB_ABSOLUTE_LINE_TO(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_absolute_line_to_get_x(const BbAbsoluteLineTo *command)
{
    g_return_val_if_fail(command != NULL, 0);

    return command->x;
}


int
bb_absolute_line_to_get_y(const BbAbsoluteLineTo *command)
{
    g_return_val_if_fail(command != NULL, 0);

    return command->y;
}


static void
bb_absolute_line_to_init(BbAbsoluteLineTo *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


static void
bb_absolute_line_to_mirror_x(BbPathCommand *command, int cx)
{
    bb_absolute_line_to_set_x(
        BB_ABSOLUTE_LINE_TO(command),
        2 * cx - bb_absolute_line_to_get_x(BB_ABSOLUTE_LINE_TO(command))
        );
}


static void
bb_absolute_line_to_mirror_y(BbPathCommand *command, int cy)
{
    bb_absolute_line_to_set_y(
        BB_ABSOLUTE_LINE_TO(command),
        2 * cy - bb_absolute_line_to_get_y(BB_ABSOLUTE_LINE_TO(command))
        );
}


BbAbsoluteLineTo*
bb_absolute_line_to_new(int x, int y)
{
    return (BbAbsoluteLineTo*) g_object_new(
        BB_TYPE_ABSOLUTE_LINE_TO,
        "x", x,
        "y", y,
        NULL
        );
}


__attribute__((constructor)) void
bb_absolute_line_to_register()
{
    bb_absolute_line_to_get_type();
}


static void
bb_absolute_line_to_render(BbPathCommand *command, BbItemRenderer *renderer)
{
    bb_item_renderer_render_absolute_line_to(
        renderer,
        bb_absolute_line_to_get_x(BB_ABSOLUTE_LINE_TO(command)),
        bb_absolute_line_to_get_y(BB_ABSOLUTE_LINE_TO(command))
        );
}


static void
bb_absolute_line_to_rotate(BbPathCommand *command, int cx, int cy, int angle)
{
    BbAbsoluteLineTo* instance = BB_ABSOLUTE_LINE_TO(command);

    g_return_if_fail(instance != NULL);

    bb_coord_rotate(cx, cy, angle, &instance->x, &instance->y);

    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_X]);
    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_Y]);
}


static void
bb_absolute_line_to_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_X:
            bb_absolute_line_to_set_x(BB_ABSOLUTE_LINE_TO(object), g_value_get_int(value));
            break;

        case PROP_Y:
            bb_absolute_line_to_set_y(BB_ABSOLUTE_LINE_TO(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_absolute_line_to_set_x(BbAbsoluteLineTo *command, int x)
{
    g_return_if_fail(command != NULL);

    command->x = x;

    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_X]);
}


void
bb_absolute_line_to_set_y(BbAbsoluteLineTo *command, int y)
{
    g_return_if_fail(command != NULL);

    command->y = y;

    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_Y]);
}


static void
bb_absolute_line_to_translate(BbPathCommand *command, int dx, int dy)
{
    BbAbsoluteLineTo* instance = BB_ABSOLUTE_LINE_TO(command);

    g_return_if_fail(instance != NULL);

    bb_coord_translate(dx, dy, &instance->x, &instance->y, 1);

    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_X]);
    g_object_notify_by_pspec(G_OBJECT(command), properties[PROP_Y]);
}
