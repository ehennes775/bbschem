#ifndef __BBGRAPHICLINE__
#define __BBGRAPHICLINE__
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

#define BB_TYPE_GRAPHIC_LINE bb_graphic_line_get_type()
G_DECLARE_FINAL_TYPE(BbGraphicLine, bb_graphic_line, BB, GRAPHIC_LINE, BbSchematicItem)

int
bb_graphic_line_get_x0(BbGraphicLine *line);

int
bb_graphic_line_get_x1(BbGraphicLine *line);

int
bb_graphic_line_get_y0(BbGraphicLine *line);

int
bb_graphic_line_get_y1(BbGraphicLine *line);

void
bb_graphic_line_set_x0(BbGraphicLine *line, int x);

void
bb_graphic_line_set_x1(BbGraphicLine *line, int x);

void
bb_graphic_line_set_y0(BbGraphicLine *line, int y);

void
bb_graphic_line_set_y1(BbGraphicLine *line, int y);

#endif
