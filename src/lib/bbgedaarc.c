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
#include "bbgedaarc.h"
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbadjustablelinestyle.h"
#include "bblibrary.h"


/**
 * The positions of parameters in the file format
 */
enum
{
    PARAM_TOKEN,
    PARAM_CENTER_X,
    PARAM_CENTER_Y,
    PARAM_RADIUS,
    PARAM_START_ANGLE,
    PARAM_SWEEP_ANGLE,
    PARAM_COLOR,
    PARAM_LINE_WIDTH,
    PARAM_CAP_TYPE,
    PARAM_DASH_TYPE,
    PARAM_DASH_LENGTH,
    PARAM_DASH_SPACE,
    N_PARAMETERS
};


enum
{
    PROP_0,

    PROP_CENTER_X,
    PROP_CENTER_Y,
    PROP_RADIUS,
    PROP_START_ANGLE,
    PROP_SWEEP_ANGLE,

    PROP_ITEM_COLOR,

    PROP_CAP_TYPE,
    PROP_DASH_LENGTH,
    PROP_DASH_SPACE,
    PROP_DASH_TYPE,
    PROP_LINE_WIDTH,

    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbGedaArc
{
    BbGedaItem parent;

    BbItemParams *params;

    int center_x;
    int center_y;

    int radius;

    int start_angle;
    int sweep_angle;

    int color;

    BbLineStyle *line_style;
};


static void
bb_geda_arc_adjustable_item_color_init(BbAdjustableLineStyleInterface *iface);

static void
bb_geda_arc_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface);

static BbBounds*
bb_geda_arc_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator);

static BbGedaItem*
bb_geda_arc_clone(BbGedaItem *item);

static void
bb_geda_arc_dispose(GObject *object);

static void
bb_geda_arc_finalize(GObject *object);

static int
bb_geda_arc_get_cap_type(BbGedaArc *arc);

static int
bb_geda_arc_get_dash_space(BbGedaArc *arc);

static int
bb_geda_arc_get_dash_length(BbGedaArc *arc);

static int
bb_geda_arc_get_dash_type(BbGedaArc *arc);

static int
bb_geda_arc_get_item_color(BbGedaArc *arc);

static void
bb_geda_arc_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static int
bb_geda_arc_get_line_width(BbGedaArc *arc);

static void
bb_geda_arc_render(BbGedaItem *item, BbItemRenderer *renderer);

static void
bb_geda_arc_set_cap_type(BbGedaArc *arc, int cap_type);

static void
bb_geda_arc_set_dash_length(BbGedaArc *arc, int dash_length);

static void
bb_geda_arc_set_dash_space(BbGedaArc *arc, int dash_space);

static void
bb_geda_arc_set_dash_type(BbGedaArc *arc, int dash_type);

static void
bb_geda_arc_set_item_color(BbGedaArc *arc, int item_color);

static void
bb_geda_arc_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_geda_arc_set_width(BbGedaArc *arc, int width);

static void
bb_geda_arc_translate(BbGedaItem *item, int dx, int dy);

static gboolean
bb_geda_arc_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error);

static void
bb_geda_arc_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_arc_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbGedaArc,
    bb_geda_arc,
    BB_TYPE_GEDA_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_geda_arc_adjustable_item_color_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_LINE_STYLE, bb_geda_arc_adjustable_line_style_init)
    )


static void
bb_geda_arc_adjustable_item_color_init(BbAdjustableLineStyleInterface *iface)
{
}


static void
bb_geda_arc_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface)
{
}

static BbBounds*
bb_geda_arc_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaArc *arc = BB_GEDA_ARC(item);

    g_return_val_if_fail(arc != NULL, NULL);
    g_return_val_if_fail(arc->line_style != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        arc->center_x - arc->radius,
        arc->center_y - arc->radius,
        arc->center_x + arc->radius,
        arc->center_y + arc->radius,
        arc->line_style->line_width
        );
}


