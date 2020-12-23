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
#include "bbgedabox.h"
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbadjustablefillstyle.h"
#include "bbadjustableitemcolor.h"
#include "bbadjustablelinestyle.h"
#include "bbparams.h"
#include "bbcolor.h"
#include "bbcolors.h"


/**
 * The positions of parameters in the file format
 */
enum
{
    PARAM_TOKEN,
    PARAM_CORNER_X,
    PARAM_CORNER_Y,
    PARAM_WIDTH,
    PARAM_HEIGHT,
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

    PROP_X0,
    PROP_X1,
    PROP_Y0,
    PROP_Y1,

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


struct _BbGedaBox
{
    BbGedaItem parent;

    BbItemParams *params;

    int color;

    BbFillStyle *fill_style;
    BbLineStyle *line_style;

    int x[2];
    int y[2];
};


static void
bb_geda_box_adjustable_fill_style_init(
    BbAdjustableFillStyleInterface *iface
    );

static void
bb_geda_box_adjustable_item_color_init(
    BbAdjustableItemColorInterface *iface
    );

static void
bb_geda_box_adjustable_line_style_init(
    BbAdjustableLineStyleInterface *iface
    );

static BbBounds*
bb_geda_box_calculate_bounds(
    BbGedaItem *item,
    BbBoundsCalculator *calculator
    );

static BbGedaItem*
bb_geda_box_clone(
    BbGedaItem *item
    );

static void
bb_geda_box_dispose(
    GObject *object
    );

static void
bb_geda_box_finalize(
    GObject *object
    );

static int
bb_geda_box_get_angle_1(
    BbGedaBox *box
    );

static int
bb_geda_box_get_angle_2(
    BbGedaBox *box
    );

static int
bb_geda_box_get_cap_type(
    BbGedaBox *box
    );

static int
bb_geda_box_get_dash_space(
    BbGedaBox *box
    );

static int
bb_geda_box_get_dash_length(
    BbGedaBox *box
    );

static int
bb_geda_box_get_dash_type(
    BbGedaBox *box
    );

static int
bb_geda_box_get_fill_type(
    BbGedaBox *box
    );

static int
bb_geda_box_get_fill_width(
    BbGedaBox *box
    );

static int
bb_geda_box_get_item_color(
    BbGedaBox *box
    );

static int
bb_geda_box_get_line_width(
    BbGedaBox *box
    );

static int
bb_geda_box_get_pitch_1(
    BbGedaBox *box
    );

static int
bb_geda_box_get_pitch_2(
    BbGedaBox *box
    );

static void
bb_geda_box_get_property(
    GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_box_render(
    BbGedaItem *item,
    BbItemRenderer *renderer
    );

static void
bb_geda_box_set_property(
    GObject *object,
    guint property_id,
    const GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_box_translate(
    BbGedaItem *item,
    int dx,
    int dy
    );

static gboolean
bb_geda_box_write(
    BbGedaItem *item,
    GOutputStream *stream,
    GCancellable *cancellable,
    GError **error
    );

static void
bb_geda_box_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_box_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbGedaBox,
    bb_geda_box,
    BB_TYPE_GEDA_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_FILL_STYLE, bb_geda_box_adjustable_fill_style_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_geda_box_adjustable_item_color_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_LINE_STYLE, bb_geda_box_adjustable_line_style_init)
    )


static void
bb_geda_box_adjustable_fill_style_init(BbAdjustableFillStyleInterface *iface)
{
}


static void
bb_geda_box_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
}


static void
bb_geda_box_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface)
{
}


static BbBounds*
bb_geda_box_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaBox *box = BB_GEDA_BOX(item);

    g_return_val_if_fail(box != NULL, NULL);
    g_return_val_if_fail(box->line_style != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        box->x[0],
        box->y[0],
        box->x[1],
        box->y[1],
        box->line_style->line_width
        );
}


