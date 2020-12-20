#ifndef __BBGEDANET__
#define __BBGEDANET__
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


#define BB_GEDA_NET_TOKEN "N"

#define BB_GEDA_NET_WIDTH (10)



#define BB_TYPE_GEDA_NET bb_geda_net_get_type()
G_DECLARE_FINAL_TYPE(BbGedaNet, bb_geda_net, BB, GEDA_NET, BbGedaItem)


int
bb_geda_net_get_x0(BbGedaNet *net);


int
bb_geda_net_get_x1(BbGedaNet *net);


int
bb_geda_net_get_y0(BbGedaNet *net);


int
bb_geda_net_get_y1(BbGedaNet *net);


BbGedaNet*
bb_geda_net_new();


BbGedaNet*
bb_geda_net_new_with_params(BbParams *params, GError **error);


void
bb_geda_net_set_x0(BbGedaNet *net, int x);


void
bb_geda_net_set_x1(BbGedaNet *net, int x);


void
bb_geda_net_set_y0(BbGedaNet *net, int y);


void
bb_geda_net_set_y1(BbGedaNet *net, int y);


#endif