static void
bb_geda_arc_class_init(BbGedaArcClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_geda_arc_dispose;
    object_class->finalize = bb_geda_arc_finalize;
    object_class->get_property = bb_geda_arc_get_property;
    object_class->set_property = bb_geda_arc_set_property;

    BB_GEDA_ITEM_CLASS(klasse)->calculate_bounds = bb_geda_arc_calculate_bounds;
    BB_GEDA_ITEM_CLASS(klasse)->clone = bb_geda_arc_clone;
    BB_GEDA_ITEM_CLASS(klasse)->render = bb_geda_arc_render;
    BB_GEDA_ITEM_CLASS(klasse)->translate = bb_geda_arc_translate;
    BB_GEDA_ITEM_CLASS(klasse)->write = bb_geda_arc_write;
    BB_GEDA_ITEM_CLASS(klasse)->write_async = bb_geda_arc_write_async;
    BB_GEDA_ITEM_CLASS(klasse)->write_finish = bb_geda_arc_write_finish;

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

    bb_object_class_install_property(
        object_class,
        PROP_START_ANGLE,
        properties[PROP_START_ANGLE] = g_param_spec_int(
            "start-angle",
            "Start Angle",
            "The staring angle",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        object_class,
        PROP_SWEEP_ANGLE,
        properties[PROP_SWEEP_ANGLE] = g_param_spec_int(
            "sweep-angle",
            "Sweep Angle",
            "The sweep angle",
            INT_MIN,
            INT_MAX,
            90,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        object_class,
        PROP_LINE_WIDTH,
        properties[PROP_LINE_WIDTH] = g_param_spec_int(
            "width",
            "Line Width",
            "The line width",
            0,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        object_class,
        PROP_ITEM_COLOR,
        "item-color"
        );

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

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_GEDA_ITEM);
}


static BbGedaItem*
bb_geda_arc_clone(BbGedaItem *item)
{
    return BB_GEDA_ITEM(g_object_new(
        BB_TYPE_GEDA_ARC,
        "center-x", bb_geda_arc_get_center_x(BB_GEDA_ARC(item)),
        "center-y", bb_geda_arc_get_center_y(BB_GEDA_ARC(item)),
        "radius", bb_geda_arc_get_radius(BB_GEDA_ARC(item)),
        "start-angle", bb_geda_arc_get_start_angle(BB_GEDA_ARC(item)),
        "sweep-angle", bb_geda_arc_get_sweep_angle(BB_GEDA_ARC(item)),

        "item-color", bb_geda_arc_get_item_color(BB_GEDA_ARC(item)),

        "cap-type", bb_geda_arc_get_cap_type(BB_GEDA_ARC(item)),
        "dash-length", bb_geda_arc_get_dash_length(BB_GEDA_ARC(item)),
        "dash-space", bb_geda_arc_get_dash_space(BB_GEDA_ARC(item)),
        "dash-type", bb_geda_arc_get_dash_type(BB_GEDA_ARC(item)),
        "line-width", bb_geda_arc_get_line_width(BB_GEDA_ARC(item)),

        NULL
        ));
}


static void
bb_geda_arc_dispose(GObject *object)
{
}


static void
bb_geda_arc_finalize(GObject *object)
{
    BbGedaArc *arc = BB_GEDA_ARC(object);

    g_return_if_fail(arc != NULL);

    bb_line_style_free(arc->line_style);
}


static int
bb_geda_arc_get_cap_type(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);
    g_return_val_if_fail(arc->line_style != NULL, 0);

    return arc->line_style->cap_type;
}


int
bb_geda_arc_get_center_x(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);

    return arc->center_x;
}


int
bb_geda_arc_get_center_y(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);

    return arc->center_y;
}


static int
bb_geda_arc_get_dash_length(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);
    g_return_val_if_fail(arc->line_style != NULL, 0);

    return arc->line_style->dash_length;
}


static int
bb_geda_arc_get_dash_space(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);
    g_return_val_if_fail(arc->line_style != NULL, 0);

    return arc->line_style->dash_space;
}


static int
bb_geda_arc_get_dash_type(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);
    g_return_val_if_fail(arc->line_style != NULL, 0);

    return arc->line_style->dash_type;
}


static int
bb_geda_arc_get_item_color(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);

    return arc->color;
}