static void
bb_geda_box_class_init(BbGedaBoxClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);

    object_class->dispose = bb_geda_box_dispose;
    object_class->finalize = bb_geda_box_finalize;
    object_class->get_property = bb_geda_box_get_property;
    object_class->set_property = bb_geda_box_set_property;

    BB_GEDA_ITEM_CLASS(klasse)->calculate_bounds = bb_geda_box_calculate_bounds;
    BB_GEDA_ITEM_CLASS(klasse)->clone = bb_geda_box_clone;
    BB_GEDA_ITEM_CLASS(klasse)->render = bb_geda_box_render;
    BB_GEDA_ITEM_CLASS(klasse)->translate = bb_geda_box_translate;
    BB_GEDA_ITEM_CLASS(klasse)->write = bb_geda_box_write;
    BB_GEDA_ITEM_CLASS(klasse)->write_async = bb_geda_box_write_async;
    BB_GEDA_ITEM_CLASS(klasse)->write_finish = bb_geda_box_write_finish;

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
        PROP_X0,
        properties[PROP_X0] = g_param_spec_int(
            "x0",
            "x0",
            "The x coordinate of the first corner",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        object_class,
        PROP_X1,
        properties[PROP_X1] = g_param_spec_int(
            "x1",
            "x1",
            "The x coordinate of the second corner",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        object_class,
        PROP_Y0,
        properties[PROP_Y0] = g_param_spec_int(
            "y0",
            "y0",
            "The y coordinate of the first corner",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        object_class,
        PROP_Y1,
        properties[PROP_Y1] = g_param_spec_int(
            "y1",
            "y1",
            "The y coordinate of the second corner",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_GEDA_ITEM);
}


static BbGedaItem*
bb_geda_box_clone(BbGedaItem *item)
{
    return BB_GEDA_ITEM(g_object_new(
        BB_TYPE_GEDA_BOX,
        "x0", bb_geda_box_get_x0(BB_GEDA_BOX(item)),
        "x1", bb_geda_box_get_x1(BB_GEDA_BOX(item)),
        "y0", bb_geda_box_get_y0(BB_GEDA_BOX(item)),
        "y1", bb_geda_box_get_y1(BB_GEDA_BOX(item)),

        /* From AdjustableFillStyle */
        "angle-1", bb_geda_box_get_angle_1(BB_GEDA_BOX(item)),
        "angle-2", bb_geda_box_get_angle_2(BB_GEDA_BOX(item)),
        "fill-type", bb_geda_box_get_fill_type(BB_GEDA_BOX(item)),
        "fill-width", bb_geda_box_get_fill_width(BB_GEDA_BOX(item)),
        "pitch-1", bb_geda_box_get_pitch_1(BB_GEDA_BOX(item)),
        "pitch-2", bb_geda_box_get_pitch_2(BB_GEDA_BOX(item)),

        /* From AdjustableItemColor */
        "item-color", bb_geda_box_get_item_color(BB_GEDA_BOX(item)),

        /* From AdjustableLineStyle */
        "cap-type", bb_geda_box_get_cap_type(BB_GEDA_BOX(item)),
        "dash-length", bb_geda_box_get_dash_length(BB_GEDA_BOX(item)),
        "dash-space", bb_geda_box_get_dash_space(BB_GEDA_BOX(item)),
        "dash-type", bb_geda_box_get_dash_type(BB_GEDA_BOX(item)),
        "line-width", bb_geda_box_get_line_width(BB_GEDA_BOX(item)),

        NULL
    ));
}


static void
bb_geda_box_dispose(GObject *object)
{
}


static void
bb_geda_box_finalize(GObject *object)
{
    BbGedaBox *box = BB_GEDA_BOX(object);

    g_return_if_fail(box != NULL);

    bb_fill_style_free(box->fill_style);
    bb_line_style_free(box->line_style);
}

static int
bb_geda_box_get_angle_1(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), BB_FILL_STYLE_DEFAULT_ANGLE1);
    g_return_val_if_fail(box->fill_style != NULL, BB_FILL_STYLE_DEFAULT_ANGLE1);

    return box->fill_style->angle[0];
}


static int
bb_geda_box_get_angle_2(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), BB_FILL_STYLE_DEFAULT_ANGLE2);
    g_return_val_if_fail(box->fill_style != NULL, BB_FILL_STYLE_DEFAULT_ANGLE2);

    return box->fill_style->angle[1];
}


static int
bb_geda_box_get_cap_type(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), BB_CAP_TYPE_DEFAULT);
    g_return_val_if_fail(box->line_style != NULL, BB_CAP_TYPE_DEFAULT);

    return box->line_style->cap_type;
}


static int
bb_geda_box_get_dash_length(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), 0);
    g_return_val_if_fail(box->line_style != NULL, 0);

    return box->line_style->dash_length;
}


