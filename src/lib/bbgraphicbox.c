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
#include "bbgraphicbox.h"
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbadjustablefillstyle.h"
#include "bbadjustableitemcolor.h"
#include "bbadjustablelinestyle.h"


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


struct _BbGraphicBox
{
    BbSchematicItem parent;

    BbItemParams *params;

    int color;

    BbFillStyle *fill_style;
    BbLineStyle *line_style;

    int x[2];
    int y[2];
};


static void
bb_graphic_box_adjustable_fill_style_init(BbAdjustableFillStyleInterface *iface);

static void
bb_graphic_box_adjustable_item_color_init(BbAdjustableItemColorInterface *iface);

static void
bb_graphic_box_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface);

static BbBounds*
bb_graphic_box_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static BbSchematicItem*
bb_graphic_box_clone(BbSchematicItem *item);

static void
bb_graphic_box_dispose(GObject *object);

static void
bb_graphic_box_finalize(GObject *object);

static int
bb_graphic_box_get_angle_1(BbGraphicBox *box);

static int
bb_graphic_box_get_angle_2(BbGraphicBox *box);

static int
bb_graphic_box_get_cap_type(BbGraphicBox *box);

static int
bb_graphic_box_get_dash_space(BbGraphicBox *box);

static int
bb_graphic_box_get_dash_length(BbGraphicBox *box);

static int
bb_graphic_box_get_dash_type(BbGraphicBox *box);

static int
bb_graphic_box_get_fill_type(BbGraphicBox *box);

static int
bb_graphic_box_get_fill_width(BbGraphicBox *box);

static int
bb_graphic_box_get_item_color(BbGraphicBox *box);

static int
bb_graphic_box_get_line_width(BbGraphicBox *box);

static int
bb_graphic_box_get_pitch_1(BbGraphicBox *box);

static int
bb_graphic_box_get_pitch_2(BbGraphicBox *box);

static void
bb_graphic_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphic_box_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_graphic_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_graphic_box_translate(BbSchematicItem *item, int dx, int dy);

static void
bb_graphic_box_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_graphic_box_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbGraphicBox,
    bb_graphic_box,
    BB_TYPE_SCHEMATIC_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_FILL_STYLE, bb_graphic_box_adjustable_fill_style_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_graphic_box_adjustable_item_color_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_LINE_STYLE, bb_graphic_box_adjustable_line_style_init)
    )


static void
bb_graphic_box_adjustable_fill_style_init(BbAdjustableFillStyleInterface *iface)
{
}


static void
bb_graphic_box_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
}


static void
bb_graphic_box_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface)
{
}


static BbBounds*
bb_graphic_box_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator)
{
    BbGraphicBox *box = BB_GRAPHIC_BOX(item);

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
bb_graphic_box_class_init(BbGraphicBoxClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);

    object_class->dispose = bb_graphic_box_dispose;
    object_class->finalize = bb_graphic_box_finalize;
    object_class->get_property = bb_graphic_box_get_property;
    object_class->set_property = bb_graphic_box_set_property;

    BB_SCHEMATIC_ITEM_CLASS(klasse)->calculate_bounds = bb_graphic_box_calculate_bounds;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->clone = bb_graphic_box_clone;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->render = bb_graphic_box_render;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->translate = bb_graphic_box_translate;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_async = bb_graphic_box_write_async;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_finish = bb_graphic_box_write_finish;

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

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_SCHEMATIC_ITEM);
}


static BbSchematicItem*
bb_graphic_box_clone(BbSchematicItem *item)
{
    return BB_SCHEMATIC_ITEM(g_object_new(
        BB_TYPE_GRAPHIC_BOX,
        "x0", bb_graphic_box_get_x0(BB_GRAPHIC_BOX(item)),
        "x1", bb_graphic_box_get_x1(BB_GRAPHIC_BOX(item)),
        "y0", bb_graphic_box_get_y0(BB_GRAPHIC_BOX(item)),
        "y1", bb_graphic_box_get_y1(BB_GRAPHIC_BOX(item)),

        /* From AdjustableFillStyle */
        "angle-1", bb_graphic_box_get_angle_1(BB_GRAPHIC_BOX(item)),
        "angle-2", bb_graphic_box_get_angle_2(BB_GRAPHIC_BOX(item)),
        "fill-type", bb_graphic_box_get_fill_type(BB_GRAPHIC_BOX(item)),
        "fill-width", bb_graphic_box_get_fill_width(BB_GRAPHIC_BOX(item)),
        "pitch-1", bb_graphic_box_get_pitch_1(BB_GRAPHIC_BOX(item)),
        "pitch-2", bb_graphic_box_get_pitch_2(BB_GRAPHIC_BOX(item)),

        /* From AdjustableItemColor */
        "item-color", bb_graphic_box_get_item_color(BB_GRAPHIC_BOX(item)),

        /* From AdjustableLineStyle */
        "cap-type", bb_graphic_box_get_cap_type(BB_GRAPHIC_BOX(item)),
        "dash-length", bb_graphic_box_get_dash_length(BB_GRAPHIC_BOX(item)),
        "dash-space", bb_graphic_box_get_dash_space(BB_GRAPHIC_BOX(item)),
        "dash-type", bb_graphic_box_get_dash_type(BB_GRAPHIC_BOX(item)),
        "line-width", bb_graphic_box_get_line_width(BB_GRAPHIC_BOX(item)),

        NULL
    ));
}


