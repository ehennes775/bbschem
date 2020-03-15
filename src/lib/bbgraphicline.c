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
#include "bbgraphicline.h"


enum
{
    PROP_0,
    PROP_WIDTH,
    PROP_X0,
    PROP_X1,
    PROP_Y0,
    PROP_Y1,
    N_PROPERTIES
};


struct _BbGraphicLine
{
    BbSchematicItem parent;

    int width;

    int x[2];
    int y[2];
};


G_DEFINE_TYPE(BbGraphicLine, bb_graphic_line, BB_TYPE_SCHEMATIC_ITEM)


static BbBounds*
bb_graphic_line_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static void
bb_graphic_line_dispose(GObject *object);

static void
bb_graphic_line_finalize(GObject *object);

static void
bb_graphic_line_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphic_line_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_graphic_line_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


static BbBounds*
bb_graphic_line_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator)
{
    BbGraphicLine *line = BB_GRAPHIC_LINE(item);

    g_return_val_if_fail(line != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        line->x[0],
        line->y[0],
        line->x[1],
        line->y[1],
        line->width
        );
}


static void
bb_graphic_line_class_init(BbGraphicLineClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_graphic_line_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_graphic_line_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_graphic_line_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_graphic_line_set_property;

    BB_SCHEMATIC_ITEM_CLASS(klasse)->calculate_bounds = bb_graphic_line_calculate_bounds;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->render = bb_graphic_line_render;

    properties[PROP_WIDTH] = g_param_spec_int(
        "width",
        "Line Width",
        "The line width",
        0,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_X0] = g_param_spec_int(
        "x0",
        "x0",
        "The x coordinate of the first endpoint",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_X1] = g_param_spec_int(
        "x1",
        "x1",
        "The x coordinate of the second endpoint",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_Y0] = g_param_spec_int(
        "y0",
        "y0",
        "The y coordinate of the first endpoint",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_Y1] = g_param_spec_int(
        "y1",
        "y1",
        "The y coordinate of the second endpoint",
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


static void
bb_graphic_line_dispose(GObject *object)
{
}


static void
bb_graphic_line_finalize(GObject *object)
{
}


static void
bb_graphic_line_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WIDTH:
            g_value_set_int(value, bb_graphic_line_get_width(BB_GRAPHIC_LINE(object)));
            break;

        case PROP_X0:
            g_value_set_int(value, bb_graphic_line_get_x0(BB_GRAPHIC_LINE(object)));
            break;

        case PROP_X1:
            g_value_set_int(value, bb_graphic_line_get_x1(BB_GRAPHIC_LINE(object)));
            break;

        case PROP_Y0:
            g_value_set_int(value, bb_graphic_line_get_y0(BB_GRAPHIC_LINE(object)));
            break;

        case PROP_Y1:
            g_value_set_int(value, bb_graphic_line_get_y1(BB_GRAPHIC_LINE(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_graphic_line_get_width(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->width;
}


int
bb_graphic_line_get_x0(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->x[0];
}


int
bb_graphic_line_get_x1(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->x[1];
}


int
bb_graphic_line_get_y0(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->y[0];
}


int
bb_graphic_line_get_y1(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->y[1];
}


static void
bb_graphic_line_init(BbGraphicLine *window)
{
}


BbGraphicLine*
bb_graphic_line_new()
{
    return g_object_new(BB_TYPE_GRAPHIC_LINE, NULL);
}


__attribute__((constructor)) void
bb_graphic_line_register()
{
    bb_graphic_line_get_type();
}


static void
bb_graphic_line_render(BbSchematicItem *item, BbItemRenderer *renderer)
{
    bb_item_renderer_render_graphic_line(renderer, BB_GRAPHIC_LINE(item));
}


static void
bb_graphic_line_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WIDTH:
            bb_graphic_line_set_width(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        case PROP_X0:
            bb_graphic_line_set_x0(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        case PROP_X1:
            bb_graphic_line_set_x1(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        case PROP_Y0:
            bb_graphic_line_set_y0(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        case PROP_Y1:
            bb_graphic_line_set_y1(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_graphic_line_set_width(BbGraphicLine *line, int width)
{
    g_return_if_fail(line != NULL);

    if (line->width != width)
    {
        line->width = width;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_WIDTH]);
    }
}


void
bb_graphic_line_set_x0(BbGraphicLine *line, int x)
{
    g_return_if_fail(line != NULL);

    if (line->x[0] != x)
    {
        line->x[0] = x;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_X0]);
    }
}


void
bb_graphic_line_set_x1(BbGraphicLine *line, int x)
{
    g_return_if_fail(line != NULL);

    if (line->x[1] != x)
    {
        line->x[1] = x;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_X1]);
    }
}


void
bb_graphic_line_set_y0(BbGraphicLine *line, int y)
{
    g_return_if_fail(line != NULL);

    if (line->y[0] != y)
    {
        line->y[0] = y;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_Y0]);
    }
}


void
bb_graphic_line_set_y1(BbGraphicLine *line, int y)
{
    g_return_if_fail(line != NULL);

    if (line->y[1] != y)
    {
        line->y[1] = y;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_Y1]);
    };
}
