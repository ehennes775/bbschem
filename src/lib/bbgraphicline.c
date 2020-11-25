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
#include <bbextensions.h>
#include "bbgraphicline.h"
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbadjustablelinestyle.h"
#include "bbadjustableitemcolor.h"


enum
{
    PROP_0,

    PROP_X0,
    PROP_X1,
    PROP_Y0,
    PROP_Y1,

    PROP_ITEM_COLOR,

    PROP_CAP_TYPE,
    PROP_DASH_LENGTH,
    PROP_DASH_SPACE,
    PROP_DASH_TYPE,
    PROP_LINE_WIDTH,

    N_PROPERTIES
};


struct _BbGraphicLine
{
    BbSchematicItem parent;

    BbItemParams *params;

    int x[2];
    int y[2];

    int color;

    BbLineStyle *line_style;
};


static void
bb_graphical_line_adjustable_item_color_init(BbAdjustableLineStyleInterface *iface);

static void
bb_graphical_line_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface);

static BbBounds*
bb_graphic_line_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static void
bb_graphic_line_dispose(GObject *object);

static void
bb_graphic_line_finalize(GObject *object);

static int
bb_graphic_line_get_cap_type(BbGraphicLine *line);

static int
bb_graphic_line_get_dash_length(BbGraphicLine *line);

static int
bb_graphic_line_get_dash_space(BbGraphicLine *line);

static int
bb_graphic_line_get_dash_type(BbGraphicLine *line);

static int
bb_graphic_line_get_item_color(BbGraphicLine *line);

static int
bb_graphic_line_get_line_width(BbGraphicLine *line);

static void
bb_graphic_line_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphic_line_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_graphic_line_set_cap_type(BbGraphicLine *line, int type);

static void
bb_graphic_line_set_dash_length(BbGraphicLine *line, int length);

static void
bb_graphic_line_set_dash_space(BbGraphicLine *line, int space);

static void
bb_graphic_line_set_dash_type(BbGraphicLine *line, int type);

static void
bb_graphic_line_set_item_color(BbGraphicLine *line, int color);

static void
bb_graphic_line_set_line_width(BbGraphicLine *line, int width);

static void
bb_graphic_line_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_graphic_line_translate(BbSchematicItem *item, int dx, int dy);

static void
bb_graphic_line_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_graphic_line_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbGraphicLine,
    bb_graphic_line,
    BB_TYPE_SCHEMATIC_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_graphical_line_adjustable_item_color_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_LINE_STYLE, bb_graphical_line_adjustable_line_style_init)
    )