static int
bb_geda_box_get_dash_space(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), 0);
    g_return_val_if_fail(box->line_style != NULL, 0);

    return box->line_style->dash_space;
}


static int
bb_geda_box_get_dash_type(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), BB_DASH_TYPE_DEFAULT);
    g_return_val_if_fail(box->line_style != NULL, BB_DASH_TYPE_DEFAULT);

    return box->line_style->dash_type;
}


static int
bb_geda_box_get_fill_type(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), BB_FILL_TYPE_DEFAULT);
    g_return_val_if_fail(box->fill_style != NULL, BB_FILL_TYPE_DEFAULT);

    return box->fill_style->type;
}


static int
bb_geda_box_get_fill_width(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), BB_FILL_STYLE_DEFAULT_WIDTH);
    g_return_val_if_fail(box->fill_style != NULL, BB_FILL_STYLE_DEFAULT_WIDTH);

    return box->fill_style->width;
}


static int
bb_geda_box_get_item_color(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), BB_COLOR_GRAPHIC);

    return box->color;
}


static int
bb_geda_box_get_line_width(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), 0);
    g_return_val_if_fail(box->line_style != NULL, 0);

    return box->line_style->line_width;
}


static int
bb_geda_box_get_pitch_1(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), BB_FILL_STYLE_DEFAULT_PITCH);
    g_return_val_if_fail(box->fill_style != NULL, BB_FILL_STYLE_DEFAULT_PITCH);

    return box->fill_style->pitch[0];
}


static int
bb_geda_box_get_pitch_2(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), BB_FILL_STYLE_DEFAULT_PITCH);
    g_return_val_if_fail(box->fill_style != NULL, BB_FILL_STYLE_DEFAULT_PITCH);

    return box->fill_style->pitch[1];
}


static void
bb_geda_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ANGLE_1:
            g_value_set_int(value, bb_geda_box_get_angle_1(BB_GEDA_BOX(object)));
            break;

        case PROP_ANGLE_2:
            g_value_set_int(value, bb_geda_box_get_angle_2(BB_GEDA_BOX(object)));
            break;

        case PROP_CAP_TYPE:
            g_value_set_int(value, bb_geda_box_get_cap_type(BB_GEDA_BOX(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_geda_box_get_item_color(BB_GEDA_BOX(object)));
            break;

        case PROP_DASH_LENGTH:
            g_value_set_int(value, bb_geda_box_get_dash_length(BB_GEDA_BOX(object)));
            break;

        case PROP_DASH_SPACE:
            g_value_set_int(value, bb_geda_box_get_dash_space(BB_GEDA_BOX(object)));
            break;

        case PROP_DASH_TYPE:
            g_value_set_int(value, bb_geda_box_get_dash_type(BB_GEDA_BOX(object)));
            break;

        case PROP_FILL_TYPE:
            g_value_set_int(value, bb_geda_box_get_fill_type(BB_GEDA_BOX(object)));
            break;

        case PROP_FILL_WIDTH:
            g_value_set_int(value, bb_geda_box_get_fill_width(BB_GEDA_BOX(object)));
            break;

        case PROP_LINE_WIDTH:
            g_value_set_int(value, bb_geda_box_get_line_width(BB_GEDA_BOX(object)));
            break;

        case PROP_PITCH_1:
            g_value_set_int(value, bb_geda_box_get_pitch_1(BB_GEDA_BOX(object)));
            break;

        case PROP_PITCH_2:
            g_value_set_int(value, bb_geda_box_get_pitch_2(BB_GEDA_BOX(object)));
            break;

        case PROP_X0:
            g_value_set_int(value, bb_geda_box_get_x0(BB_GEDA_BOX(object)));
            break;

        case PROP_X1:
            g_value_set_int(value, bb_geda_box_get_x1(BB_GEDA_BOX(object)));
            break;

        case PROP_Y0:
            g_value_set_int(value, bb_geda_box_get_y0(BB_GEDA_BOX(object)));
            break;

        case PROP_Y1:
            g_value_set_int(value, bb_geda_box_get_y1(BB_GEDA_BOX(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_geda_box_get_x0(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), 0);

    return box->x[0];
}


int
bb_geda_box_get_x1(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), 0);

    return box->x[1];
}


int
bb_geda_box_get_y0(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), 0);

    return box->y[0];
}