static void
bb_geda_arc_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CENTER_X:
            g_value_set_int(value, bb_geda_arc_get_center_x(BB_GEDA_ARC(object)));
            break;

        case PROP_CENTER_Y:
            g_value_set_int(value, bb_geda_arc_get_center_y(BB_GEDA_ARC(object)));
            break;

        case PROP_RADIUS:
            g_value_set_int(value, bb_geda_arc_get_radius(BB_GEDA_ARC(object)));
            break;

        case PROP_START_ANGLE:
            g_value_set_int(value, bb_geda_arc_get_start_angle(BB_GEDA_ARC(object)));
            break;

        case PROP_SWEEP_ANGLE:
            g_value_set_int(value, bb_geda_arc_get_sweep_angle(BB_GEDA_ARC(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_geda_arc_get_item_color(BB_GEDA_ARC(object)));
            break;

        case PROP_CAP_TYPE:
            g_value_set_int(value, bb_geda_arc_get_cap_type(BB_GEDA_ARC(object)));
            break;

        case PROP_DASH_LENGTH:
            g_value_set_int(value, bb_geda_arc_get_dash_length(BB_GEDA_ARC(object)));
            break;

        case PROP_DASH_SPACE:
            g_value_set_int(value, bb_geda_arc_get_dash_space(BB_GEDA_ARC(object)));
            break;

        case PROP_DASH_TYPE:
            g_value_set_int(value, bb_geda_arc_get_dash_type(BB_GEDA_ARC(object)));
            break;

        case PROP_LINE_WIDTH:
            g_value_set_int(value, bb_geda_arc_get_line_width(BB_GEDA_ARC(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static int
bb_geda_arc_get_line_width(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);
    g_return_val_if_fail(arc->line_style != NULL, 0);

    return arc->line_style->line_width;
}


int
bb_geda_arc_get_radius(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);

    return arc->radius;
}


int
bb_geda_arc_get_start_angle(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);

    return arc->start_angle;
}


int
bb_geda_arc_get_sweep_angle(BbGedaArc *arc)
{
    g_return_val_if_fail(arc != NULL, 0);

    return arc->sweep_angle;
}


static void
bb_geda_arc_init(BbGedaArc *arc)
{
    g_return_if_fail(arc != NULL);

    arc->line_style = bb_line_style_new();
}


BbGedaArc*
bb_geda_arc_new_with_params(BbParams *params, GError **error)
{
    GError *local_error[N_PARAMETERS] = { NULL };

    g_return_val_if_fail(bb_params_token_matches(params, BB_GEDA_ARC_TOKEN), NULL);

    BbGedaArc *arc = BB_GEDA_ARC(g_object_new(
        BB_TYPE_GEDA_ARC,
        "center-x", bb_params_get_int(params, PARAM_CENTER_X, &local_error[PARAM_CENTER_X]),
        "center-y", bb_params_get_int(params, PARAM_CENTER_Y, &local_error[PARAM_CENTER_Y]),
        "radius", bb_params_get_int(params, PARAM_RADIUS, &local_error[PARAM_RADIUS]),
        "start-angle", bb_params_get_int(params, PARAM_START_ANGLE, &local_error[PARAM_START_ANGLE]),
        "sweep-angle", bb_params_get_int(params, PARAM_SWEEP_ANGLE, &local_error[PARAM_SWEEP_ANGLE]),

        "item-color", bb_params_get_int(params, PARAM_COLOR, &local_error[PARAM_COLOR]),

        "line-width", bb_params_get_int(params, PARAM_LINE_WIDTH, &local_error[PARAM_LINE_WIDTH]),
        "cap-type", bb_params_get_int(params, PARAM_CAP_TYPE, &local_error[PARAM_CAP_TYPE]),
        "dash_type", bb_params_get_int(params, PARAM_DASH_TYPE, &local_error[PARAM_DASH_TYPE]),
        "dash-length", bb_params_get_int(params, PARAM_DASH_LENGTH, &local_error[PARAM_DASH_LENGTH]),
        "dash-space", bb_params_get_int(params, PARAM_DASH_SPACE, &local_error[PARAM_DASH_SPACE]),
        NULL
    ));

    for (int index=0; index < N_PARAMETERS; index++)
    {
        if (local_error[index] != NULL)
        {
            g_propagate_error(error, local_error[index]);
            local_error[index] = NULL;
            g_clear_object(&arc);
            break;
        }
    }

    for (int index=0; index < N_PARAMETERS; index++)
    {
        g_clear_error(&local_error[index]);
    }

    return arc;
}


__attribute__((constructor)) void
bb_geda_arc_register()
{
    bb_geda_arc_get_type();
}


static void
bb_geda_arc_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaArc *arc = BB_GEDA_ARC(item);

    bb_item_renderer_set_color(renderer, arc->color);
    bb_item_renderer_set_line_style(renderer, arc->line_style);

    bb_item_renderer_render_arc(
        renderer,
        arc->center_x,
        arc->center_y,
        arc->radius,
        arc->start_angle,
        arc->sweep_angle
        );
}


static void
bb_geda_arc_set_cap_type(BbGedaArc *arc, int cap_type)
{
    g_return_if_fail(arc != NULL);
    g_return_if_fail(arc->line_style != NULL);

    if (arc->line_style->cap_type != cap_type)
    {
        arc->line_style->cap_type = cap_type;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_CAP_TYPE]);
    }
}


void
bb_geda_arc_set_center_x(BbGedaArc *arc, int x)
{
    g_return_if_fail(arc != NULL);

    if (arc->center_x != x)
    {
        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        arc->center_x = x;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_CENTER_X]);
    }
}


