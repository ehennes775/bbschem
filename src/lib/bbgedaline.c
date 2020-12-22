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
#include "bbgedaline.h"
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbadjustablelinestyle.h"
#include "bbadjustableitemcolor.h"
#include "bbcolor.h"


/**
 * The positions of parameters in the file format
 */
enum
{
    PARAM_TOKEN,
    PARAM_X0,
    PARAM_Y0,
    PARAM_X1,
    PARAM_Y1,
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

    PROP_X0,
    PROP_X1,
    PROP_Y0,
    PROP_Y1,

    /* From AdjustableItemColor */
    PROP_ITEM_COLOR,

    /* From AdjustableLineStyle */
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


struct _BbGedaLine
{
    BbGedaItem parent;

    BbItemParams *params;

    int x[2];
    int y[2];

    int color;

    BbLineStyle *line_style;
};


static void
bb_geda_line_adjustable_item_color_init(BbAdjustableLineStyleInterface *iface);

static void
bb_geda_line_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface);

static BbBounds*
bb_geda_line_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator);

static BbGedaItem*
bb_geda_line_clone(BbGedaItem *item);

static void
bb_geda_line_dispose(GObject *object);

static void
bb_geda_line_finalize(GObject *object);

static int
bb_geda_line_get_cap_type(BbGedaLine *line);

static int
bb_geda_line_get_dash_length(BbGedaLine *line);

static int
bb_geda_line_get_dash_space(BbGedaLine *line);

static int
bb_geda_line_get_dash_type(BbGedaLine *line);

static int
bb_geda_line_get_item_color(BbGedaLine *line);

static int
bb_geda_line_get_line_width(BbGedaLine *line);

static void
bb_geda_line_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_line_render(BbGedaItem *item, BbItemRenderer *renderer);

static void
bb_geda_line_set_cap_type(BbGedaLine *line, int type);

static void
bb_geda_line_set_dash_length(BbGedaLine *line, int length);

static void
bb_geda_line_set_dash_space(BbGedaLine *line, int space);

static void
bb_geda_line_set_dash_type(BbGedaLine *line, int type);

static void
bb_geda_line_set_item_color(BbGedaLine *line, int color);

static void
bb_geda_line_set_line_width(BbGedaLine *line, int width);

static void
bb_geda_line_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_geda_line_translate(BbGedaItem *item, int dx, int dy);

static gboolean
bb_geda_line_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error);

static void
bb_geda_line_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_line_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_CODE(
    BbGedaLine,
    bb_geda_line,
    BB_TYPE_GEDA_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_geda_line_adjustable_item_color_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_LINE_STYLE, bb_geda_line_adjustable_line_style_init)
    )


