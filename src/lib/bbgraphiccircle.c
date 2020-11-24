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
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbgraphiccircle.h"
#include "bbadjustablelinestyle.h"
#include "bbadjustablefillstyle.h"
#include "bbadjustableitemcolor.h"


enum
{
    PROP_0,

    PROP_CENTER_X,
    PROP_CENTER_Y,
    PROP_RADIUS,

    PROP_ITEM_COLOR,

    PROP_CAP_TYPE,
    PROP_DASH_LENGTH,
    PROP_DASH_SPACE,
    PROP_DASH_TYPE,
    PROP_LINE_WIDTH,

    PROP_FILL_TYPE,
    PROP_FILL_WIDTH,
    PROP_ANGLE_1,
    PROP_ANGLE_2,
    PROP_PITCH_1,
    PROP_PITCH_2,

    N_PROPERTIES
};


struct _BbGraphicCircle
{
    BbSchematicItem parent;

    BbItemParams *params;

    int center_x;
    int center_y;

    int radius;

    int color;

    BbLineStyle *line_style;
    BbFillStyle *fill_style;
};


static void
bb_graphic_circle_adjustable_fill_style_init(BbAdjustableFillStyleInterface *iface);

static void
bb_graphic_circle_adjustable_item_color_init(BbAdjustableItemColorInterface *iface);

static void
bb_graphic_circle_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface);


G_DEFINE_TYPE_WITH_CODE(
    BbGraphicCircle,
    bb_graphic_circle,
    BB_TYPE_SCHEMATIC_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_FILL_STYLE, bb_graphic_circle_adjustable_fill_style_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_graphic_circle_adjustable_item_color_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_LINE_STYLE, bb_graphic_circle_adjustable_line_style_init)
    )


static BbBounds*
bb_graphic_circle_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static void
bb_graphic_circle_dispose(GObject *object);

static void
bb_graphic_circle_finalize(GObject *object);

static int
bb_graphic_circle_get_cap_type(BbGraphicCircle *circle);

static int
bb_graphic_circle_get_dash_length(BbGraphicCircle *circle);

static int
bb_graphic_circle_get_dash_space(BbGraphicCircle *circle);

static int
bb_graphic_circle_get_dash_type(BbGraphicCircle *circle);

static int
bb_graphic_circle_get_item_color(BbGraphicCircle *circle);

static int
bb_graphic_circle_get_line_width(BbGraphicCircle *circle);

static void
bb_graphic_circle_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphic_circle_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_graphic_circle_set_cap_type(BbGraphicCircle *circle, int type);

static void
bb_graphic_circle_set_dash_length(BbGraphicCircle *circle, int length);

static void
bb_graphic_circle_set_dash_space(BbGraphicCircle *circle, int space);

static void
bb_graphic_circle_set_dash_type(BbGraphicCircle *circle, int type);

static void
bb_graphic_circle_set_item_color(BbGraphicCircle *circle, int color);

static void
bb_graphic_circle_set_line_width(BbGraphicCircle *line, int width);

static void
bb_graphic_circle_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_graphic_circle_translate(BbSchematicItem *item, int dx, int dy);

static void
bb_graphic_circle_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_graphic_circle_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];


static void
bb_graphic_circle_adjustable_fill_style_init(BbAdjustableFillStyleInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static void
bb_graphic_circle_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static void
bb_graphic_circle_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


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
        circle->line_style->line_width
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
    BB_SCHEMATIC_ITEM_CLASS(klasse)->translate = bb_graphic_circle_translate;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_async = bb_graphic_circle_write_async;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_finish = bb_graphic_circle_write_finish;

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

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CENTER_X,
        g_param_spec_int(
            "center-x",
            "Center X",
            "The x coordinate of the center",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CENTER_Y,
        g_param_spec_int(
            "center-y",
            "Center Y",
            "The y coordinate of the center",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_RADIUS,
        g_param_spec_int(
            "radius",
            "Radius",
            "The radius of the arc",
            0,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    for (int index = PROP_0 + 1; index < N_PROPERTIES; ++index)
    {
        properties[index] = g_object_class_find_property(
            G_OBJECT_CLASS(klasse),
            index
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
    BbGraphicCircle *circle = BB_GRAPHIC_CIRCLE(object);

    g_return_if_fail(circle != NULL);

    bb_fill_style_free(circle->fill_style);
    bb_line_style_free(circle->line_style);
}


static int
bb_graphic_circle_get_cap_type(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->line_style != NULL, 0);

    return circle->line_style->cap_type;
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


static int
bb_graphic_circle_get_dash_length(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->line_style != NULL, 0);

    return circle->line_style->dash_length;
}


static int
bb_graphic_circle_get_dash_space(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->line_style != NULL, 0);

    return circle->line_style->dash_space;
}


static int
bb_graphic_circle_get_dash_type(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->line_style != NULL, 0);

    return circle->line_style->dash_type;
}


static int
bb_graphic_circle_get_item_color(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);

    return circle->color;
}


static int
bb_graphic_circle_get_line_width(BbGraphicCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->line_style != NULL, 0);

    return circle->line_style->line_width;
}


