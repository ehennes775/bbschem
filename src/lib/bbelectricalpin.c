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
#include "bbelectricalpin.h"
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbadjustableitemcolor.h"
#include "bbelectricalnet.h"
#include "bbelectricalbus.h"
#include "bbpintype.h"
#include "bbcolors.h"
#include "bbparams.h"


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

    PARAM_PIN_TYPE,
    PARAM_PIN_END,

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

    PROP_PIN_END,

    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbElectricalPin
{
    BbSchematicItem parent;

    BbItemParams *params;

    int x[2];
    int y[2];

    int color;

    BbPinType type;

    int end;
};


static void
bb_electrical_pin_adjustable_item_color_init(BbAdjustableItemColorInterface *iface);

static BbBounds*
bb_electrical_pin_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static BbSchematicItem*
bb_electrical_pin_clone(BbSchematicItem *item);

static void
bb_electrical_pin_dispose(GObject *object);

static void
bb_electrical_pin_finalize(GObject *object);

static int
bb_electrical_pin_get_item_color(BbElectricalPin *pin);

static int
bb_electrical_pin_get_pin_width(BbElectricalPin *pin);

static void
bb_electrical_pin_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_electrical_pin_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_electrical_pin_set_item_color(BbElectricalPin *pin, int color);

static void
bb_electrical_pin_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_electrical_pin_translate(BbSchematicItem *item, int dx, int dy);

static gboolean
bb_electrical_pin_write(BbSchematicItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error);

static void
bb_electrical_pin_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_electrical_pin_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_CODE(
    BbElectricalPin,
    bb_electrical_pin,
    BB_TYPE_SCHEMATIC_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_electrical_pin_adjustable_item_color_init)
    )


static void
bb_electrical_pin_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static BbBounds*
bb_electrical_pin_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator)
{
    BbElectricalPin *pin = BB_ELECTRICAL_PIN(item);

    g_return_val_if_fail(pin != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        pin->x[0],
        pin->y[0],
        pin->x[1],
        pin->y[1],
        bb_electrical_pin_get_pin_width(pin)
        );
}


static void
bb_electrical_pin_class_init(BbElectricalPinClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_electrical_pin_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_electrical_pin_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_electrical_pin_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_electrical_pin_set_property;

    BB_SCHEMATIC_ITEM_CLASS(klasse)->calculate_bounds = bb_electrical_pin_calculate_bounds;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->clone = bb_electrical_pin_clone;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->render = bb_electrical_pin_render;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->translate = bb_electrical_pin_translate;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write = bb_electrical_pin_write;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_async = bb_electrical_pin_write_async;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_finish = bb_electrical_pin_write_finish;

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

    properties[PROP_PIN_END] = bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_PIN_END,
        g_param_spec_int(
            "pin-end",
            "",
            "",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_SCHEMATIC_ITEM);
}


static BbSchematicItem*
bb_electrical_pin_clone(BbSchematicItem *item)
{
    return BB_SCHEMATIC_ITEM(g_object_new(
        BB_TYPE_ELECTRICAL_PIN,
        "x0", bb_electrical_pin_get_x0(BB_ELECTRICAL_PIN(item)),
        "x1", bb_electrical_pin_get_x1(BB_ELECTRICAL_PIN(item)),
        "y0", bb_electrical_pin_get_y0(BB_ELECTRICAL_PIN(item)),
        "y1", bb_electrical_pin_get_y1(BB_ELECTRICAL_PIN(item)),

        /* From AdjustableItemColor */
        "item-color", bb_electrical_pin_get_item_color(BB_ELECTRICAL_PIN(item)),

        NULL
    ));
}


static void
bb_electrical_pin_dispose(GObject *object)
{
}


static void
bb_electrical_pin_finalize(GObject *object)
{
    BbElectricalPin *pin = BB_ELECTRICAL_PIN(object);

    g_return_if_fail(pin != NULL);
}


static int
bb_electrical_pin_get_item_color(BbElectricalPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->color;
}


static int
bb_electrical_pin_get_pin_width(BbElectricalPin *pin)
{
    switch (pin->type)
    {
        case BB_PIN_TYPE_NET:
            return BB_ELECTRICAL_NET_WIDTH;

        case BB_PIN_TYPE_BUS:
            return BB_ELECTRICAL_BUS_WIDTH;

        default:
            g_return_val_if_reached(BB_ELECTRICAL_NET_WIDTH);
    }
}


