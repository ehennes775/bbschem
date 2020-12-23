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
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbgedacircle.h"
#include "bbadjustablelinestyle.h"
#include "bbadjustablefillstyle.h"
#include "bbadjustableitemcolor.h"
#include "bbparams.h"
#include "bbcolor.h"
#include "bbcolors.h"


/**
 * The positions of parameters in the file format
 */
enum
{
    PARAM_TOKEN,
    PARAM_CENTER_X,
    PARAM_CENTER_Y,
    PARAM_RADIUS,
    PARAM_COLOR,
    PARAM_LINE_WIDTH,
    PARAM_CAP_TYPE,
    PARAM_DASH_TYPE,
    PARAM_DASH_LENGTH,
    PARAM_DASH_SPACE,
    PARAM_FILL_TYPE,
    PARAM_FILL_WIDTH,
    PARAM_FILL_ANGLE_1,
    PARAM_FILL_PITCH_1,
    PARAM_FILL_ANGLE_2,
    PARAM_FILL_PITCH_2,
    N_PARAMETERS
};


enum
{
    PROP_0,

    PROP_CENTER_X,
    PROP_CENTER_Y,
    PROP_RADIUS,

    /* From AdjustableLineStyle */
    PROP_CAP_TYPE,
    PROP_DASH_LENGTH,
    PROP_DASH_SPACE,
    PROP_DASH_TYPE,
    PROP_LINE_WIDTH,

    /* From AdjustableItemColor */
    PROP_ITEM_COLOR,