int
bb_geda_box_get_y1(BbGedaBox *box)
{
    g_return_val_if_fail(BB_IS_GEDA_BOX(box), 0);

    return box->y[1];
}


static void
bb_geda_box_init(BbGedaBox *box)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));

    box->fill_style = bb_fill_style_new();
    box->line_style = bb_line_style_new();
}


BbGedaBox*
bb_geda_box_new()
{
    return g_object_new(BB_TYPE_GEDA_BOX, NULL);
}


BbGedaBox*
bb_geda_box_new_with_params(BbParams *params, GError **error)
{
    GError *local_error = NULL;

    g_return_val_if_fail(bb_params_token_matches(params, BB_GEDA_BOX_TOKEN), NULL);

    BbGedaBox *box = NULL;
    int color;
    int corner_x;
    int corner_y;
    BbFillStyle fill_style;
    int height;
    BbLineStyle line_style;
    int width;

    corner_x = bb_params_get_int(params, PARAM_CORNER_X, &local_error);

    if (local_error == NULL)
    {
        corner_y = bb_params_get_int(params, PARAM_CORNER_Y, &local_error);
    }

    if (local_error == NULL)
    {
        width = bb_params_get_int(params, PARAM_WIDTH, &local_error);
    }

    if (local_error == NULL)
    {
        height = bb_params_get_int(params, PARAM_HEIGHT, &local_error);
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
        box = BB_GEDA_BOX(g_object_new(
            BB_TYPE_GEDA_BOX,

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

            "x0", corner_x,
            "y0", corner_y,
            "x1", corner_x + width,
            "y1", corner_y + height,

            NULL
            ));
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        g_clear_object(&box);
    }

    return box;
}


static void
bb_geda_box_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaBox *box = BB_GEDA_BOX(item);

    g_return_if_fail(box != NULL);

    bb_item_renderer_set_color(renderer, box->color);
    bb_item_renderer_set_fill_style(renderer, box->fill_style);
    bb_item_renderer_set_line_style(renderer, box->line_style);

    bb_item_renderer_render_absolute_move_to(renderer, box->x[0], box->y[0]);
    bb_item_renderer_render_absolute_line_to(renderer, box->x[1], box->y[0]);
    bb_item_renderer_render_absolute_line_to(renderer, box->x[1], box->y[1]);
    bb_item_renderer_render_absolute_line_to(renderer, box->x[0], box->y[1]);
    bb_item_renderer_close_path(renderer);
}


void
bb_geda_box_set_angle_1(BbGedaBox *box, int angle)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->angle[0] != angle)
    {
        box->fill_style->angle[0] = angle;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_ANGLE_1]);
    }
}


void
bb_geda_box_set_angle_2(BbGedaBox *box, int angle)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->angle[1] != angle)
    {
        box->fill_style->angle[1] = angle;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_ANGLE_2]);
    }
}


static void
bb_geda_box_set_cap_type(BbGedaBox *box, int cap_type)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->line_style != NULL);
    g_return_if_fail(bb_cap_type_is_valid(cap_type));

    if (box->line_style->cap_type != cap_type)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->line_style->cap_type = cap_type;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_CAP_TYPE]);
    }
}


static void
bb_geda_box_set_dash_length(BbGedaBox *box, int length)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->line_style != NULL);

    if (box->line_style->dash_length != length)
    {
        box->line_style->dash_length = length;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_DASH_LENGTH]);
    }
}


static void
bb_geda_box_set_dash_space(BbGedaBox *box, int space)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->line_style != NULL);

    if (box->line_style->dash_space != space)
    {
        box->line_style->dash_space = space;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_DASH_SPACE]);
    }
}


static void
bb_geda_box_set_dash_type(BbGedaBox *box, int dash_type)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->line_style != NULL);
    g_return_if_fail(bb_dash_type_is_valid(dash_type));

    if (box->line_style->dash_type != dash_type)
    {
        box->line_style->dash_type = dash_type;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_DASH_TYPE]);
    }
}


static void
bb_geda_box_set_item_color(BbGedaBox *box, int color)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(bb_color_is_valid(color));

    if (box->color != color)
    {
        box->color = color;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_ITEM_COLOR]);
    }
}