static void
bb_graphical_line_adjustable_item_color_init(BbAdjustableLineStyleInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static void
bb_graphical_line_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


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
        line->line_style->line_width
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
    BB_SCHEMATIC_ITEM_CLASS(klasse)->translate = bb_graphic_line_translate;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_async = bb_graphic_line_write_async;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_finish = bb_graphic_line_write_finish;

    properties[PROP_CAP_TYPE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAP_TYPE,
        "cap-type"
        );

    properties[PROP_DASH_LENGTH] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_DASH_LENGTH,
        "dash-length"
        );

    properties[PROP_DASH_SPACE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_DASH_SPACE,
        "dash-space"
        );

    properties[PROP_DASH_TYPE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_DASH_TYPE,
        "dash-type"
        );

    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_ITEM_COLOR,
        "item-color"
        );

    properties[PROP_LINE_WIDTH] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_LINE_WIDTH,
        "line-width"
        );

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_X0,
        g_param_spec_int(
            "x0",
            "x0",
            "The x coordinate of the first endpoint",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_X1,
        g_param_spec_int(
            "x1",
            "x1",
            "The x coordinate of the second endpoint",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_Y0,
        g_param_spec_int(
            "y0",
            "y0",
            "The y coordinate of the first endpoint",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_Y1,
        g_param_spec_int(
            "y1",
            "y1",
            "The y coordinate of the second endpoint",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );
}


static void
bb_graphic_line_dispose(GObject *object)
{
}


static void
bb_graphic_line_finalize(GObject *object)
{
    BbGraphicLine *line = BB_GRAPHIC_LINE(object);

    g_return_if_fail(line != NULL);

    bb_line_style_free(line->line_style);
}


static int
bb_graphic_line_get_cap_type(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->cap_type;
}


static int
bb_graphic_line_get_dash_length(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->dash_length;
}


static int
bb_graphic_line_get_dash_space(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->dash_space;
}


static int
bb_graphic_line_get_dash_type(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->dash_type;
}


static int
bb_graphic_line_get_item_color(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->color;
}


static int
bb_graphic_line_get_line_width(BbGraphicLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->line_width;
}


static void
bb_graphic_line_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAP_TYPE:
            g_value_set_int(value, bb_graphic_line_get_cap_type(BB_GRAPHIC_LINE(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_graphic_line_get_item_color(BB_GRAPHIC_LINE(object)));
            break;

        case PROP_DASH_LENGTH:
            g_value_set_int(value, bb_graphic_line_get_dash_length(BB_GRAPHIC_LINE(object)));
            break;

        case PROP_DASH_SPACE:
            g_value_set_int(value, bb_graphic_line_get_dash_space(BB_GRAPHIC_LINE(object)));
            break;

        case PROP_DASH_TYPE:
            g_value_set_int(value, bb_graphic_line_get_dash_type(BB_GRAPHIC_LINE(object)));
            break;

        case PROP_LINE_WIDTH:
            g_value_set_int(value, bb_graphic_line_get_line_width(BB_GRAPHIC_LINE(object)));
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
bb_graphic_line_init(BbGraphicLine *line)
{
    g_return_if_fail(line != NULL);

    line->line_style = bb_line_style_new();
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
    BbGraphicLine *line = BB_GRAPHIC_LINE(item);

    g_return_if_fail(line != NULL);

    bb_item_renderer_set_color(renderer, line->color);
    bb_item_renderer_set_line_style(renderer, line->line_style);

    bb_item_renderer_render_absolute_move_to(
        renderer,
        line->x[0],
        line->y[0]
        );

    bb_item_renderer_render_absolute_line_to(
        renderer,
        line->x[1],
        line->y[1]
        );
}


static void
bb_graphic_line_set_cap_type(BbGraphicLine *line, int type)
{
    g_return_if_fail(line != NULL);
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->cap_type != type)
    {
        line->line_style->cap_type = type;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_CAP_TYPE]);
    }
}


static void
bb_graphic_line_set_dash_length(BbGraphicLine *line, int length)
{
    g_return_if_fail(line != NULL);
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->dash_length != length)
    {
        line->line_style->dash_length = length;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_DASH_LENGTH]);
    }
}


static void
bb_graphic_line_set_dash_space(BbGraphicLine *line, int space)
{
    g_return_if_fail(line != NULL);
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->dash_space != space)
    {
        line->line_style->dash_space = space;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_DASH_SPACE]);
    }
}


static void
bb_graphic_line_set_dash_type(BbGraphicLine *line, int type)
{
    g_return_if_fail(line != NULL);
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->dash_type != type)
    {
        line->line_style->dash_type = type;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_DASH_TYPE]);
    }
}


static void
bb_graphic_line_set_item_color(BbGraphicLine *line, int color)
{
    g_return_if_fail(line != NULL);

    if (line->color != color)
    {
        line->color = color;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_graphic_line_set_line_width(BbGraphicLine *line, int width)
{
    g_return_if_fail(line != NULL);
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->line_width != width)
    {
        line->line_style->line_width = width;

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_LINE_WIDTH]);
    }
}


static void
bb_graphic_line_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAP_TYPE:
            bb_graphic_line_set_cap_type(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_graphic_line_set_item_color(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        case PROP_DASH_LENGTH:
            bb_graphic_line_set_dash_length(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        case PROP_DASH_SPACE:
            bb_graphic_line_set_dash_space(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        case PROP_DASH_TYPE:
            bb_graphic_line_set_dash_type(BB_GRAPHIC_LINE(object), g_value_get_int(value));
            break;

        case PROP_LINE_WIDTH:
            bb_graphic_line_set_line_width(BB_GRAPHIC_LINE(object), g_value_get_int(value));
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


static void
bb_graphic_line_translate(BbSchematicItem *item, int dx, int dy)
{
    BbGraphicLine *line = BB_GRAPHIC_LINE(item);
    g_return_if_fail(line != NULL);

    bb_coord_translate(dx, dy, line->x, line->y, 2);

    g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_X0]);
    g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_Y0]);
    g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_X1]);
    g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_Y1]);
}


static void
bb_graphic_line_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGraphicLine *line = BB_GRAPHIC_LINE(item);

    bb_item_params_write_async(
        line->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_graphic_line_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    )
{
    return g_output_stream_write_all_finish(
        stream,
        result,
        NULL,
        error
        );
}
