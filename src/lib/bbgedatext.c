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
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbgedatext.h"
#include "bbadjustableitemcolor.h"


enum
{
    PROP_0,

    PROP_INSERT_X,
    PROP_INSERT_Y,

    /* From AdjustableItemColor */
    PROP_ITEM_COLOR,
    
    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbGedaText
{
    BbGedaItem parent;

    BbItemParams *params;

    int insert_x;
    int insert_y;

    int color;
};


static void
bb_geda_text_adjustable_item_color_init(BbAdjustableItemColorInterface *iface);

static BbBounds*
bb_geda_text_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator);

static BbGedaItem*
bb_geda_text_clone(BbGedaItem *item);

static void
bb_geda_text_dispose(GObject *object);

static void
bb_geda_text_finalize(GObject *object);

static int
bb_geda_text_get_item_color(BbGedaText *text);

static void
bb_geda_text_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_text_render(BbGedaItem *item, BbItemRenderer *renderer);

static void
bb_geda_text_set_item_color(BbGedaText *text, int color);

static void
bb_geda_text_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_geda_text_translate(BbGedaItem *item, int dx, int dy);

static void
bb_geda_text_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_text_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbGedaText,
    bb_geda_text,
    BB_TYPE_GEDA_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_geda_text_adjustable_item_color_init)
    )


static void
bb_geda_text_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static BbBounds*
bb_geda_text_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaText *text = BB_GEDA_TEXT(item);

    g_return_val_if_fail(text != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        text->insert_x,
        text->insert_y,
        text->insert_x,
        text->insert_y,
        0
        );
}


static void
bb_geda_text_class_init(BbGedaTextClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_geda_text_dispose;
    object_class->finalize = bb_geda_text_finalize;
    object_class->get_property = bb_geda_text_get_property;
    object_class->set_property = bb_geda_text_set_property;

    BB_GEDA_ITEM_CLASS(klasse)->calculate_bounds = bb_geda_text_calculate_bounds;
    BB_GEDA_ITEM_CLASS(klasse)->clone = bb_geda_text_clone;
    BB_GEDA_ITEM_CLASS(klasse)->render = bb_geda_text_render;
    BB_GEDA_ITEM_CLASS(klasse)->translate = bb_geda_text_translate;
    BB_GEDA_ITEM_CLASS(klasse)->write_async = bb_geda_text_write_async;
    BB_GEDA_ITEM_CLASS(klasse)->write_finish = bb_geda_text_write_finish;

    /* Properties from BbAdjustableItemColor */
    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        object_class,
        PROP_ITEM_COLOR,
        "item-color"
        );
    
    bb_object_class_install_property(
        object_class,
        PROP_INSERT_X,
        properties[PROP_INSERT_X] = g_param_spec_int(
            "insert-x",
            "Insertion Point X",
            "The x coordinate of the insertion point",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        object_class,
        PROP_INSERT_Y,
        properties[PROP_INSERT_Y] = g_param_spec_int(
            "insert-y",
            "Insertion Point Y",
            "The y coordinate of the insertion point",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE
            )
        );

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_GEDA_ITEM);
}


static BbGedaItem*
bb_geda_text_clone(BbGedaItem *item)
{
    return BB_GEDA_ITEM(g_object_new(
        BB_TYPE_GEDA_TEXT,
        "insert-x", bb_geda_text_get_insert_x(BB_GEDA_TEXT(item)),
        "insert-y", bb_geda_text_get_insert_y(BB_GEDA_TEXT(item)),

        /* From AdjustableItemColor */
        "item-color", bb_geda_text_get_item_color(BB_GEDA_TEXT(item)),

        NULL
        ));
}


static void
bb_geda_text_dispose(GObject *object)
{
}


static void
bb_geda_text_finalize(GObject *object)
{
}


int
bb_geda_text_get_insert_x(BbGedaText *text)
{
    g_return_val_if_fail(text != NULL, 0);

    return text->insert_x;
}


int
bb_geda_text_get_insert_y(BbGedaText *text)
{
    g_return_val_if_fail(text != NULL, 0);

    return text->insert_y;
}


static int
bb_geda_text_get_item_color(BbGedaText *text)
{
    g_return_val_if_fail(text != NULL, 0);

    return text->color;
}


static void
bb_geda_text_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_INSERT_X:
            g_value_set_int(value, bb_geda_text_get_insert_x(BB_GEDA_TEXT(object)));
            break;

        case PROP_INSERT_Y:
            g_value_set_int(value, bb_geda_text_get_insert_y(BB_GEDA_TEXT(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_geda_text_get_item_color(BB_GEDA_TEXT(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_text_init(BbGedaText *text)
{
    g_return_if_fail(text != NULL);
}


BbGedaText*
bb_geda_text_new()
{
    return g_object_new(BB_TYPE_GEDA_TEXT, NULL);
}


static void
bb_geda_text_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaText *text = BB_GEDA_TEXT(item);
    g_return_if_fail(text != NULL);
}


void
bb_geda_text_set_insert_x(BbGedaText *text, int x)
{
    g_return_if_fail(text != NULL);

    if (text->insert_x != x)
    {
        g_signal_emit(text, signals[SIG_INVALIDATE], 0);

        text->insert_x = x;

        g_signal_emit(text, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text), properties[PROP_INSERT_X]);
    }
}


void
bb_geda_text_set_insert_y(BbGedaText *text, int y)
{
    g_return_if_fail(text != NULL);

    if (text->insert_y != y)
    {
        g_signal_emit(text, signals[SIG_INVALIDATE], 0);

        text->insert_y = y;

        g_signal_emit(text, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text), properties[PROP_INSERT_Y]);
    }
}


static void
bb_geda_text_set_item_color(BbGedaText *text, int color)
{
    g_return_if_fail(text != NULL);

    if (text->color != color)
    {
        text->color = color;

        g_signal_emit(text, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_geda_text_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_INSERT_X:
            bb_geda_text_set_insert_x(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        case PROP_INSERT_Y:
            bb_geda_text_set_insert_y(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_geda_text_set_item_color(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_text_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaText *text = BB_GEDA_TEXT(item);
    g_return_if_fail(text != NULL);

    bb_coord_translate(dx, dy, &text->insert_x, &text->insert_y, 1);

    g_object_notify_by_pspec(G_OBJECT(text), properties[PROP_INSERT_X]);
    g_object_notify_by_pspec(G_OBJECT(text), properties[PROP_INSERT_Y]);
}


static void
bb_geda_text_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGedaText *text = BB_GEDA_TEXT(item);

    bb_item_params_write_async(
        text->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_geda_text_write_finish(
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
