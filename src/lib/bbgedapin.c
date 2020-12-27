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
#include "bbgedapin.h"
#include "bbcoord.h"
#include "bbadjustableitemcolor.h"
#include "bbgedanet.h"
#include "bbgedabus.h"
#include "bbpintype.h"
#include "bbcolors.h"
#include "bbparams.h"
#include "bbcolor.h"
#include "bbelectrical.h"


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
    PROP_PIN_TYPE,
    
    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbGedaPin
{
    BbGedaItem parent;

    int x[2];
    int y[2];

    int color;

    BbPinEnd pin_end;
    BbPinType pin_type;

    GSList *attributes;
};


static void
bb_geda_pin_add_attribute(
    BbElectrical *electrical,
    BbAttribute *attribute
    );

static void
bb_geda_pin_adjustable_item_color_init(
    BbAdjustableItemColorInterface *iface
    );

static BbBounds*
bb_geda_pin_calculate_bounds(
    BbGedaItem *item,
    BbBoundsCalculator *calculator
    );

static BbGedaItem*
bb_geda_pin_clone(
    BbGedaItem *item
    );

static void
bb_geda_pin_dispose(
    GObject *object
    );

static void
bb_geda_pin_electrical_init(
    BbElectricalInterface *iface
    );

static void
bb_geda_pin_finalize(
    GObject *object
    );

static void
bb_geda_pin_foreach(
    BbElectrical *electrical,
    GFunc func,
    gpointer user_data
    );

static int
bb_geda_pin_get_item_color(
    BbGedaPin *pin
    );

static int
bb_geda_pin_get_pin_width(
    BbGedaPin *pin
    );

static void
bb_geda_pin_get_property(
    GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_pin_render(
    BbGedaItem *item,
    BbItemRenderer *renderer
    );

static void
bb_geda_pin_set_item_color(
    BbGedaPin *pin,
    int color
    );

static void
bb_geda_pin_set_property(
    GObject *object,
    guint property_id,
    const GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_pin_translate(
    BbGedaItem *item,
    int dx,
    int dy
    );

static gboolean
bb_geda_pin_write(
    BbGedaItem *item,
    GOutputStream *stream,
    GCancellable *cancellable,
    GError **error
    );

static void
bb_geda_pin_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_pin_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_CODE(
    BbGedaPin,
    bb_geda_pin,
    BB_TYPE_GEDA_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_geda_pin_adjustable_item_color_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ELECTRICAL, bb_geda_pin_electrical_init)
    )


static void
bb_geda_pin_add_attribute(BbElectrical *electrical, BbAttribute *attribute)
{
    BbGedaPin *pin_item = BB_GEDA_PIN(electrical);

    g_return_if_fail(BB_IS_GEDA_PIN(pin_item));
    g_return_if_fail(BB_IS_ATTRIBUTE(attribute));

    g_object_ref(attribute);

    pin_item->attributes = g_slist_append(pin_item->attributes, attribute);
}


static void
bb_geda_pin_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static BbBounds*
bb_geda_pin_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaPin *pin = BB_GEDA_PIN(item);

    g_return_val_if_fail(pin != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        pin->x[0],
        pin->y[0],
        pin->x[1],
        pin->y[1],
        bb_geda_pin_get_pin_width(pin)
        );
}


