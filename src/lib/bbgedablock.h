#ifndef __BBGEDABLOCK__
#define __BBGEDABLOCK__
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
#include "bbgedaitem.h"

#define BB_TYPE_GEDA_BLOCK bb_geda_block_get_type()
G_DECLARE_FINAL_TYPE(BbGedaBlock, bb_geda_block, BB, GEDA_BLOCK, BbGedaItem)


int
bb_geda_block_get_insert_x(BbGedaBlock *block);


int
bb_geda_block_get_insert_y(BbGedaBlock *block);


BbGedaBlock*
bb_geda_block_new();


void
bb_geda_block_set_insert_x(BbGedaBlock *block, int x);


void
bb_geda_block_set_insert_y(BbGedaBlock *block, int y);


#endif