void
bb_geda_arc_set_center_y(BbGedaArc *arc, int y)
{
    g_return_if_fail(arc != NULL);

    if (arc->center_y != y)
    {
        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        arc->center_y = y;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_CENTER_Y]);
    }
}


static void
bb_geda_arc_set_dash_length(BbGedaArc *arc, int dash_length)
{
    g_return_if_fail(arc != NULL);
    g_return_if_fail(arc->line_style != NULL);

    if (arc->line_style->dash_length != dash_length)
    {
        arc->line_style->dash_length = dash_length;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_DASH_LENGTH]);
    }
}


static void
bb_geda_arc_set_dash_space(BbGedaArc *arc, int dash_space)
{
    g_return_if_fail(arc != NULL);
    g_return_if_fail(arc->line_style != NULL);

    if (arc->line_style->dash_space != dash_space)
    {
        arc->line_style->dash_space = dash_space;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_DASH_SPACE]);
    }
}


static void
bb_geda_arc_set_dash_type(BbGedaArc *arc, int dash_type)
{
    g_return_if_fail(arc != NULL);
    g_return_if_fail(arc->line_style != NULL);

    if (arc->line_style->dash_type != dash_type)
    {
        arc->line_style->dash_type = dash_type;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_DASH_TYPE]);
    }
}


static void
bb_geda_arc_set_item_color(BbGedaArc *arc, int color)
{
    g_return_if_fail(arc != NULL);

    if (arc->color != color)
    {
        arc->color = color;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_geda_arc_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CENTER_X:
            bb_geda_arc_set_center_x(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_CENTER_Y:
            bb_geda_arc_set_center_y(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_RADIUS:
            bb_geda_arc_set_radius(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_START_ANGLE:
            bb_geda_arc_set_start_angle(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_SWEEP_ANGLE:
            bb_geda_arc_set_sweep_angle(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_geda_arc_set_item_color(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_CAP_TYPE:
            bb_geda_arc_set_cap_type(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_DASH_LENGTH:
            bb_geda_arc_set_dash_length(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_DASH_SPACE:
            bb_geda_arc_set_dash_space(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_DASH_TYPE:
            bb_geda_arc_set_dash_type(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        case PROP_LINE_WIDTH:
            bb_geda_arc_set_width(BB_GEDA_ARC(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_geda_arc_set_radius(BbGedaArc *arc, int radius)
{
    g_return_if_fail(arc != NULL);

    if (arc->radius != radius)
    {
        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        arc->radius = radius;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_RADIUS]);
    }
}


void
bb_geda_arc_set_start_angle(BbGedaArc *arc, int angle)
{
    g_return_if_fail(arc != NULL);

    if (arc->start_angle != angle)
    {
        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        arc->start_angle = angle;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_START_ANGLE]);
    }
}


void
bb_geda_arc_set_sweep_angle(BbGedaArc *arc, int angle)
{
    g_return_if_fail(arc != NULL);

    if (arc->sweep_angle != angle)
    {
        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        arc->sweep_angle = angle;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_SWEEP_ANGLE]);
    }
}


static void
bb_geda_arc_set_width(BbGedaArc *arc, int width)
{
    g_return_if_fail(arc != NULL);
    g_return_if_fail(arc->line_style != NULL);

    if (arc->line_style->line_width != width)
    {
        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        arc->line_style->line_width = width;

        g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_LINE_WIDTH]);
    }
}


static void
bb_geda_arc_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaArc *arc = BB_GEDA_ARC(item);
    g_return_if_fail(arc != NULL);

    g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

    bb_coord_translate(dx, dy, &arc->center_x, &arc->center_y, 1);

    g_signal_emit(arc, signals[SIG_INVALIDATE], 0);

    g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_CENTER_X]);
    g_object_notify_by_pspec(G_OBJECT(arc), properties[PROP_CENTER_Y]);
}


static gboolean
bb_geda_arc_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error)
{
    BbGedaArc *arc = BB_GEDA_ARC(item);
    g_return_val_if_fail(arc != NULL, FALSE);

    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d %d %d %d %d %d %d\n",
        BB_GEDA_ARC_TOKEN,
        arc->center_x,
        arc->center_y,
        arc->radius,
        arc->start_angle,
        arc->sweep_angle,
        arc->color,
        arc->line_style->line_width,
        arc->line_style->cap_type,
        arc->line_style->dash_type,
        bb_line_style_get_dash_length_for_file(arc->line_style),
        bb_line_style_get_dash_space_for_file(arc->line_style)
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
bb_geda_arc_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGedaArc *arc = BB_GEDA_ARC(item);

    bb_item_params_write_async(
        arc->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_geda_arc_write_finish(
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