static void
bb_geda_pin_class_init(BbGedaPinClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(G_IS_OBJECT_CLASS(object_class));

    object_class->dispose = bb_geda_pin_dispose;
    object_class->finalize = bb_geda_pin_finalize;
    object_class->get_property = bb_geda_pin_get_property;
    object_class->set_property = bb_geda_pin_set_property;

    BbGedaItemClass *item_class = BB_GEDA_ITEM_CLASS(klasse);
    g_return_if_fail(BB_IS_GEDA_ITEM_CLASS(item_class));

    item_class->calculate_bounds = bb_geda_pin_calculate_bounds;
    item_class->clone = bb_geda_pin_clone;
    item_class->render = bb_geda_pin_render;
    item_class->translate = bb_geda_pin_translate;
    item_class->write = bb_geda_pin_write;
    item_class->write_async = bb_geda_pin_write_async;
    item_class->write_finish = bb_geda_pin_write_finish;

    /* From AdjustableItemColor */
    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        object_class,
        PROP_ITEM_COLOR,
        "item-color"
        );

    properties[PROP_X0] = bb_object_class_install_property(
        object_class,
        PROP_X0,
        g_param_spec_int(
            "x0",
            "x0",
            "The x coordinate of the first endpoint",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_X1] = bb_object_class_install_property(
        object_class,
        PROP_X1,
        g_param_spec_int(
            "x1",
            "x1",
            "The x coordinate of the second endpoint",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_Y0] = bb_object_class_install_property(
        object_class,
        PROP_Y0,
        g_param_spec_int(
            "y0",
            "y0",
            "The y coordinate of the first endpoint",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_Y1] = bb_object_class_install_property(
        object_class,
        PROP_Y1,
        g_param_spec_int(
            "y1",
            "y1",
            "The y coordinate of the second endpoint",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_PIN_END] = bb_object_class_install_property(
        object_class,
        PROP_PIN_END,
        g_param_spec_int(
            "pin-end",
            "",
            "",
            BB_PIN_END_MIN,
            BB_PIN_END_MAX,
            BB_PIN_END_DEFAULT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_PIN_TYPE] = bb_object_class_install_property(
        object_class,
        PROP_PIN_TYPE,
        g_param_spec_int(
            "pin-type",
            "",
            "",
            0,
            N_PIN_TYPES - 1,
            BB_PIN_TYPE_DEFAULT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_GEDA_ITEM);
}


static BbGedaItem*
bb_geda_pin_clone(BbGedaItem *item)
{
    return BB_GEDA_ITEM(g_object_new(
        BB_TYPE_GEDA_PIN,

        /* From AdjustableItemColor */
        "item-color", bb_geda_pin_get_item_color(BB_GEDA_PIN(item)),

        /* From GedaPin */
        "x0", bb_geda_pin_get_x0(BB_GEDA_PIN(item)),
        "x1", bb_geda_pin_get_x1(BB_GEDA_PIN(item)),
        "y0", bb_geda_pin_get_y0(BB_GEDA_PIN(item)),
        "y1", bb_geda_pin_get_y1(BB_GEDA_PIN(item)),
        "pin-end", bb_geda_pin_get_pin_end(BB_GEDA_PIN(item)),
        "pin-type", bb_geda_pin_get_pin_type(BB_GEDA_PIN(item)),

        NULL
    ));
}


static void
bb_geda_pin_dispose(GObject *object)
{
    BbGedaPin *pin_item = BB_GEDA_PIN(object);
    g_return_if_fail(BB_IS_GEDA_PIN(pin_item));

    g_slist_free_full(g_steal_pointer(&pin_item->attributes), g_object_unref);
}


static void
bb_geda_pin_electrical_init(BbElectricalInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->add_attribute = bb_geda_pin_add_attribute;
    iface->foreach = bb_geda_pin_foreach;
}


static void
bb_geda_pin_finalize(GObject *object)
{
    BbGedaPin *pin = BB_GEDA_PIN(object);

    g_return_if_fail(pin != NULL);
}


static void
bb_geda_pin_foreach(BbElectrical *electrical, GFunc func, gpointer user_data)
{
    BbGedaPin *pin_item = BB_GEDA_PIN(electrical);

    g_return_if_fail(BB_IS_GEDA_PIN(pin_item));
    g_return_if_fail(func != NULL);

    g_slist_foreach(pin_item->attributes, func, user_data);
}


static int
bb_geda_pin_get_item_color(BbGedaPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->color;
}


BbPinEnd
bb_geda_pin_get_pin_end(BbGedaPin *pin)
{
    g_return_val_if_fail(BB_IS_GEDA_PIN(pin), BB_PIN_END_DEFAULT);

    return pin->pin_end;
}


BbPinType
bb_geda_pin_get_pin_type(BbGedaPin *pin)
{
    g_return_val_if_fail(BB_IS_GEDA_PIN(pin), BB_PIN_TYPE_DEFAULT);

    return pin->pin_type;
}


static int
bb_geda_pin_get_pin_width(BbGedaPin *pin)
{
    switch (pin->pin_type)
    {
        case BB_PIN_TYPE_NET:
            return BB_GEDA_NET_WIDTH;

        case BB_PIN_TYPE_BUS:
            return BB_GEDA_BUS_WIDTH;

        default:
            g_return_val_if_reached(BB_GEDA_NET_WIDTH);
    }
}


static void
bb_geda_pin_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_geda_pin_get_item_color(BB_GEDA_PIN(object)));
            break;

        case PROP_PIN_END:
            g_value_set_int(value, bb_geda_pin_get_pin_end(BB_GEDA_PIN(object)));
            break;

        case PROP_PIN_TYPE:
            g_value_set_int(value, bb_geda_pin_get_pin_type(BB_GEDA_PIN(object)));
            break;

        case PROP_X0:
            g_value_set_int(value, bb_geda_pin_get_x0(BB_GEDA_PIN(object)));
            break;

        case PROP_X1:
            g_value_set_int(value, bb_geda_pin_get_x1(BB_GEDA_PIN(object)));
            break;

        case PROP_Y0:
            g_value_set_int(value, bb_geda_pin_get_y0(BB_GEDA_PIN(object)));
            break;

        case PROP_Y1:
            g_value_set_int(value, bb_geda_pin_get_y1(BB_GEDA_PIN(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_geda_pin_get_x0(BbGedaPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->x[0];
}


int
bb_geda_pin_get_x1(BbGedaPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->x[1];
}


int
bb_geda_pin_get_y0(BbGedaPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->y[0];
}


int
bb_geda_pin_get_y1(BbGedaPin *pin)
{
    g_return_val_if_fail(pin != NULL, 0);

    return pin->y[1];
}


static void
bb_geda_pin_init(BbGedaPin *pin)
{
    g_return_if_fail(pin != NULL);

    bb_geda_pin_set_item_color(pin, BB_COLOR_PIN);
}


BbGedaPin*
bb_geda_pin_new()
{
    return g_object_new(BB_TYPE_GEDA_PIN, NULL);
}


BbGedaPin*
bb_geda_pin_new_with_params(BbParams *params, GError **error)
{
    GError *local_error = NULL;

    g_return_val_if_fail(bb_params_token_matches(params, BB_GEDA_PIN_TOKEN), NULL);

    int color;
    BbGedaPin *pin = NULL;
    int pin_end;
    BbPinType pin_type;
    int x[2];
    int y[2];

    x[0] = bb_params_get_int(params, PARAM_X0, &local_error);

    if (local_error == NULL)
    {
        y[0] = bb_params_get_int(params, PARAM_Y0, &local_error);
    }

    if (local_error == NULL)
    {
        x[1] = bb_params_get_int(params, PARAM_X1, &local_error);
    }

    if (local_error == NULL)
    {
        y[1] = bb_params_get_int(params, PARAM_Y1, &local_error);
    }

    if (local_error == NULL)
    {
        color = bb_text_color_from_params(params, PARAM_COLOR, BB_COLOR_PIN, &local_error);
    }

    if (local_error == NULL)
    {
        pin_end = bb_pin_end_from_params(params, PARAM_PIN_END, &local_error);
    }

    if (local_error == NULL)
    {
        pin_type = bb_pin_type_from_params(params, PARAM_PIN_TYPE, &local_error);
    }

    if (local_error == NULL)
    {
        pin = BB_GEDA_PIN(g_object_new(
            BB_TYPE_GEDA_PIN,
            "x0", x[0],
            "y0", y[0],
            "x1", x[1],
            "y1", y[1],
            "item-color", color,
            "pin-type", pin_type,
            "pin-end", pin_end,
            NULL
            ));
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        g_clear_object(&pin);
    }

    return pin;
}


static void
bb_geda_pin_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaPin *pin = BB_GEDA_PIN(item);

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
bb_geda_pin_set_item_color(BbGedaPin *pin, int color)
{
    g_return_if_fail(BB_IS_GEDA_PIN(pin));
    g_return_if_fail(bb_color_is_valid(color));

    if (pin->color != color)
    {
        pin->color = color;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_ITEM_COLOR]);
    }
}


void
bb_geda_pin_set_pin_end(BbGedaPin *pin, BbPinEnd pin_end)
{
    g_return_if_fail(BB_IS_GEDA_PIN(pin));
    g_return_if_fail(bb_pin_end_is_valid(pin_end));

    if (pin->pin_end != pin_end)
    {
        pin->pin_end = pin_end;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_PIN_END]);
    }
}


void
bb_geda_pin_set_pin_type(BbGedaPin *pin, BbPinType pin_type)
{
    g_return_if_fail(BB_IS_GEDA_PIN(pin));
    g_return_if_fail(bb_pin_type_is_valid(pin_type));

    if (pin->pin_type != pin_type)
    {
        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        pin->pin_type = pin_type;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_PIN_TYPE]);
    }
}


static void
bb_geda_pin_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM_COLOR:
            bb_geda_pin_set_item_color(BB_GEDA_PIN(object), g_value_get_int(value));
            break;

        case PROP_PIN_END:
            bb_geda_pin_set_pin_end(BB_GEDA_PIN(object), g_value_get_int(value));
            break;

        case PROP_PIN_TYPE:
            bb_geda_pin_set_pin_type(BB_GEDA_PIN(object), g_value_get_int(value));
            break;

        case PROP_X0:
            bb_geda_pin_set_x0(BB_GEDA_PIN(object), g_value_get_int(value));
            break;

        case PROP_X1:
            bb_geda_pin_set_x1(BB_GEDA_PIN(object), g_value_get_int(value));
            break;

        case PROP_Y0:
            bb_geda_pin_set_y0(BB_GEDA_PIN(object), g_value_get_int(value));
            break;

        case PROP_Y1:
            bb_geda_pin_set_y1(BB_GEDA_PIN(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_geda_pin_set_x0(BbGedaPin *pin, int x)
{
    g_return_if_fail(BB_IS_GEDA_PIN(pin));

    if (pin->x[0] != x)
    {
        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        pin->x[0] = x;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_X0]);
    }
}


void
bb_geda_pin_set_x1(BbGedaPin *pin, int x)
{
    g_return_if_fail(BB_IS_GEDA_PIN(pin));

    if (pin->x[1] != x)
    {
        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        pin->x[1] = x;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_X1]);
    }
}


