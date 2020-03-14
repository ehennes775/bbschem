#ifndef __BBSCHEMATICITEM__
#define __BBSCHEMATICITEM__
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
#include "bbbounds.h"
#include "bbboundscalculator.h"
#include "bbitemrenderer.h"

#define BB_TYPE_SCHEMATIC_ITEM bb_schematic_item_get_type()
G_DECLARE_DERIVABLE_TYPE(BbSchematicItem, bb_schematic_item, BB, SCHEMATIC_ITEM, GObject)

struct _BbSchematicItemClass
{
    GObjectClass parent_class;

    BbBounds* (*calculate_bounds)(BbSchematicItem *item, BbBoundsCalculator *calculator);
    void (*render)(BbSchematicItem *item, BbItemRenderer *renderer);
};


BbBounds*
bb_schematic_item_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

void
bb_schematic_item_render(BbSchematicItem *item, BbItemRenderer *renderer);

#endif
