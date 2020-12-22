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
#include "bbgedanet.h"
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbadjustableitemcolor.h"
#include "bbcolors.h"
#include "bbparams.h"
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

    N_PARAMETERS
};


enum
{
    PROP_0,

    PROP_X0,
    PROP_Y0,
    PROP_X1,
    PROP_Y1,

    /* From AdjustableItemColor */
    PROP_ITEM_COLOR,

    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbGedaNet
{
    BbGedaItem parent;

    BbItemParams *params;

    int x[2];
    int y[2];

    int color;
};


static void
bb_geda_net_adjustable_item_color_init(BbAdjustableItemColorInterface *iface);

static BbBounds*
bb_geda_net_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator);

static BbGedaItem*
bb_geda_net_clone(BbGedaItem *item);

static void
bb_geda_net_dispose(GObject *object);

static void
bb_geda_net_finalize(GObject *object);

static int
bb_geda_net_get_item_color(BbGedaNet *net);

static void
bb_geda_net_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_net_render(BbGedaItem *item, BbItemRenderer *renderer);

static void
bb_geda_net_set_item_color(BbGedaNet *net, int color);

static void
bb_geda_net_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_geda_net_translate(BbGedaItem *item, int dx, int dy);

static gboolean
bb_geda_net_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error);

static void
bb_geda_net_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_net_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_CODE(
    BbGedaNet,
    bb_geda_net,
    BB_TYPE_GEDA_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_geda_net_adjustable_item_color_init)
    )


static void
bb_geda_net_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static BbBounds*
bb_geda_net_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaNet *net = BB_GEDA_NET(item);

    g_return_val_if_fail(net != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        net->x[0],
        net->y[0],
        net->x[1],
        net->y[1],
        BB_GEDA_NET_WIDTH
        );
}