    /* From AdjustableFillStyle */
    PROP_FILL_TYPE,
    PROP_FILL_WIDTH,
    PROP_ANGLE_1,
    PROP_ANGLE_2,
    PROP_PITCH_1,
    PROP_PITCH_2,

    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbGedaCircle
{
    BbGedaItem parent;

    BbItemParams *params;

    int center_x;
    int center_y;

    int radius;

    int color;

    BbLineStyle *line_style;
    BbFillStyle *fill_style;
};


static void
bb_geda_circle_adjustable_fill_style_init(
    BbAdjustableFillStyleInterface *iface
    );

static void
bb_geda_circle_adjustable_item_color_init(
    BbAdjustableItemColorInterface *iface
    );

static void
bb_geda_circle_adjustable_line_style_init(
    BbAdjustableLineStyleInterface *iface
    );

static BbBounds*
bb_geda_circle_calculate_bounds(
    BbGedaItem *item,
    BbBoundsCalculator *calculator
    );

static BbGedaItem*
bb_geda_circle_clone(
    BbGedaItem *item
    );

static void
bb_geda_circle_dispose(
    GObject *object
    );

static void
bb_geda_circle_finalize(
    GObject *object
    );

static int
bb_geda_circle_get_angle_1(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_angle_2(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_cap_type(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_dash_length(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_dash_space(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_dash_type(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_fill_type(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_fill_width(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_item_color(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_line_width(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_pitch_1(
    BbGedaCircle *circle
    );

static int
bb_geda_circle_get_pitch_2(
    BbGedaCircle *circle
    );

static void
bb_geda_circle_get_property(
    GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_circle_render(
    BbGedaItem *item,
    BbItemRenderer *renderer
    );

static void
bb_geda_circle_set_cap_type(
    BbGedaCircle *circle,
    int type
    );

static void
bb_geda_circle_set_dash_length(
    BbGedaCircle *circle,
    int length
    );

static void
bb_geda_circle_set_dash_space(
    BbGedaCircle *circle,
    int space
    );

static void
bb_geda_circle_set_dash_type(
    BbGedaCircle *circle,
    int type
    );

static void
bb_geda_circle_set_item_color(
    BbGedaCircle *circle,
    int color
    );

static void
bb_geda_circle_set_line_width(
    BbGedaCircle *line,
    int width
    );

static void
bb_geda_circle_set_property(
    GObject *object,
    guint property_id,
    const GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_circle_translate(
    BbGedaItem *item,
    int dx,
    int dy
    );

static gboolean
bb_geda_circle_write(
    BbGedaItem *item,
    GOutputStream *stream,
    GCancellable *cancellable,
    GError **error
    );

static void
bb_geda_circle_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_circle_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbGedaCircle,
    bb_geda_circle,
    BB_TYPE_GEDA_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_FILL_STYLE, bb_geda_circle_adjustable_fill_style_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_geda_circle_adjustable_item_color_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_LINE_STYLE, bb_geda_circle_adjustable_line_style_init)
    )


static void
bb_geda_circle_adjustable_fill_style_init(BbAdjustableFillStyleInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static void
bb_geda_circle_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static void
bb_geda_circle_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static BbBounds*
bb_geda_circle_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaCircle *circle = BB_GEDA_CIRCLE(item);

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
bb_geda_circle_class_init(BbGedaCircleClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_geda_circle_dispose;
    object_class->finalize = bb_geda_circle_finalize;
    object_class->get_property = bb_geda_circle_get_property;
    object_class->set_property = bb_geda_circle_set_property;

    BB_GEDA_ITEM_CLASS(klasse)->calculate_bounds = bb_geda_circle_calculate_bounds;
    BB_GEDA_ITEM_CLASS(klasse)->clone = bb_geda_circle_clone;
    BB_GEDA_ITEM_CLASS(klasse)->render = bb_geda_circle_render;
    BB_GEDA_ITEM_CLASS(klasse)->translate = bb_geda_circle_translate;
    BB_GEDA_ITEM_CLASS(klasse)->write = bb_geda_circle_write;
    BB_GEDA_ITEM_CLASS(klasse)->write_async = bb_geda_circle_write_async;
    BB_GEDA_ITEM_CLASS(klasse)->write_finish = bb_geda_circle_write_finish;

    /* Properties from BbAdjustableFillStyle */

    properties[PROP_ANGLE_1] = bb_object_class_override_property(
        object_class,
        PROP_ANGLE_1,
        "angle-1"
        );

    properties[PROP_ANGLE_2] = bb_object_class_override_property(
        object_class,
        PROP_ANGLE_2,
        "angle-2"
        );

    properties[PROP_PITCH_1] = bb_object_class_override_property(
        object_class,
        PROP_PITCH_1,
        "pitch-1"
        );

    properties[PROP_PITCH_2] = bb_object_class_override_property(
        object_class,
        PROP_PITCH_2,
        "pitch-2"
        );

    properties[PROP_FILL_TYPE] = bb_object_class_override_property(
        object_class,
        PROP_FILL_TYPE,
        "fill-type"
        );

    properties[PROP_FILL_WIDTH] = bb_object_class_override_property(
        object_class,
        PROP_FILL_WIDTH,
        "fill-width"
        );

    /* Properties from BbAdjustableItemColor */

    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        object_class,
        PROP_ITEM_COLOR,
        "item-color"
        );


    /* Properties from BbAdjustableLineStyle */

    properties[PROP_CAP_TYPE] = bb_object_class_override_property(
        object_class,
        PROP_CAP_TYPE,
        "cap-type"
        );

    properties[PROP_DASH_LENGTH] = bb_object_class_override_property(
        object_class,
        PROP_DASH_LENGTH,
        "dash-length"
        );

    properties[PROP_DASH_SPACE] = bb_object_class_override_property(
        object_class,
        PROP_DASH_SPACE,
        "dash-space"
        );

    properties[PROP_DASH_TYPE] = bb_object_class_override_property(
        object_class,
        PROP_DASH_TYPE,
        "dash-type"
        );

    properties[PROP_LINE_WIDTH] = bb_object_class_override_property(
        object_class,
        PROP_LINE_WIDTH,
        "line-width"
        );

    bb_object_class_install_property(
        object_class,
        PROP_CENTER_X,
        properties[PROP_CENTER_X] = g_param_spec_int(
            "center-x",
            "Center X",
            "The x coordinate of the center",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        object_class,
        PROP_CENTER_Y,
        properties[PROP_CENTER_Y] = g_param_spec_int(
            "center-y",
            "Center Y",
            "The y coordinate of the center",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        object_class,
        PROP_RADIUS,
        properties[PROP_RADIUS] = g_param_spec_int(
            "radius",
            "Radius",
            "The radius of the arc",
            0,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_GEDA_ITEM);
}


static BbGedaItem*
bb_geda_circle_clone(BbGedaItem *item)
{
    return BB_GEDA_ITEM(g_object_new(
        BB_TYPE_GEDA_CIRCLE,
        "center-x", bb_geda_circle_get_center_x(BB_GEDA_CIRCLE(item)),
        "center-y", bb_geda_circle_get_center_y(BB_GEDA_CIRCLE(item)),
        "radius", bb_geda_circle_get_radius(BB_GEDA_CIRCLE(item)),

        /* From AdjustableFillStyle */
        "angle-1", bb_geda_circle_get_angle_1(BB_GEDA_CIRCLE(item)),
        "angle-2", bb_geda_circle_get_angle_2(BB_GEDA_CIRCLE(item)),
        "fill-type", bb_geda_circle_get_fill_type(BB_GEDA_CIRCLE(item)),
        "fill-width", bb_geda_circle_get_fill_width(BB_GEDA_CIRCLE(item)),
        "pitch-1", bb_geda_circle_get_pitch_1(BB_GEDA_CIRCLE(item)),
        "pitch-2", bb_geda_circle_get_pitch_2(BB_GEDA_CIRCLE(item)),

        /* From AdjustableItemColor */
        "item-color", bb_geda_circle_get_item_color(BB_GEDA_CIRCLE(item)),

        /* From AdjustableLineStyle */
        "cap-type", bb_geda_circle_get_cap_type(BB_GEDA_CIRCLE(item)),
        "dash-length", bb_geda_circle_get_dash_length(BB_GEDA_CIRCLE(item)),
        "dash-space", bb_geda_circle_get_dash_space(BB_GEDA_CIRCLE(item)),
        "dash-type", bb_geda_circle_get_dash_type(BB_GEDA_CIRCLE(item)),
        "line-width", bb_geda_circle_get_line_width(BB_GEDA_CIRCLE(item)),

        NULL
        ));
}


static void
bb_geda_circle_dispose(GObject *object)
{
}


static void
bb_geda_circle_finalize(GObject *object)
{
    BbGedaCircle *circle = BB_GEDA_CIRCLE(object);

    g_return_if_fail(circle != NULL);

    bb_fill_style_free(circle->fill_style);
    bb_line_style_free(circle->line_style);
}


static int
bb_geda_circle_get_angle_1(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->fill_style != NULL, 0);

    return circle->fill_style->angle[0];
}


static int
bb_geda_circle_get_angle_2(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->fill_style != NULL, 0);

    return circle->fill_style->angle[1];
}


static int
bb_geda_circle_get_cap_type(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, BB_CAP_TYPE_DEFAULT);
    g_return_val_if_fail(circle->line_style != NULL, BB_CAP_TYPE_DEFAULT);

    return circle->line_style->cap_type;
}


int
bb_geda_circle_get_center_x(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);

    return circle->center_x;
}


int
bb_geda_circle_get_center_y(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);

    return circle->center_y;
}


static int
bb_geda_circle_get_dash_length(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->line_style != NULL, 0);

    return circle->line_style->dash_length;
}


static int
bb_geda_circle_get_dash_space(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->line_style != NULL, 0);

    return circle->line_style->dash_space;
}


static int
bb_geda_circle_get_dash_type(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, BB_DASH_TYPE_DEFAULT);
    g_return_val_if_fail(circle->line_style != NULL, BB_DASH_TYPE_DEFAULT);

    return circle->line_style->dash_type;
}


static int
bb_geda_circle_get_fill_type(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, BB_FILL_TYPE_DEFAULT);
    g_return_val_if_fail(circle->fill_style != NULL, BB_FILL_TYPE_DEFAULT);

    return circle->fill_style->type;
}


static int
bb_geda_circle_get_fill_width(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->fill_style != NULL, 0);

    return circle->fill_style->width;
}


static int
bb_geda_circle_get_item_color(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);

    return circle->color;
}


static int
bb_geda_circle_get_line_width(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->line_style != NULL, 0);

    return circle->line_style->line_width;
}


static int
bb_geda_circle_get_pitch_1(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->fill_style != NULL, 0);

    return circle->fill_style->pitch[0];
}


static int
bb_geda_circle_get_pitch_2(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);
    g_return_val_if_fail(circle->fill_style != NULL, 0);

