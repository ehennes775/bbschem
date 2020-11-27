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
#include "bbuniversalblock.h"
#include "bbadjustableitemcolor.h"


enum
{
    PROP_0,

    PROP_INSERT_X,
    PROP_INSERT_Y,

    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbUniversalBlock
{
    BbSchematicItem parent;

    BbItemParams *params;

    int insert_x;
    int insert_y;
};


static void
bb_universal_block_adjustable_item_color_init(BbAdjustableItemColorInterface *iface);

static BbBounds*
bb_universal_block_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static BbSchematicItem*
bb_universal_block_clone(BbSchematicItem *item);

static void
bb_universal_block_dispose(GObject *object);

static void
bb_universal_block_finalize(GObject *object);

static void
bb_universal_block_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_universal_block_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_universal_block_set_item_color(BbUniversalBlock *block, int color);

static void
bb_universal_block_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_universal_block_translate(BbSchematicItem *item, int dx, int dy);

static void
bb_universal_block_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_universal_block_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbUniversalBlock,
    bb_universal_block,
    BB_TYPE_SCHEMATIC_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_universal_block_adjustable_item_color_init)
    )


static void
bb_universal_block_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static BbBounds*
bb_universal_block_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator)
{
    BbUniversalBlock *block = BB_UNIVERSAL_BLOCK(item);

    g_return_val_if_fail(block != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        block->insert_x,
        block->insert_y,
        block->insert_x,
        block->insert_y,
        0
        );
}


static void
bb_universal_block_class_init(BbUniversalBlockClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_universal_block_dispose;
    object_class->finalize = bb_universal_block_finalize;
    object_class->get_property = bb_universal_block_get_property;
    object_class->set_property = bb_universal_block_set_property;

    BB_SCHEMATIC_ITEM_CLASS(klasse)->calculate_bounds = bb_universal_block_calculate_bounds;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->clone = bb_universal_block_clone;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->render = bb_universal_block_render;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->translate = bb_universal_block_translate;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_async = bb_universal_block_write_async;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_finish = bb_universal_block_write_finish;

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

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_SCHEMATIC_ITEM);
}


static BbSchematicItem*
bb_universal_block_clone(BbSchematicItem *item)
{
    return BB_SCHEMATIC_ITEM(g_object_new(
        BB_TYPE_UNIVERSAL_BLOCK,
        "insert-x", bb_universal_block_get_insert_x(BB_UNIVERSAL_BLOCK(item)),
        "insert-y", bb_universal_block_get_insert_y(BB_UNIVERSAL_BLOCK(item)),

        NULL
        ));
}


static void
bb_universal_block_dispose(GObject *object)
{
}


static void
bb_universal_block_finalize(GObject *object)
{
}


int
bb_universal_block_get_insert_x(BbUniversalBlock *block)
{
    g_return_val_if_fail(block != NULL, 0);

    return block->insert_x;
}


int
bb_universal_block_get_insert_y(BbUniversalBlock *block)
{
    g_return_val_if_fail(block != NULL, 0);

    return block->insert_y;
}


static void
bb_universal_block_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_INSERT_X:
            g_value_set_int(value, bb_universal_block_get_insert_x(BB_UNIVERSAL_BLOCK(object)));
            break;

        case PROP_INSERT_Y:
            g_value_set_int(value, bb_universal_block_get_insert_y(BB_UNIVERSAL_BLOCK(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_universal_block_init(BbUniversalBlock *block)
{
    g_return_if_fail(block != NULL);
}


BbUniversalBlock*
bb_universal_block_new()
{
    return g_object_new(BB_TYPE_UNIVERSAL_BLOCK, NULL);
}


static void
bb_universal_block_render(BbSchematicItem *item, BbItemRenderer *renderer)
{
    BbUniversalBlock *block = BB_UNIVERSAL_BLOCK(item);
    g_return_if_fail(block != NULL);
}


void
bb_universal_block_set_insert_x(BbUniversalBlock *block, int x)
{
    g_return_if_fail(block != NULL);

    if (block->insert_x != x)
    {
        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        block->insert_x = x;

        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_INSERT_X]);
    }
}


void
bb_universal_block_set_insert_y(BbUniversalBlock *block, int y)
{
    g_return_if_fail(block != NULL);

    if (block->insert_y != y)
    {
        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        block->insert_y = y;

        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_INSERT_Y]);
    }
}


static void
bb_universal_block_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_INSERT_X:
            bb_universal_block_set_insert_x(BB_UNIVERSAL_BLOCK(object), g_value_get_int(value));
            break;

        case PROP_INSERT_Y:
            bb_universal_block_set_insert_y(BB_UNIVERSAL_BLOCK(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_universal_block_translate(BbSchematicItem *item, int dx, int dy)
{
    BbUniversalBlock *block = BB_UNIVERSAL_BLOCK(item);
    g_return_if_fail(block != NULL);

    bb_coord_translate(dx, dy, &block->insert_x, &block->insert_y, 1);

    g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_INSERT_X]);
    g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_INSERT_Y]);
}


static void
bb_universal_block_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbUniversalBlock *block = BB_UNIVERSAL_BLOCK(item);

    bb_item_params_write_async(
        block->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_universal_block_write_finish(
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