static void
bb_geda_line_adjustable_item_color_init(BbAdjustableLineStyleInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static void
bb_geda_line_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static BbBounds*
bb_geda_line_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaLine *line = BB_GEDA_LINE(item);

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
bb_geda_line_class_init(BbGedaLineClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_geda_line_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_geda_line_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_geda_line_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_geda_line_set_property;

    BB_GEDA_ITEM_CLASS(klasse)->calculate_bounds = bb_geda_line_calculate_bounds;
    BB_GEDA_ITEM_CLASS(klasse)->clone = bb_geda_line_clone;
    BB_GEDA_ITEM_CLASS(klasse)->render = bb_geda_line_render;
    BB_GEDA_ITEM_CLASS(klasse)->translate = bb_geda_line_translate;
    BB_GEDA_ITEM_CLASS(klasse)->write = bb_geda_line_write;
    BB_GEDA_ITEM_CLASS(klasse)->write_async = bb_geda_line_write_async;
    BB_GEDA_ITEM_CLASS(klasse)->write_finish = bb_geda_line_write_finish;

    /* From AdjustableItemColor */
    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_ITEM_COLOR,
        "item-color"
        );

    /* From AdjustableLineStyle */
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

    properties[PROP_LINE_WIDTH] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_LINE_WIDTH,
        "line-width"
        );

    properties[PROP_X0] = bb_object_class_install_property(
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

    properties[PROP_X1] = bb_object_class_install_property(
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

    properties[PROP_Y0] = bb_object_class_install_property(
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

    properties[PROP_Y1] = bb_object_class_install_property(
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

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_GEDA_ITEM);
}


static BbGedaItem*
bb_geda_line_clone(BbGedaItem *item)
{
    return BB_GEDA_ITEM(g_object_new(
        BB_TYPE_GEDA_LINE,
        "x0", bb_geda_line_get_x0(BB_GEDA_LINE(item)),
        "x1", bb_geda_line_get_x1(BB_GEDA_LINE(item)),
        "y0", bb_geda_line_get_y0(BB_GEDA_LINE(item)),
        "y1", bb_geda_line_get_y1(BB_GEDA_LINE(item)),

        /* From AdjustableItemColor */
        "item-color", bb_geda_line_get_item_color(BB_GEDA_LINE(item)),

        /* From AdjustableLineStyle */
        "cap-type", bb_geda_line_get_cap_type(BB_GEDA_LINE(item)),
        "dash-length", bb_geda_line_get_dash_length(BB_GEDA_LINE(item)),
        "dash-space", bb_geda_line_get_dash_space(BB_GEDA_LINE(item)),
        "dash-type", bb_geda_line_get_dash_type(BB_GEDA_LINE(item)),
        "line-width", bb_geda_line_get_line_width(BB_GEDA_LINE(item)),

        NULL
    ));
}


static void
bb_geda_line_dispose(GObject *object)
{
}


static void
bb_geda_line_finalize(GObject *object)
{
    BbGedaLine *line = BB_GEDA_LINE(object);

    g_return_if_fail(line != NULL);

    bb_line_style_free(line->line_style);
}


static int
bb_geda_line_get_cap_type(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->cap_type;
}


static int
bb_geda_line_get_dash_length(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->dash_length;
}


static int
bb_geda_line_get_dash_space(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->dash_space;
}


static int
bb_geda_line_get_dash_type(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->dash_type;
}


static int
bb_geda_line_get_item_color(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->color;
}


static int
bb_geda_line_get_line_width(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);
    g_return_val_if_fail(line->line_style != NULL, 0);

    return line->line_style->line_width;
}


static void
bb_geda_line_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAP_TYPE:
            g_value_set_int(value, bb_geda_line_get_cap_type(BB_GEDA_LINE(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_geda_line_get_item_color(BB_GEDA_LINE(object)));
            break;

        case PROP_DASH_LENGTH:
            g_value_set_int(value, bb_geda_line_get_dash_length(BB_GEDA_LINE(object)));
            break;

        case PROP_DASH_SPACE:
            g_value_set_int(value, bb_geda_line_get_dash_space(BB_GEDA_LINE(object)));
            break;

        case PROP_DASH_TYPE:
            g_value_set_int(value, bb_geda_line_get_dash_type(BB_GEDA_LINE(object)));
            break;

        case PROP_LINE_WIDTH:
            g_value_set_int(value, bb_geda_line_get_line_width(BB_GEDA_LINE(object)));
            break;

        case PROP_X0:
            g_value_set_int(value, bb_geda_line_get_x0(BB_GEDA_LINE(object)));
            break;

        case PROP_X1:
            g_value_set_int(value, bb_geda_line_get_x1(BB_GEDA_LINE(object)));
            break;

        case PROP_Y0:
            g_value_set_int(value, bb_geda_line_get_y0(BB_GEDA_LINE(object)));
            break;

        case PROP_Y1:
            g_value_set_int(value, bb_geda_line_get_y1(BB_GEDA_LINE(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_geda_line_get_x0(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->x[0];
}


int
bb_geda_line_get_x1(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->x[1];
}


int
bb_geda_line_get_y0(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->y[0];
}


int
bb_geda_line_get_y1(BbGedaLine *line)
{
    g_return_val_if_fail(line != NULL, 0);

    return line->y[1];
}


static void
bb_geda_line_init(BbGedaLine *line)
{
    g_return_if_fail(line != NULL);

    line->line_style = bb_line_style_new();
}


BbGedaLine*
bb_geda_line_new()
{
    return g_object_new(BB_TYPE_GEDA_LINE, NULL);
}


BbGedaLine*
bb_geda_line_new_with_params(BbParams *params, GError **error)
{
    GError *local_error[N_PARAMETERS] = { NULL };

    g_return_val_if_fail(bb_params_token_matches(params, BB_GEDA_LINE_TOKEN), NULL);

    BbGedaLine *line = BB_GEDA_LINE(g_object_new(
        BB_TYPE_GEDA_LINE,
        "x0", bb_params_get_int(params, PARAM_X0, &local_error[PARAM_X0]),
        "y0", bb_params_get_int(params, PARAM_Y0, &local_error[PARAM_Y0]),
        "x1", bb_params_get_int(params, PARAM_X1, &local_error[PARAM_X1]),
        "y1", bb_params_get_int(params, PARAM_Y1, &local_error[PARAM_Y1]),

        "item-color", bb_params_get_int(params, PARAM_COLOR, &local_error[PARAM_COLOR]),

        "line-width",  bb_params_get_int(params, PARAM_LINE_WIDTH, &local_error[PARAM_LINE_WIDTH]),
        "cap-type",  bb_params_get_int(params, PARAM_CAP_TYPE, &local_error[PARAM_CAP_TYPE]),
        "dash_type",  bb_params_get_int(params, PARAM_DASH_TYPE, &local_error[PARAM_DASH_TYPE]),
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
            g_clear_object(&line);
            break;
        }
    }

    for (int index=0; index < N_PARAMETERS; index++)
    {
        g_clear_error(&local_error[index]);
    }

    return line;
}


static void
bb_geda_line_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaLine *line = BB_GEDA_LINE(item);

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
bb_geda_line_set_cap_type(BbGedaLine *line, int type)
{
    g_return_if_fail(line != NULL);
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->cap_type != type)
    {
        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        line->line_style->cap_type = type;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_CAP_TYPE]);
    }
}


static void
bb_geda_line_set_dash_length(BbGedaLine *line, int length)
{
    g_return_if_fail(line != NULL);
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->dash_length != length)
    {
        line->line_style->dash_length = length;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_DASH_LENGTH]);
    }
}