static void
bb_geda_net_class_init(BbGedaNetClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_geda_net_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_geda_net_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_geda_net_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_geda_net_set_property;

    BB_GEDA_ITEM_CLASS(klasse)->calculate_bounds = bb_geda_net_calculate_bounds;
    BB_GEDA_ITEM_CLASS(klasse)->clone = bb_geda_net_clone;
    BB_GEDA_ITEM_CLASS(klasse)->render = bb_geda_net_render;
    BB_GEDA_ITEM_CLASS(klasse)->translate = bb_geda_net_translate;
    BB_GEDA_ITEM_CLASS(klasse)->write = bb_geda_net_write;
    BB_GEDA_ITEM_CLASS(klasse)->write_async = bb_geda_net_write_async;
    BB_GEDA_ITEM_CLASS(klasse)->write_finish = bb_geda_net_write_finish;

    /* From AdjustableItemColor */
    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_ITEM_COLOR,
        "item-color"
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
bb_geda_net_clone(BbGedaItem *item)
{
    return BB_GEDA_ITEM(g_object_new(
        BB_TYPE_GEDA_NET,
        "x0", bb_geda_net_get_x0(BB_GEDA_NET(item)),
        "x1", bb_geda_net_get_x1(BB_GEDA_NET(item)),
        "y0", bb_geda_net_get_y0(BB_GEDA_NET(item)),
        "y1", bb_geda_net_get_y1(BB_GEDA_NET(item)),

        /* From AdjustableItemColor */
        "item-color", bb_geda_net_get_item_color(BB_GEDA_NET(item)),

        NULL
    ));
}


static void
bb_geda_net_dispose(GObject *object)
{
}


static void
bb_geda_net_finalize(GObject *object)
{
    BbGedaNet *net = BB_GEDA_NET(object);

    g_return_if_fail(net != NULL);
}


static int
bb_geda_net_get_item_color(BbGedaNet *net)
{
    g_return_val_if_fail(net != NULL, 0);

    return net->color;
}


static void
bb_geda_net_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_geda_net_get_item_color(BB_GEDA_NET(object)));
            break;

        case PROP_X0:
            g_value_set_int(value, bb_geda_net_get_x0(BB_GEDA_NET(object)));
            break;

        case PROP_X1:
            g_value_set_int(value, bb_geda_net_get_x1(BB_GEDA_NET(object)));
            break;

        case PROP_Y0:
            g_value_set_int(value, bb_geda_net_get_y0(BB_GEDA_NET(object)));
            break;

        case PROP_Y1:
            g_value_set_int(value, bb_geda_net_get_y1(BB_GEDA_NET(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_geda_net_get_x0(BbGedaNet *net)
{
    g_return_val_if_fail(net != NULL, 0);

    return net->x[0];
}


int
bb_geda_net_get_x1(BbGedaNet *net)
{
    g_return_val_if_fail(net != NULL, 0);

    return net->x[1];
}


int
bb_geda_net_get_y0(BbGedaNet *net)
{
    g_return_val_if_fail(net != NULL, 0);

    return net->y[0];
}


int
bb_geda_net_get_y1(BbGedaNet *net)
{
    g_return_val_if_fail(net != NULL, 0);

    return net->y[1];
}


static void
bb_geda_net_init(BbGedaNet *net)
{
    g_return_if_fail(net != NULL);

    bb_geda_net_set_item_color(net, BB_COLOR_NET);
}


BbGedaNet*
bb_geda_net_new()
{
    return g_object_new(BB_TYPE_GEDA_NET, NULL);
}


BbGedaNet*
bb_geda_net_new_with_params(BbParams *params, GError **error)
{
    GError *local_error[N_PARAMETERS] = { NULL };

    g_return_val_if_fail(bb_params_token_matches(params, BB_GEDA_NET_TOKEN), NULL);

    BbGedaNet *net = BB_GEDA_NET(g_object_new(
        BB_TYPE_GEDA_NET,
        "x0", bb_params_get_int(params, PARAM_X0, &local_error[PARAM_X0]),
        "y0", bb_params_get_int(params, PARAM_Y0, &local_error[PARAM_Y0]),
        "x1", bb_params_get_int(params, PARAM_X1, &local_error[PARAM_X1]),
        "y1", bb_params_get_int(params, PARAM_Y1, &local_error[PARAM_Y1]),

        "item-color", bb_params_get_int(params, PARAM_COLOR, &local_error[PARAM_COLOR]),

        NULL
    ));

    for (int index=0; index < N_PARAMETERS; index++)
    {
        if (local_error[index] != NULL)
        {
            g_propagate_error(error, local_error[index]);
            local_error[index] = NULL;
            g_clear_object(&net);
            break;
        }
    }

    for (int index=0; index < N_PARAMETERS; index++)
    {
        g_clear_error(&local_error[index]);
    }

    return net;
}


static void
bb_geda_net_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaNet *net = BB_GEDA_NET(item);

    g_return_if_fail(net != NULL);

    bb_item_renderer_set_color(renderer, net->color);

    bb_item_renderer_render_absolute_move_to(
        renderer,
        net->x[0],
        net->y[0]
        );

    bb_item_renderer_render_absolute_line_to(
        renderer,
        net->x[1],
        net->y[1]
        );
}


static void
bb_geda_net_set_item_color(BbGedaNet *net, int color)
{
    g_return_if_fail(BB_IS_GEDA_NET(net));
    g_return_if_fail(bb_color_is_valid(color));

    if (net->color != color)
    {
        net->color = color;

        g_signal_emit(net, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(net), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_geda_net_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM_COLOR:
            bb_geda_net_set_item_color(BB_GEDA_NET(object), g_value_get_int(value));
            break;

        case PROP_X0:
            bb_geda_net_set_x0(BB_GEDA_NET(object), g_value_get_int(value));
            break;

        case PROP_X1:
            bb_geda_net_set_x1(BB_GEDA_NET(object), g_value_get_int(value));
            break;

        case PROP_Y0:
            bb_geda_net_set_y0(BB_GEDA_NET(object), g_value_get_int(value));
            break;

        case PROP_Y1:
            bb_geda_net_set_y1(BB_GEDA_NET(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_geda_net_set_x0(BbGedaNet *net, int x)
{
    g_return_if_fail(net != NULL);

    if (net->x[0] != x)
    {
        g_signal_emit(net, signals[SIG_INVALIDATE], 0);

        net->x[0] = x;

        g_signal_emit(net, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(net), properties[PROP_X0]);
    }
}


void
bb_geda_net_set_x1(BbGedaNet *net, int x)
{
    g_return_if_fail(net != NULL);

    if (net->x[1] != x)
    {
        g_signal_emit(net, signals[SIG_INVALIDATE], 0);

        net->x[1] = x;

        g_signal_emit(net, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(net), properties[PROP_X1]);
    }
}


void
bb_geda_net_set_y0(BbGedaNet *net, int y)
{
    g_return_if_fail(net != NULL);

    if (net->y[0] != y)
    {
        g_signal_emit(net, signals[SIG_INVALIDATE], 0);

        net->y[0] = y;

        g_signal_emit(net, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(net), properties[PROP_Y0]);
    }
}


void
bb_geda_net_set_y1(BbGedaNet *net, int y)
{
    g_return_if_fail(net != NULL);

    if (net->y[1] != y)
    {
        g_signal_emit(net, signals[SIG_INVALIDATE], 0);

        net->y[1] = y;

        g_signal_emit(net, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(net), properties[PROP_Y1]);
    };
}


static void
bb_geda_net_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaNet *net = BB_GEDA_NET(item);
    g_return_if_fail(net != NULL);

    bb_coord_translate(dx, dy, net->x, net->y, 2);

    g_object_notify_by_pspec(G_OBJECT(net), properties[PROP_X0]);
    g_object_notify_by_pspec(G_OBJECT(net), properties[PROP_Y0]);
    g_object_notify_by_pspec(G_OBJECT(net), properties[PROP_X1]);
    g_object_notify_by_pspec(G_OBJECT(net), properties[PROP_Y1]);
}


static gboolean
bb_geda_net_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error)
{
    BbGedaNet *net = BB_GEDA_NET(item);
    g_return_val_if_fail(net != NULL, FALSE);

    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d\n",
        BB_GEDA_NET_TOKEN,
        net->x[0],
        net->y[0],
        net->x[1],
        net->y[1],
        net->color
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
bb_geda_net_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGedaNet *net = BB_GEDA_NET(item);

    bb_item_params_write_async(
        net->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_geda_net_write_finish(
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