void
bb_geda_box_set_fill_type(BbGedaBox *box, int fill_type)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->fill_style != NULL);
    g_return_if_fail(bb_fill_type_is_valid(fill_type));

    if (box->fill_style->type != fill_type)
    {
        box->fill_style->type = fill_type;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_FILL_TYPE]);
    }
}


void
bb_geda_box_set_fill_width(BbGedaBox *box, int width)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->width != width)
    {
        box->fill_style->width = width;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_FILL_WIDTH]);
    }
}


static void
bb_geda_box_set_line_width(BbGedaBox *box, int width)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->line_style != NULL);

    if (box->line_style->line_width != width)
    {
        box->line_style->line_width = width;

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_LINE_WIDTH]);
    }
}


void
bb_geda_box_set_pitch_1(BbGedaBox *box, int pitch)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->pitch[0] != pitch)
    {
        box->fill_style->pitch[0] = pitch;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_PITCH_1]);
    }
}


void
bb_geda_box_set_pitch_2(BbGedaBox *box, int pitch)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->pitch[1] != pitch)
    {
        box->fill_style->pitch[1] = pitch;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_PITCH_2]);
    }
}


static void
bb_geda_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ANGLE_1:
            bb_geda_box_set_angle_1(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_ANGLE_2:
            bb_geda_box_set_angle_2(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_CAP_TYPE:
            bb_geda_box_set_cap_type(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_geda_box_set_item_color(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_DASH_LENGTH:
            bb_geda_box_set_dash_length(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_DASH_SPACE:
            bb_geda_box_set_dash_space(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_DASH_TYPE:
            bb_geda_box_set_dash_type(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_FILL_TYPE:
            bb_geda_box_set_fill_type(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_FILL_WIDTH:
            bb_geda_box_set_fill_width(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_LINE_WIDTH:
            bb_geda_box_set_line_width(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_PITCH_1:
            bb_geda_box_set_pitch_1(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_PITCH_2:
            bb_geda_box_set_pitch_2(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_X0:
            bb_geda_box_set_x0(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_X1:
            bb_geda_box_set_x1(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_Y0:
            bb_geda_box_set_y0(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        case PROP_Y1:
            bb_geda_box_set_y1(BB_GEDA_BOX(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_geda_box_set_x0(BbGedaBox *box, int x)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));

    if (box->x[0] != x)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->x[0] = x;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X0]);
    }
}


void
bb_geda_box_set_x1(BbGedaBox *box, int x)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));

    if (box->x[1] != x)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->x[1] = x;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X1]);
    }
}


void
bb_geda_box_set_y0(BbGedaBox *box, int y)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));

    if (box->y[0] != y)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->y[0] = y;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y0]);
    }
}


void
bb_geda_box_set_y1(BbGedaBox *box, int y)
{
    g_return_if_fail(BB_IS_GEDA_BOX(box));

    if (box->y[1] != y)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->y[1] = y;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y1]);
    };
}


static void
bb_geda_box_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaBox *box = BB_GEDA_BOX(item);
    g_return_if_fail(box != NULL);

    bb_coord_translate(dx, dy, box->x, box->y, 2);

    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X0]);
    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y0]);
    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X1]);
    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y1]);
}


static gboolean
bb_geda_box_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error)
{
    BbGedaBox *box = BB_GEDA_BOX(item);
    g_return_val_if_fail(box != NULL, FALSE);

    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
        BB_GEDA_BOX_TOKEN,
        MIN(box->x[0], box->x[1]),
        MIN(box->y[0], box->y[1]),
        ABS(box->x[0] - box->x[1]),
        ABS(box->y[0] - box->y[1]),
        box->color,
        box->line_style->line_width,
        box->line_style->cap_type,
        box->line_style->dash_type,
        bb_line_style_get_dash_length_for_file(box->line_style),
        bb_line_style_get_dash_space_for_file(box->line_style),
        box->fill_style->type,
        bb_fill_style_get_fill_width_for_file(box->fill_style),
        bb_fill_style_get_fill_angle_1_for_file(box->fill_style),
        bb_fill_style_get_fill_pitch_1_for_file(box->fill_style),
        bb_fill_style_get_fill_angle_2_for_file(box->fill_style),
        bb_fill_style_get_fill_pitch_2_for_file(box->fill_style)
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
bb_geda_box_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGedaBox *box = BB_GEDA_BOX(item);

    bb_item_params_write_async(
        box->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_geda_box_write_finish(
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
