#ifndef __BBGEDACIRCLE__
#define __BBGEDACIRCLE__
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


#define BB_GEDA_CIRCLE_TOKEN "V"


#define BB_TYPE_GEDA_CIRCLE bb_geda_circle_get_type()
G_DECLARE_FINAL_TYPE(BbGedaCircle, bb_geda_circle, BB, GEDA_CIRCLE, BbGedaItem)


int
bb_geda_circle_get_center_x(BbGedaCircle *circle);


int
bb_geda_circle_get_center_y(BbGedaCircle *circle);


int
bb_geda_circle_get_radius(BbGedaCircle *circle);


BbGedaCircle*
bb_geda_circle_new();


BbGedaCircle*
bb_geda_circle_new_with_params(BbParams *params, GError **error);


void
bb_geda_circle_set_center_x(BbGedaCircle *circle, int x);


void
bb_geda_circle_set_center_y(BbGedaCircle *circle, int y);


void
bb_geda_circle_set_radius(BbGedaCircle *circle, int radius);


#endif