    return circle->fill_style->pitch[1];
}


static void
bb_geda_circle_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ANGLE_1:
            g_value_set_int(value, bb_geda_circle_get_angle_1(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_ANGLE_2:
            g_value_set_int(value, bb_geda_circle_get_angle_2(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_CAP_TYPE:
            g_value_set_int(value, bb_geda_circle_get_cap_type(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_CENTER_X:
            g_value_set_int(value, bb_geda_circle_get_center_x(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_CENTER_Y:
            g_value_set_int(value, bb_geda_circle_get_center_y(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_geda_circle_get_item_color(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_DASH_LENGTH:
            g_value_set_int(value, bb_geda_circle_get_dash_length(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_DASH_SPACE:
            g_value_set_int(value, bb_geda_circle_get_dash_space(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_DASH_TYPE:
            g_value_set_int(value, bb_geda_circle_get_dash_type(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_LINE_WIDTH:
            g_value_set_int(value, bb_geda_circle_get_line_width(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_PITCH_1:
            g_value_set_int(value, bb_geda_circle_get_pitch_1(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_PITCH_2:
            g_value_set_int(value, bb_geda_circle_get_pitch_2(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_RADIUS:
            g_value_set_int(value, bb_geda_circle_get_radius(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_FILL_TYPE:
            g_value_set_int(value, bb_geda_circle_get_fill_type(BB_GEDA_CIRCLE(object)));
            break;

        case PROP_FILL_WIDTH:
            g_value_set_int(value, bb_geda_circle_get_fill_width(BB_GEDA_CIRCLE(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_geda_circle_get_radius(BbGedaCircle *circle)
{
    g_return_val_if_fail(circle != NULL, 0);

    return circle->radius;
}


static void
bb_geda_circle_init(BbGedaCircle *circle)
{
    g_return_if_fail(circle != NULL);

    circle->fill_style = bb_fill_style_new();
    circle->line_style = bb_line_style_new();
}


BbGedaCircle*
bb_geda_circle_new()
{
    return g_object_new(BB_TYPE_GEDA_CIRCLE, NULL);
}


BbGedaCircle*
bb_geda_circle_new_with_params(BbParams *params, GError **error)
{
    GError *local_error = NULL;

    g_return_val_if_fail(bb_params_token_matches(params, BB_GEDA_CIRCLE_TOKEN), NULL);

    int center_x;
    int center_y;
    BbGedaCircle *circle = NULL;
    int color;
    BbFillStyle fill_style;
    BbLineStyle line_style;
    int radius;

    center_x = bb_params_get_int(params, PARAM_CENTER_X, &local_error);

    if (local_error == NULL)
    {
        center_y = bb_params_get_int(params, PARAM_CENTER_Y, &local_error);
    }

    if (local_error == NULL)
    {
        radius = bb_params_get_int(params, PARAM_RADIUS, &local_error);
    }

    if (local_error == NULL)
    {
        color = bb_text_color_from_params(params, PARAM_COLOR, BB_COLOR_GRAPHIC, &local_error);
    }

    if (local_error == NULL)
    {
        bb_fill_style_from_params(params, PARAM_FILL_TYPE, &fill_style, &local_error);
    }

    if (local_error == NULL)
    {
        bb_line_style_from_params(params, PARAM_LINE_WIDTH, &line_style, &local_error);
    }

    if (local_error == NULL)
    {
        circle = BB_GEDA_CIRCLE(g_object_new(
            BB_TYPE_GEDA_CIRCLE,

            /* From AdjustableColor */
            "item-color", color,

            /* From AdjustableFillStyle */
            "fill-type", fill_style.type,
            "fill-width", fill_style.width,
            "angle-1", fill_style.angle[0],
            "pitch-1", fill_style.pitch[0],
            "angle-2", fill_style.angle[1],
            "pitch-2", fill_style.pitch[1],

            /* From AdjustableLineStyle */
            "line-width", line_style.line_width,
            "cap-type", line_style.cap_type,
            "dash_type", line_style.dash_type,
            "dash-length", line_style.dash_length,
            "dash-space", line_style.dash_space,

            /* From GedaCircle */
            "center-x", center_x,
            "center-y", center_y,
            "radius", radius,

            NULL
            ));
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        g_clear_object(&circle);
    }

    return circle;
}


static void
bb_geda_circle_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaCircle *circle = BB_GEDA_CIRCLE(item);

    g_return_if_fail(circle != NULL);

    bb_item_renderer_set_color(renderer, circle->color);
    bb_item_renderer_set_fill_style(renderer, circle->fill_style);
    bb_item_renderer_set_line_style(renderer, circle->line_style);

    bb_item_renderer_render_arc(
        renderer,
        circle->center_x,
        circle->center_y,
        circle->radius,
        0,
        360
        );
}


static void
bb_geda_circle_set_cap_type(BbGedaCircle *circle, int cap_type)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);
    g_return_if_fail(bb_cap_type_is_valid(cap_type));

    if (circle->line_style->cap_type != cap_type)
    {
        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        circle->line_style->cap_type = cap_type;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CAP_TYPE]);
    }
}


void
bb_geda_circle_set_angle_1(BbGedaCircle *circle, int angle)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->fill_style != NULL);

    if (circle->fill_style->angle[0] != angle)
    {
        circle->fill_style->angle[0] = angle;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_ANGLE_1]);
    }
}


void
bb_geda_circle_set_angle_2(BbGedaCircle *circle, int angle)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->fill_style != NULL);

    if (circle->fill_style->angle[1] != angle)
    {
        circle->fill_style->angle[1] = angle;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_ANGLE_2]);
    }
}


void
bb_geda_circle_set_center_x(BbGedaCircle *circle, int x)
{
    g_return_if_fail(circle != NULL);

    if (circle->center_x != x)
    {
        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        circle->center_x = x;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CENTER_X]);
    }
}


void
bb_geda_circle_set_center_y(BbGedaCircle *circle, int y)
{
    g_return_if_fail(circle != NULL);

    if (circle->center_y != y)
    {
        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        circle->center_y = y;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CENTER_Y]);
    }
}


static void
bb_geda_circle_set_dash_length(BbGedaCircle *circle, int length)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);

    if (circle->line_style->dash_length != length)
    {
        circle->line_style->dash_length = length;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_DASH_LENGTH]);
    }
}


static void
bb_geda_circle_set_dash_space(BbGedaCircle *circle, int space)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);

    if (circle->line_style->dash_space != space)
    {
        circle->line_style->dash_space = space;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_DASH_SPACE]);
    }
}


static void
bb_geda_circle_set_dash_type(BbGedaCircle *circle, int dash_type)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);
    g_return_if_fail(bb_dash_type_is_valid(dash_type));

    if (circle->line_style->dash_type != dash_type)
    {
        circle->line_style->dash_type = dash_type;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_DASH_TYPE]);
    }
}


