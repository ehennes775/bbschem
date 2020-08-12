#ifndef __BBGRAPHICCIRCLE__
#define __BBGRAPHICCIRCLE__
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

#define BB_TYPE_GRAPHIC_CIRCLE bb_graphic_circle_get_type()
G_DECLARE_FINAL_TYPE(BbGraphicCircle, bb_graphic_circle, BB, GRAPHIC_CIRCLE, BbSchematicItem)


int
bb_graphic_circle_get_center_x(BbGraphicCircle *circle);


int
bb_graphic_circle_get_center_y(BbGraphicCircle *circle);


int
bb_graphic_circle_get_radius(BbGraphicCircle *circle);


BbGraphicCircle*
bb_graphic_circle_new();


void
bb_graphic_circle_set_center_x(BbGraphicCircle *circle, int x);


void
bb_graphic_circle_set_center_y(BbGraphicCircle *circle, int y);


void
bb_graphic_circle_set_radius(BbGraphicCircle *circle, int radius);


#endif
