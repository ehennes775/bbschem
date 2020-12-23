#ifndef __BbGedaBox__
#define __BbGedaBox__
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
#include "bbparams.h"
#include "bbgedaitem.h"


#define BB_GEDA_BOX_TOKEN "B"


#define BB_TYPE_GEDA_BOX bb_geda_box_get_type()
G_DECLARE_FINAL_TYPE(BbGedaBox, bb_geda_box, BB, GEDA_BOX, BbGedaItem)


int
bb_geda_box_get_x0(BbGedaBox *box);


int
bb_geda_box_get_x1(BbGedaBox *box);


int
bb_geda_box_get_y0(BbGedaBox *box);


int
bb_geda_box_get_y1(BbGedaBox *box);


BbGedaBox*
bb_geda_box_new();


BbGedaBox*
bb_geda_box_new_with_params(BbParams *params, GError **error);


void
bb_geda_box_set_x0(BbGedaBox *box, int x);


void
bb_geda_box_set_x1(BbGedaBox *box, int x);


void
bb_geda_box_set_y0(BbGedaBox *box, int y);


void
bb_geda_box_set_y1(BbGedaBox *box, int y);


#endif
