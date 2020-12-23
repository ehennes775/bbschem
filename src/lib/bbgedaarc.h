#ifndef __BBGEDAARC__
#define __BBGEDAARC__
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


#define BB_GEDA_ARC_TOKEN "A"


#define BB_TYPE_GEDA_ARC bb_geda_arc_get_type()
G_DECLARE_FINAL_TYPE(BbGedaArc, bb_geda_arc, BB, GEDA_ARC, BbGedaItem)


int
bb_geda_arc_get_center_x(BbGedaArc *arc);


int
bb_geda_arc_get_center_y(BbGedaArc *arc);


int
bb_geda_arc_get_radius(BbGedaArc *arc);


int
bb_geda_arc_get_start_angle(BbGedaArc *arc);


int
bb_geda_arc_get_sweep_angle(BbGedaArc *arc);


BbGedaArc*
bb_geda_arc_new_with_params(BbParams *params, GError **error);


void
bb_geda_arc_set_center_x(BbGedaArc *arc, int x);


void
bb_geda_arc_set_center_y(BbGedaArc *arc, int y);


void
bb_geda_arc_set_radius(BbGedaArc *arc, int radius);


void
bb_geda_arc_set_start_angle(BbGedaArc *arc, int angle);


void
bb_geda_arc_set_sweep_angle(BbGedaArc *arc, int angle);


#endif
