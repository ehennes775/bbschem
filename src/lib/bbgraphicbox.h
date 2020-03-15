#ifndef __BBGRAPHICBOX__
#define __BBGRAPHICBOX__
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

#define BB_TYPE_GRAPHIC_BOX bb_graphic_box_get_type()
G_DECLARE_FINAL_TYPE(BbGraphicBox, bb_graphic_box, BB, GRAPHIC_BOX, BbSchematicItem)

int
bb_graphic_box_get_width(BbGraphicBox *box);

int
bb_graphic_box_get_x0(BbGraphicBox *box);

int
bb_graphic_box_get_x1(BbGraphicBox *box);

int
bb_graphic_box_get_y0(BbGraphicBox *box);

int
bb_graphic_box_get_y1(BbGraphicBox *box);

void
bb_graphic_box_set_width(BbGraphicBox *box, int width);

void
bb_graphic_box_set_x0(BbGraphicBox *box, int x);

void
bb_graphic_box_set_x1(BbGraphicBox *box, int x);

void
bb_graphic_box_set_y0(BbGraphicBox *box, int y);

void
bb_graphic_box_set_y1(BbGraphicBox *box, int y);

#endif
