#ifndef __BBGEDABUS__
#define __BBGEDABUS__
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


#define BB_GEDA_BUS_WIDTH (30)


#define BB_GEDA_BUS_TOKEN "U"


#define BB_TYPE_GEDA_BUS bb_geda_bus_get_type()
G_DECLARE_FINAL_TYPE(BbGedaBus, bb_geda_bus, BB, GEDA_BUS, BbGedaItem)


int
bb_geda_bus_get_x0(BbGedaBus *bus);


int
bb_geda_bus_get_x1(BbGedaBus *bus);


int
bb_geda_bus_get_y0(BbGedaBus *bus);


int
bb_geda_bus_get_y1(BbGedaBus *bus);


BbGedaBus*
bb_geda_bus_new();


BbGedaBus*
bb_geda_bus_new_with_params(BbParams *params, GError **error);


void
bb_geda_bus_set_x0(BbGedaBus *bus, int x);


void
bb_geda_bus_set_x1(BbGedaBus *bus, int x);


void
bb_geda_bus_set_y0(BbGedaBus *bus, int y);


void
bb_geda_bus_set_y1(BbGedaBus *bus, int y);


#endif