static void
bb_graphic_box_dispose(GObject *object)
{
}


static void
bb_graphic_box_finalize(GObject *object)
{
    BbGraphicBox *box = BB_GRAPHIC_BOX(object);

    g_return_if_fail(box != NULL);

    bb_fill_style_free(box->fill_style);
    bb_line_style_free(box->line_style);
}

static int
bb_graphic_box_get_angle_1(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->fill_style != NULL, 0);

    return box->fill_style->angle[0];
}


static int
bb_graphic_box_get_angle_2(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->fill_style != NULL, 0);

    return box->fill_style->angle[1];
}


static int
bb_graphic_box_get_cap_type(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->line_style != NULL, 0);

    return box->line_style->cap_type;
}


static int
bb_graphic_box_get_dash_length(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->line_style != NULL, 0);

    return box->line_style->dash_length;
}


static int
bb_graphic_box_get_dash_space(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->line_style != NULL, 0);

    return box->line_style->dash_space;
}


static int
bb_graphic_box_get_dash_type(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->line_style != NULL, 0);

    return box->line_style->dash_type;
}


static int
bb_graphic_box_get_fill_type(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->fill_style != NULL, 0);

    return box->fill_style->type;
}


static int
bb_graphic_box_get_fill_width(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->fill_style != NULL, 0);

    return box->fill_style->width;
}


static int
bb_graphic_box_get_item_color(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->color;
}


static int
bb_graphic_box_get_line_width(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->line_style != NULL, 0);

    return box->line_style->line_width;
}


static int
bb_graphic_box_get_pitch_1(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->fill_style != NULL, 0);

    return box->fill_style->pitch[0];
}


static int
bb_graphic_box_get_pitch_2(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);
    g_return_val_if_fail(box->fill_style != NULL, 0);

    return box->fill_style->pitch[1];
}


static void
bb_graphic_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ANGLE_1:
            g_value_set_int(value, bb_graphic_box_get_angle_1(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_ANGLE_2:
            g_value_set_int(value, bb_graphic_box_get_angle_2(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_CAP_TYPE:
            g_value_set_int(value, bb_graphic_box_get_cap_type(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_graphic_box_get_item_color(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_DASH_LENGTH:
            g_value_set_int(value, bb_graphic_box_get_dash_length(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_DASH_SPACE:
            g_value_set_int(value, bb_graphic_box_get_dash_space(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_DASH_TYPE:
            g_value_set_int(value, bb_graphic_box_get_dash_type(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_FILL_TYPE:
            g_value_set_int(value, bb_graphic_box_get_fill_type(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_FILL_WIDTH:
            g_value_set_int(value, bb_graphic_box_get_fill_width(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_LINE_WIDTH:
            g_value_set_int(value, bb_graphic_box_get_line_width(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_PITCH_1:
            g_value_set_int(value, bb_graphic_box_get_pitch_1(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_PITCH_2:
            g_value_set_int(value, bb_graphic_box_get_pitch_2(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_X0:
            g_value_set_int(value, bb_graphic_box_get_x0(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_X1:
            g_value_set_int(value, bb_graphic_box_get_x1(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_Y0:
            g_value_set_int(value, bb_graphic_box_get_y0(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_Y1:
            g_value_set_int(value, bb_graphic_box_get_y1(BB_GRAPHIC_BOX(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_graphic_box_get_x0(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->x[0];
}


int
bb_graphic_box_get_x1(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->x[1];
}


int
bb_graphic_box_get_y0(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->y[0];
}


int
bb_graphic_box_get_y1(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->y[1];
}


static void
bb_graphic_box_init(BbGraphicBox *box)
{
    g_return_if_fail(box != NULL);

    box->fill_style = bb_fill_style_new();
    box->line_style = bb_line_style_new();
}


BbGraphicBox*
bb_graphic_box_new()
{
    return g_object_new(BB_TYPE_GRAPHIC_BOX, NULL);
}


__attribute__((constructor)) void
bb_graphic_box_register()
{
    bb_graphic_box_get_type();
}


static void
bb_graphic_box_render(BbSchematicItem *item, BbItemRenderer *renderer)
{
    BbGraphicBox *box = BB_GRAPHIC_BOX(item);

    g_return_if_fail(box != NULL);

    bb_item_renderer_set_color(renderer, box->color);
    bb_item_renderer_set_fill_style(renderer, box->fill_style);
    bb_item_renderer_set_line_style(renderer, box->line_style);

    bb_item_renderer_render_graphic_box(renderer, box);
}


void
bb_graphic_box_set_angle_1(BbGraphicBox *box, int angle)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->angle[0] != angle)
    {
        box->fill_style->angle[0] = angle;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_ANGLE_1]);
    }
}


void
bb_graphic_box_set_angle_2(BbGraphicBox *box, int angle)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->angle[1] != angle)
    {
        box->fill_style->angle[1] = angle;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_ANGLE_2]);
    }
}


static void
bb_graphic_box_set_cap_type(BbGraphicBox *box, int type)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->line_style != NULL);

    if (box->line_style->cap_type != type)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->line_style->cap_type = type;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_CAP_TYPE]);
    }
}


static void
bb_graphic_box_set_dash_length(BbGraphicBox *box, int length)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->line_style != NULL);

    if (box->line_style->dash_length != length)
    {
        box->line_style->dash_length = length;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_DASH_LENGTH]);
    }
}


static void
bb_graphic_box_set_dash_space(BbGraphicBox *box, int space)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->line_style != NULL);

    if (box->line_style->dash_space != space)
    {
        box->line_style->dash_space = space;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_DASH_SPACE]);
    }
}


static void
bb_graphic_box_set_dash_type(BbGraphicBox *box, int type)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->line_style != NULL);

    if (box->line_style->dash_type != type)
    {
        box->line_style->dash_type = type;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_DASH_TYPE]);
    }
}


static void
bb_graphic_box_set_item_color(BbGraphicBox *box, int color)
{
    g_return_if_fail(box != NULL);

    if (box->color != color)
    {
        box->color = color;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_ITEM_COLOR]);
    }
}


void
bb_graphic_box_set_fill_type(BbGraphicBox *box, int type)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->type != type)
    {
        box->fill_style->type = type;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_FILL_TYPE]);
    }
}


void
bb_graphic_box_set_fill_width(BbGraphicBox *box, int width)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->width != width)
    {
        box->fill_style->width = width;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_FILL_WIDTH]);
    }
}


