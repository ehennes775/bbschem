#ifndef __BBELECTRICALBUS__
#define __BBELECTRICALBUS__
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
#include "bbschematicitem.h"


#define BB_ELECTRICAL_BUS_WIDTH (30)


#define BB_ELECTRICAL_BUS_TOKEN "U"


#define BB_TYPE_ELECTRICAL_BUS bb_electrical_bus_get_type()
G_DECLARE_FINAL_TYPE(BbElectricalBus, bb_electrical_bus, BB, ELECTRICAL_BUS, BbSchematicItem)


int
bb_electrical_bus_get_x0(BbElectricalBus *bus);


int
bb_electrical_bus_get_x1(BbElectricalBus *bus);


int
bb_electrical_bus_get_y0(BbElectricalBus *bus);


int
bb_electrical_bus_get_y1(BbElectricalBus *bus);


BbElectricalBus*
bb_electrical_bus_new();


BbElectricalBus*
bb_electrical_bus_new_with_params(BbParams *params, GError **error);


void
bb_electrical_bus_set_x0(BbElectricalBus *bus, int x);


void
bb_electrical_bus_set_x1(BbElectricalBus *bus, int x);


void
bb_electrical_bus_set_y0(BbElectricalBus *bus, int y);


void
bb_electrical_bus_set_y1(BbElectricalBus *bus, int y);


#endif