void
bb_geda_circle_set_fill_type(BbGedaCircle *circle, int fill_type)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->fill_style != NULL);
    g_return_if_fail(bb_fill_type_is_valid(fill_type));

    if (circle->fill_style->type != fill_type)
    {
        circle->fill_style->type = fill_type;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_FILL_TYPE]);
    }
}


void
bb_geda_circle_set_fill_width(BbGedaCircle *circle, int width)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->fill_style != NULL);

    if (circle->fill_style->width != width)
    {
        circle->fill_style->width = width;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_FILL_WIDTH]);
    }
}


static void
bb_geda_circle_set_item_color(BbGedaCircle *circle, int color)
{
    g_return_if_fail(BB_IS_GEDA_CIRCLE(circle));
    g_return_if_fail(bb_color_is_valid(color));

    if (circle->color != color)
    {
        circle->color = color;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_geda_circle_set_line_width(BbGedaCircle *circle, int width)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->line_style != NULL);

    if (circle->line_style->line_width != width)
    {
        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        circle->line_style->line_width = width;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_LINE_WIDTH]);
    }
}


void
bb_geda_circle_set_pitch_1(BbGedaCircle *circle, int pitch)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->fill_style != NULL);

    if (circle->fill_style->pitch[0] != pitch)
    {
        circle->fill_style->pitch[0] = pitch;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_PITCH_1]);
    }
}


