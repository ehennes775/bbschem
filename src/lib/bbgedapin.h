#ifndef __BBGEDAPIN__
#define __BBGEDAPIN__
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
#include "bbparams.h"
#include "bbgedaitem.h"


#define BB_GEDA_PIN_TOKEN "P"


#define BB_TYPE_GEDA_PIN bb_geda_pin_get_type()
G_DECLARE_FINAL_TYPE(BbGedaPin, bb_geda_pin, BB, GEDA_PIN, BbGedaItem)


int
bb_geda_pin_get_x0(BbGedaPin *pin);


int
bb_geda_pin_get_x1(BbGedaPin *pin);


int
bb_geda_pin_get_y0(BbGedaPin *pin);


int
bb_geda_pin_get_y1(BbGedaPin *pin);


BbGedaPin*
bb_geda_pin_new();


BbGedaPin*
bb_geda_pin_new_with_params(BbParams *params, GError **error);


void
bb_geda_pin_set_x0(BbGedaPin *pin, int x);


void
bb_geda_pin_set_x1(BbGedaPin *pin, int x);


void
bb_geda_pin_set_y0(BbGedaPin *pin, int y);


void
bb_geda_pin_set_y1(BbGedaPin *pin, int y);


#endif
