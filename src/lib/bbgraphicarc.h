#ifndef __BBGRAPHICARC__
#define __BBGRAPHICARC__
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


#define BB_GRAPHIC_ARC_TOKEN "A"


#define BB_TYPE_GRAPHIC_ARC bb_graphic_arc_get_type()
G_DECLARE_FINAL_TYPE(BbGraphicArc, bb_graphic_arc, BB, GRAPHIC_ARC, BbSchematicItem)


int
bb_graphic_arc_get_center_x(BbGraphicArc *arc);


int
bb_graphic_arc_get_center_y(BbGraphicArc *arc);


int
bb_graphic_arc_get_radius(BbGraphicArc *arc);


int
bb_graphic_arc_get_start_angle(BbGraphicArc *arc);


int
bb_graphic_arc_get_sweep_angle(BbGraphicArc *arc);


BbGraphicArc*
bb_graphic_arc_new_with_params(BbParams *params, GError **error);


void
bb_graphic_arc_set_center_x(BbGraphicArc *arc, int x);


void
bb_graphic_arc_set_center_y(BbGraphicArc *arc, int y);


void
bb_graphic_arc_set_radius(BbGraphicArc *arc, int radius);


void
bb_graphic_arc_set_start_angle(BbGraphicArc *arc, int angle);


void
bb_graphic_arc_set_sweep_angle(BbGraphicArc *arc, int angle);


#endif