static void
bb_geda_line_set_dash_space(BbGedaLine *line, int space)
{
    g_return_if_fail(line != NULL);
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->dash_space != space)
    {
        line->line_style->dash_space = space;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_DASH_SPACE]);
    }
}


static void
bb_geda_line_set_dash_type(BbGedaLine *line, int type)
{
    g_return_if_fail(line != NULL);
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->dash_type != type)
    {
        line->line_style->dash_type = type;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_DASH_TYPE]);
    }
}


static void
bb_geda_line_set_item_color(BbGedaLine *line, int color)
{
    g_return_if_fail(BB_IS_GEDA_LINE(line));
    g_return_if_fail(bb_color_is_valid(color));

    if (line->color != color)
    {
        line->color = color;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_geda_line_set_line_width(BbGedaLine *line, int width)
{
    g_return_if_fail(BB_IS_GEDA_LINE(line));
    g_return_if_fail(line->line_style != NULL);

    if (line->line_style->line_width != width)
    {
        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        line->line_style->line_width = width;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_LINE_WIDTH]);
    }
}


static void
bb_geda_line_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAP_TYPE:
            bb_geda_line_set_cap_type(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_geda_line_set_item_color(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        case PROP_DASH_LENGTH:
            bb_geda_line_set_dash_length(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        case PROP_DASH_SPACE:
            bb_geda_line_set_dash_space(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        case PROP_DASH_TYPE:
            bb_geda_line_set_dash_type(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        case PROP_LINE_WIDTH:
            bb_geda_line_set_line_width(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        case PROP_X0:
            bb_geda_line_set_x0(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        case PROP_X1:
            bb_geda_line_set_x1(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        case PROP_Y0:
            bb_geda_line_set_y0(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        case PROP_Y1:
            bb_geda_line_set_y1(BB_GEDA_LINE(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_geda_line_set_x0(BbGedaLine *line, int x)
{
    g_return_if_fail(BB_IS_GEDA_LINE(line));

    if (line->x[0] != x)
    {
        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        line->x[0] = x;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_X0]);
    }
}


void
bb_geda_line_set_x1(BbGedaLine *line, int x)
{
    g_return_if_fail(BB_IS_GEDA_LINE(line));

    if (line->x[1] != x)
    {
        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        line->x[1] = x;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_X1]);
    }
}


void
bb_geda_line_set_y0(BbGedaLine *line, int y)
{
    g_return_if_fail(BB_IS_GEDA_LINE(line));

    if (line->y[0] != y)
    {
        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        line->y[0] = y;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_Y0]);
    }
}


void
bb_geda_line_set_y1(BbGedaLine *line, int y)
{
    g_return_if_fail(BB_IS_GEDA_LINE(line));

    if (line->y[1] != y)
    {
        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        line->y[1] = y;

        g_signal_emit(line, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_Y1]);
    };
}


static void
bb_geda_line_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaLine *line = BB_GEDA_LINE(item);
    g_return_if_fail(line != NULL);

    bb_coord_translate(dx, dy, line->x, line->y, 2);

    g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_X0]);
    g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_Y0]);
    g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_X1]);
    g_object_notify_by_pspec(G_OBJECT(line), properties[PROP_Y1]);
}


static gboolean
bb_geda_line_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error)
{
    BbGedaLine *line = BB_GEDA_LINE(item);
    g_return_val_if_fail(line != NULL, FALSE);

    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d %d %d %d %d %d\n",
        BB_GEDA_LINE_TOKEN,
        line->x[0],
        line->y[0],
        line->x[1],
        line->y[1],
        line->color,
        line->line_style->line_width,
        line->line_style->cap_type,
        line->line_style->dash_type,
        bb_line_style_get_dash_length_for_file(line->line_style),
        bb_line_style_get_dash_space_for_file(line->line_style)
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
bb_geda_line_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGedaLine *line = BB_GEDA_LINE(item);

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
bb_geda_line_write_finish(
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