void
bb_geda_circle_set_pitch_2(BbGedaCircle *circle, int pitch)
{
    g_return_if_fail(circle != NULL);
    g_return_if_fail(circle->fill_style != NULL);

    if (circle->fill_style->pitch[1] != pitch)
    {
        circle->fill_style->pitch[1] = pitch;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_PITCH_2]);
    }
}


static void
bb_geda_circle_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ANGLE_1:
            bb_geda_circle_set_angle_1(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_ANGLE_2:
            bb_geda_circle_set_angle_2(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_CAP_TYPE:
            bb_geda_circle_set_cap_type(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_CENTER_X:
            bb_geda_circle_set_center_x(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_CENTER_Y:
            bb_geda_circle_set_center_y(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_geda_circle_set_item_color(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_DASH_LENGTH:
            bb_geda_circle_set_dash_length(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_DASH_SPACE:
            bb_geda_circle_set_dash_space(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_DASH_TYPE:
            bb_geda_circle_set_dash_type(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_FILL_TYPE:
            bb_geda_circle_set_fill_type(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_FILL_WIDTH:
            bb_geda_circle_set_fill_width(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_LINE_WIDTH:
            bb_geda_circle_set_line_width(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_PITCH_1:
            bb_geda_circle_set_pitch_1(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_PITCH_2:
            bb_geda_circle_set_pitch_2(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        case PROP_RADIUS:
            bb_geda_circle_set_radius(BB_GEDA_CIRCLE(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_geda_circle_set_radius(BbGedaCircle *circle, int radius)
{
    g_return_if_fail(circle != NULL);

    if (circle->radius != radius)
    {
        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        circle->radius = radius;

        g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_RADIUS]);
    }
}


static void
bb_geda_circle_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaCircle *circle = BB_GEDA_CIRCLE(item);
    g_return_if_fail(circle != NULL);

    g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

    bb_coord_translate(dx, dy, &circle->center_x, &circle->center_y, 1);

    g_signal_emit(circle, signals[SIG_INVALIDATE], 0);

    g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CENTER_X]);
    g_object_notify_by_pspec(G_OBJECT(circle), properties[PROP_CENTER_Y]);
}


static gboolean
bb_geda_circle_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error)
{
    BbGedaCircle *circle = BB_GEDA_CIRCLE(item);
    g_return_val_if_fail(circle != NULL, FALSE);

    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        BB_GEDA_CIRCLE_TOKEN,
        bb_geda_circle_get_center_x(BB_GEDA_CIRCLE(item)),
        bb_geda_circle_get_center_y(BB_GEDA_CIRCLE(item)),
        bb_geda_circle_get_radius(BB_GEDA_CIRCLE(item)),
        circle->color,
        circle->line_style->line_width,
        circle->line_style->cap_type,
        circle->line_style->dash_type,
        bb_line_style_get_dash_length_for_file(circle->line_style),
        bb_line_style_get_dash_space_for_file(circle->line_style),
        circle->fill_style->type,
        bb_fill_style_get_fill_width_for_file(circle->fill_style),
        bb_fill_style_get_fill_angle_1_for_file(circle->fill_style),
        bb_fill_style_get_fill_pitch_1_for_file(circle->fill_style),
        bb_fill_style_get_fill_angle_2_for_file(circle->fill_style),
        bb_fill_style_get_fill_pitch_2_for_file(circle->fill_style)
        );

    gboolean result = g_output_stream_write_all(
        stream,
        params->str,
        params->len,
        NULL,
        cancellable,
        error
        );

    g_string_free(params, TRUE);

    return result;
}


static void
bb_geda_circle_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGedaCircle *circle = BB_GEDA_CIRCLE(item);

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
bb_geda_circle_write_finish(
    BbGedaItem *item,
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
