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
#include "bbgraphiccircle.h"


enum
{
    PROP_0,
    PROP_CENTER_X,
    PROP_CENTER_Y,
    PROP_RADIUS,
    PROP_WIDTH,
    N_PROPERTIES
};


struct _BbGraphicCircle
{
    BbSchematicItem parent;

    int center_x;
    int center_y;

    int radius;

    int width;
};


G_DEFINE_TYPE(BbGraphicCircle, bb_graphic_circle, BB_TYPE_SCHEMATIC_ITEM)


static BbBounds*
bb_graphic_circle_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static void
bb_graphic_circle_dispose(GObject *object);

static void
bb_graphic_circle_finalize(GObject *object);

static void
bb_graphic_circle_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphic_circle_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_graphic_circle_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


static BbBounds*
bb_graphic_circle_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator)
{
    BbGraphicCircle *circle = BB_GRAPHIC_CIRCLE(item);

    g_return_val_if_fail(circle != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        circle->center_x - circle->radius,
        circle->center_y - circle->radius,
        circle->center_x + circle->radius,
        circle->center_y + circle->radius,
        circle->width
        );
}


static void
bb_graphic_circle_class_init(BbGraphicCircleClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_graphic_circle_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_graphic_circle_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_graphic_circle_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_graphic_circle_set_property;

    BB_SCHEMATIC_ITEM_CLASS(klasse)->calculate_bounds = bb_graphic_circle_calculate_bounds;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->render = bb_graphic_circle_render;

    properties[PROP_CENTER_X] = g_param_spec_int(
        "center-x",
        "Center X",
        "The x coordinate of the center",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_CENTER_Y] = g_param_spec_int(
        "center-y",
        "Center Y",
        "The y coordinate of the center",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_RADIUS] = g_param_spec_int(
        "radius",
        "Radius",
        "The radius of the arc",
        0,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_WIDTH] = g_param_spec_int(
        "width",
        "Line Width",
        "The line width",
        0,
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


static void
bb_graphic_circle_dispose(GObject *object)
{
}


static void
bb_graphic_circle_finalize(GObject *object)
{
}


int
bb_graphic_circle_get_center_x(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);

    return circle->center_x;
}


int
bb_graphic_circle_get_center_y(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);

    return circle->center_y;
}


static void
bb_graphic_circle_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CENTER_X:
            g_value_set_int(value, bb_graphic_circle_get_center_x(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_CENTER_Y:
            g_value_set_int(value, bb_graphic_circle_get_center_y(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_RADIUS:
            g_value_set_int(value, bb_graphic_circle_get_radius(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_WIDTH:
            g_value_set_int(value, bb_graphic_circle_get_width(BB_GRAPHIC_CIRCLE(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_graphic_circle_get_radius(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);

    return circle->radius;
}


int
bb_graphic_circle_get_width(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);

    return circle->width;
}


static void
bb_graphic_circle_init(BbGraphicCircle *window)
{
}


__attribute__((constructor)) void
bb_graphic_circle_register()
{
    bb_graphic_circle_get_type();
}


static void
bb_graphic_circle_render(BbSchematicItem *item, BbItemRenderer *renderer)
{
    bb_item_renderer_render_graphic_circle(renderer, BB_GRAPHIC_CIRCLE(item));
}


void
bb_graphic_circle_set_center_x(BbGraphicCircle *circle, int x)
{
    g_return_if_fail(circle != NULL);

    if (circle->center_x != x)
    {
        circle->center_x = x;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CENTER_X]);
    }
}


void
bb_graphic_circle_set_center_y(BbGraphicCircle *circle, int y)
{
    g_return_if_fail(circle != NULL);

    if (circle->center_y != y)
    {
        circle->center_y = y;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CENTER_Y]);
    }
}


static void
bb_graphic_circle_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CENTER_X:
            bb_graphic_circle_set_center_x(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_CENTER_Y:
            bb_graphic_circle_set_center_y(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_RADIUS:
            bb_graphic_circle_set_radius(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_WIDTH:
            bb_graphic_circle_set_width(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_graphic_circle_set_radius(BbGraphicCircle *circle, int radius)
{
    g_return_if_fail(circle != NULL);

    if (circle->radius != radius)
    {
        circle->radius = radius;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_RADIUS]);
    }
}


void
bb_graphic_circle_set_width(BbGraphicCircle *circle, int width)
{
    g_return_if_fail(circle != NULL);

    if (circle->width != width)
    {
        circle->width = width;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_WIDTH]);
    }
}