void
bb_geda_pin_set_y0(BbGedaPin *pin, int y)
{
    g_return_if_fail(BB_IS_GEDA_PIN(pin));

    if (pin->y[0] != y)
    {
        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        pin->y[0] = y;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_Y0]);
    }
}


void
bb_geda_pin_set_y1(BbGedaPin *pin, int y)
{
    g_return_if_fail(BB_IS_GEDA_PIN(pin));

    if (pin->y[1] != y)
    {
        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        pin->y[1] = y;

        g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_Y1]);
    };
}


static void
bb_geda_pin_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaPin *pin = BB_GEDA_PIN(item);
    g_return_if_fail(pin != NULL);

    g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

    bb_coord_translate(dx, dy, pin->x, pin->y, 2);

    g_signal_emit(pin, signals[SIG_INVALIDATE], 0);

    g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_X0]);
    g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_Y0]);
    g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_X1]);
    g_object_notify_by_pspec(G_OBJECT(pin), properties[PROP_Y1]);
}


static gboolean
bb_geda_pin_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error)
{
    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d %d %d\n",
        BB_GEDA_PIN_TOKEN,
        bb_geda_pin_get_x0(BB_GEDA_PIN(item)),
        bb_geda_pin_get_y0(BB_GEDA_PIN(item)),
        bb_geda_pin_get_x1(BB_GEDA_PIN(item)),
        bb_geda_pin_get_y1(BB_GEDA_PIN(item)),
        bb_geda_pin_get_item_color(BB_GEDA_PIN(item)),
        bb_geda_pin_get_pin_type(BB_GEDA_PIN(item)),
        bb_geda_pin_get_pin_end(BB_GEDA_PIN(item))
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
bb_geda_pin_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGedaPin *pin = BB_GEDA_PIN(item);

//    bb_item_params_write_async(
//        pin->params,
//        stream,
//        io_priority,
//        cancellable,
//        callback,
//        callback_data
//        );
}


static gboolean
bb_geda_pin_write_finish(
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
