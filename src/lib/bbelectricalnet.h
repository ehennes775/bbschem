#ifndef __BBELECTRICALNET__
#define __BBELECTRICALNET__
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
#include "bbschematicitem.h"


#define BB_ELECTRICAL_NET_WIDTH (10)


#define BB_TYPE_ELECTRICAL_NET bb_electrical_net_get_type()
G_DECLARE_FINAL_TYPE(BbElectricalNet, bb_electrical_net, BB, ELECTRICAL_NET, BbSchematicItem)


int
bb_electrical_net_get_x0(BbElectricalNet *net);


int
bb_electrical_net_get_x1(BbElectricalNet *net);


int
bb_electrical_net_get_y0(BbElectricalNet *net);


int
bb_electrical_net_get_y1(BbElectricalNet *net);


BbElectricalNet*
bb_electrical_net_new();


void
bb_electrical_net_set_x0(BbElectricalNet *net, int x);


void
bb_electrical_net_set_x1(BbElectricalNet *net, int x);


void
bb_electrical_net_set_y0(BbElectricalNet *net, int y);


void
bb_electrical_net_set_y1(BbElectricalNet *net, int y);


#endif