static void
bb_graphic_circle_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAP_TYPE:
            g_value_set_int(value, bb_graphic_circle_get_cap_type(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_CENTER_X:
            g_value_set_int(value, bb_graphic_circle_get_center_x(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_CENTER_Y:
            g_value_set_int(value, bb_graphic_circle_get_center_y(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_graphic_circle_get_item_color(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_DASH_LENGTH:
            g_value_set_int(value, bb_graphic_circle_get_dash_length(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_DASH_SPACE:
            g_value_set_int(value, bb_graphic_circle_get_dash_space(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_DASH_TYPE:
            g_value_set_int(value, bb_graphic_circle_get_dash_type(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_LINE_WIDTH:
            g_value_set_int(value, bb_graphic_circle_get_line_width(BB_GRAPHIC_CIRCLE(object)));
            break;

        case PROP_RADIUS:
            g_value_set_int(value, bb_graphic_circle_get_radius(BB_GRAPHIC_CIRCLE(object)));
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


static void
bb_graphic_circle_init(BbGraphicCircle *circle)
{
    g_return_if_fail(circle != NULL);

    circle->fill_style = bb_fill_style_new();
    circle->line_style = bb_line_style_new();
}


BbGraphicCircle*
bb_graphic_circle_new()
{
    return g_object_new(BB_TYPE_GRAPHIC_CIRCLE, NULL);
}


__attribute__((constructor)) void
bb_graphic_circle_register()
{
    bb_graphic_circle_get_type();
}


static void
bb_graphic_circle_render(BbSchematicItem *item, BbItemRenderer *renderer)
{
    BbGraphicCircle *circle = BB_GRAPHIC_CIRCLE(item);

    g_return_if_fail(circle != NULL);

    bb_item_renderer_set_color(renderer, circle->color);
    bb_item_renderer_set_fill_style(renderer, circle->fill_style);
    bb_item_renderer_set_line_style(renderer, circle->line_style);

    bb_item_renderer_render_graphic_circle(renderer, circle);
}


static void
bb_graphic_circle_set_cap_type(BbGraphicCircle *circle, int type)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);

    if (circle->line_style->cap_type != type)
    {
        circle->line_style->cap_type = type;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CAP_TYPE]);
    }
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
bb_graphic_circle_set_dash_length(BbGraphicCircle *circle, int length)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);

    if (circle->line_style->dash_length != length)
    {
        circle->line_style->dash_length = length;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_DASH_LENGTH]);
    }
}


static void
bb_graphic_circle_set_dash_space(BbGraphicCircle *circle, int space)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);

    if (circle->line_style->dash_space != space)
    {
        circle->line_style->dash_space = space;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_DASH_SPACE]);
    }
}


static void
bb_graphic_circle_set_dash_type(BbGraphicCircle *circle, int type)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);

    if (circle->line_style->dash_type != type)
    {
        circle->line_style->dash_type = type;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_DASH_TYPE]);
    }
}


static void
bb_graphic_circle_set_item_color(BbGraphicCircle *circle, int color)
{
    g_return_if_fail(circle != NULL);

    if (circle->color != color)
    {
        circle->color = color;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_graphic_circle_set_line_width(BbGraphicCircle *circle, int width)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);

    if (circle->line_style->line_width != width)
    {
        circle->line_style->line_width = width;

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_LINE_WIDTH]);
    }
}


static void
bb_graphic_circle_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAP_TYPE:
            bb_graphic_circle_set_cap_type(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_CENTER_X:
            bb_graphic_circle_set_center_x(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_CENTER_Y:
            bb_graphic_circle_set_center_y(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_graphic_circle_set_item_color(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_DASH_LENGTH:
            bb_graphic_circle_set_dash_length(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_DASH_SPACE:
            bb_graphic_circle_set_dash_space(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_DASH_TYPE:
            bb_graphic_circle_set_dash_type(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_LINE_WIDTH:
            bb_graphic_circle_set_line_width(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
            break;
            
        case PROP_RADIUS:
            bb_graphic_circle_set_radius(BB_GRAPHIC_CIRCLE(object), g_value_get_int(value));
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


static void
bb_graphic_circle_translate(BbSchematicItem *item, int dx, int dy)
{
    BbGraphicCircle *circle = BB_GRAPHIC_CIRCLE(item);
    g_return_if_fail(circle != NULL);

    bb_coord_translate(dx, dy, &circle->center_x, &circle->center_y, 1);

    g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CENTER_X]);
    g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CENTER_Y]);
}


static void
bb_graphic_circle_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGraphicCircle *circle = BB_GRAPHIC_CIRCLE(item);

    bb_item_params_write_async(
        circle->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_graphic_circle_write_finish(
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