static void
bb_graphic_box_set_line_width(BbGraphicBox *box, int width)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->line_style != NULL);

    if (box->line_style->line_width != width)
    {
        box->line_style->line_width = width;

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_LINE_WIDTH]);
    }
}


void
bb_graphic_box_set_pitch_1(BbGraphicBox *box, int pitch)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->pitch[0] != pitch)
    {
        box->fill_style->pitch[0] = pitch;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_PITCH_1]);
    }
}


void
bb_graphic_box_set_pitch_2(BbGraphicBox *box, int pitch)
{
    g_return_if_fail(box != NULL);
    g_return_if_fail(box->fill_style != NULL);

    if (box->fill_style->pitch[1] != pitch)
    {
        box->fill_style->pitch[1] = pitch;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_PITCH_2]);
    }
}


static void
bb_graphic_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ANGLE_1:
            bb_graphic_box_set_angle_1(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_ANGLE_2:
            bb_graphic_box_set_angle_2(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_CAP_TYPE:
            bb_graphic_box_set_cap_type(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_graphic_box_set_item_color(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_DASH_LENGTH:
            bb_graphic_box_set_dash_length(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_DASH_SPACE:
            bb_graphic_box_set_dash_space(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_DASH_TYPE:
            bb_graphic_box_set_dash_type(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_FILL_TYPE:
            bb_graphic_box_set_fill_type(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_FILL_WIDTH:
            bb_graphic_box_set_fill_width(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_LINE_WIDTH:
            bb_graphic_box_set_line_width(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_PITCH_1:
            bb_graphic_box_set_pitch_1(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_PITCH_2:
            bb_graphic_box_set_pitch_2(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_X0:
            bb_graphic_box_set_x0(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_X1:
            bb_graphic_box_set_x1(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_Y0:
            bb_graphic_box_set_y0(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_Y1:
            bb_graphic_box_set_y1(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_graphic_box_set_x0(BbGraphicBox *box, int x)
{
    g_return_if_fail(box != NULL);

    if (box->x[0] != x)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->x[0] = x;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X0]);
    }
}


void
bb_graphic_box_set_x1(BbGraphicBox *box, int x)
{
    g_return_if_fail(box != NULL);

    if (box->x[1] != x)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->x[1] = x;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X1]);
    }
}


void
bb_graphic_box_set_y0(BbGraphicBox *box, int y)
{
    g_return_if_fail(box != NULL);

    if (box->y[0] != y)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->y[0] = y;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y0]);
    }
}


void
bb_graphic_box_set_y1(BbGraphicBox *box, int y)
{
    g_return_if_fail(box != NULL);

    if (box->y[1] != y)
    {
        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        box->y[1] = y;

        g_signal_emit(box, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y1]);
    };
}


static void
bb_graphic_box_translate(BbSchematicItem *item, int dx, int dy)
{
    BbGraphicBox *box = BB_GRAPHIC_BOX(item);
    g_return_if_fail(box != NULL);

    bb_coord_translate(dx, dy, box->x, box->y, 2);

    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X0]);
    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y0]);
    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X1]);
    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y1]);
}


static void
bb_graphic_box_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGraphicBox *box = BB_GRAPHIC_BOX(item);

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
bb_graphic_box_write_finish(
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