static void
bb_electrical_pin_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_electrical_pin_get_item_color(BB_ELECTRICAL_PIN(object)));
            break;

        case PROP_X0:
            g_value_set_int(value, bb_electrical_pin_get_x0(BB_ELECTRICAL_PIN(object)));
            break;

        case PROP_X1:
            g_value_set_int(value, bb_electrical_pin_get_x1(BB_ELECTRICAL_PIN(object)));
            break;

        case PROP_Y0:
            g_value_set_int(value, bb_electrical_pin_get_y0(BB_ELECTRICAL_PIN(object)));
            break;

        case PROP_Y1:
            g_value_set_int(value, bb_electrical_pin_get_y1(BB_ELECTRICAL_PIN(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_electrical_pin_get_x0(BbElectricalPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->x[0];
}


int
bb_electrical_pin_get_x1(BbElectricalPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->x[1];
}


int
bb_electrical_pin_get_y0(BbElectricalPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->y[0];
}


int
bb_electrical_pin_get_y1(BbElectricalPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->y[1];
}


static void
bb_electrical_pin_init(BbElectricalPin *pin)
{
    g_return_if_fail(pin != NULL);

    bb_electrical_pin_set_item_color(pin, BB_COLOR_PIN);
}


BbElectricalPin*
bb_electrical_pin_new()
{
    return g_object_new(BB_TYPE_ELECTRICAL_PIN, NULL);
}


BbElectricalPin*
bb_electrical_pin_new_with_params(BbParams *params, GError **error)
{
    GError *local_error[N_PARAMETERS] = { NULL };

    g_return_val_if_fail(bb_params_token_matches(params, BB_ELECTRICAL_PIN_TOKEN), NULL);

    BbElectricalPin *pin = BB_ELECTRICAL_PIN(g_object_new(
        BB_TYPE_ELECTRICAL_PIN,
        "x0", bb_params_get_int(params, PARAM_X0, &local_error[PARAM_X0]),
        "y0", bb_params_get_int(params, PARAM_Y0, &local_error[PARAM_Y0]),
        "x1", bb_params_get_int(params, PARAM_X1, &local_error[PARAM_X1]),
        "y1", bb_params_get_int(params, PARAM_Y1, &local_error[PARAM_Y1]),

        "item-color", bb_params_get_int(params, PARAM_COLOR, &local_error[PARAM_COLOR]),

        "pin-type", bb_params_get_int(params, PARAM_PIN_TYPE, &local_error[PARAM_PIN_TYPE]),
        "pin-end", bb_params_get_int(params, PARAM_PIN_END, &local_error[PARAM_PIN_END]),

        NULL
        ));

    for (int index=0; index < N_PARAMETERS; index++)
    {
        if (local_error[index] != NULL)
        {
            g_propagate_error(error, local_error[index]);
            local_error[index] = NULL;
            g_clear_object(&pin);
            break;
        }
    }

    for (int index=0; index < N_PARAMETERS; index++)
    {
        g_clear_error(&local_error[index]);
    }

    return pin;
}


static void
bb_electrical_pin_render(BbSchematicItem *item, BbItemRenderer *renderer)
{
    BbElectricalPin *pin = BB_ELECTRICAL_PIN(item);

    g_return_if_fail(pin != NULL);

    bb_item_renderer_set_color(renderer, pin->color);

    bb_item_renderer_render_absolute_move_to(
        renderer,
        pin->x[0],
        pin->y[0]
        );

    bb_item_renderer_render_absolute_line_to(
        renderer,
        pin->x[1],
        pin->y[1]
        );
}


static void
bb_electrical_pin_set_item_color(BbElectricalPin *pin, int color)
{
    g_return_if_fail(pin != NULL);

    if (pin->color != color)
    {
        pin->color = color;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_electrical_pin_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM_COLOR:
            bb_electrical_pin_set_item_color(BB_ELECTRICAL_PIN(object), g_value_get_int(value));
            break;

        case PROP_X0:
            bb_electrical_pin_set_x0(BB_ELECTRICAL_PIN(object), g_value_get_int(value));
            break;

        case PROP_X1:
            bb_electrical_pin_set_x1(BB_ELECTRICAL_PIN(object), g_value_get_int(value));
            break;

        case PROP_Y0:
            bb_electrical_pin_set_y0(BB_ELECTRICAL_PIN(object), g_value_get_int(value));
            break;

        case PROP_Y1:
            bb_electrical_pin_set_y1(BB_ELECTRICAL_PIN(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_electrical_pin_set_x0(BbElectricalPin *pin, int x)
{
    g_return_if_fail(pin != NULL);

    if (pin->x[0] != x)
    {
        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        pin->x[0] = x;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_X0]);
    }
}


void
bb_electrical_pin_set_x1(BbElectricalPin *pin, int x)
{
    g_return_if_fail(pin != NULL);

    if (pin->x[1] != x)
    {
        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        pin->x[1] = x;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_X1]);
    }
}


void
bb_electrical_pin_set_y0(BbElectricalPin *pin, int y)
{
    g_return_if_fail(pin != NULL);

    if (pin->y[0] != y)
    {
        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        pin->y[0] = y;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_Y0]);
    }
}


void
bb_electrical_pin_set_y1(BbElectricalPin *pin, int y)
{
    g_return_if_fail(pin != NULL);

    if (pin->y[1] != y)
    {
        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        pin->y[1] = y;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_Y1]);
    };
}


static void
bb_electrical_pin_translate(BbSchematicItem *item, int dx, int dy)
{
    BbElectricalPin *pin = BB_ELECTRICAL_PIN(item);
    g_return_if_fail(pin != NULL);

    bb_coord_translate(dx, dy, pin->x, pin->y, 2);

    g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_X0]);
    g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_Y0]);
    g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_X1]);
    g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_Y1]);
}


static gboolean
bb_electrical_pin_write(BbSchematicItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error)
{
    BbElectricalPin *pin = BB_ELECTRICAL_PIN(item);
    g_return_val_if_fail(pin != NULL, FALSE);

    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d %d %d\n",
        BB_ELECTRICAL_PIN_TOKEN,
        pin->x[0],
        pin->y[0],
        pin->x[1],
        pin->y[1],
        pin->color,
        pin->type,
        pin->end
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
bb_electrical_pin_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbElectricalPin *pin = BB_ELECTRICAL_PIN(item);

    bb_item_params_write_async(
        pin->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_electrical_pin_write_finish(
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
