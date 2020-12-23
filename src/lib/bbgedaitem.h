#ifndef __BbGedaItem__
#define __BbGedaItem__
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
#include "bbbounds.h"
#include "bbboundscalculator.h"
#include "bbitemrenderer.h"

#define BB_TYPE_GEDA_ITEM bb_geda_item_get_type()
G_DECLARE_DERIVABLE_TYPE(BbGedaItem, bb_geda_item, BB, GEDA_ITEM, GObject)

struct _BbGedaItemClass
{
    GObjectClass parent_class;

    BbBounds* (*calculate_bounds)(BbGedaItem *item, BbBoundsCalculator *calculator);
    BbGedaItem* (*clone)(BbGedaItem *item);
    gboolean (*is_significant)(BbGedaItem *item);
    void (*mirror_x)(BbGedaItem *item, int cx);
    void (*mirror_y)(BbGedaItem *item, int cy);
    void (*render)(BbGedaItem *item, BbItemRenderer *renderer);
    void (*rotate)(BbGedaItem *item, int cx, int cy, int angle);
    void (*translate)(BbGedaItem *item, int dx, int dy);
    gboolean (*write)(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error);

    void (*write_async)(
        BbGedaItem *item,
        GOutputStream *stream,
        int io_priority,
        GCancellable *cancellable,
        GAsyncReadyCallback callback,
        gpointer callback_data
        );

    gboolean (*write_finish)(
        BbGedaItem *item,
        GOutputStream *stream,
        GAsyncResult *result,
        GError **error
        );
};


BbBounds*
bb_geda_item_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator);

BbGedaItem*
bb_geda_item_clone(BbGedaItem *item);

gboolean
bb_geda_item_is_significant(BbGedaItem *item);

void
bb_geda_item_mirror_x(BbGedaItem *item, int cx);

void
bb_geda_item_mirror_y(BbGedaItem *item, int cy);

void
bb_geda_item_render(BbGedaItem *item, BbItemRenderer *renderer);

void
bb_geda_item_rotate(BbGedaItem *item, int cx, int cy, int angle);

void
bb_geda_item_translate(BbGedaItem *item, int dx, int dy);

gboolean
bb_geda_item_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error);

void
bb_geda_item_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

gboolean
bb_geda_item_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );

#endif
