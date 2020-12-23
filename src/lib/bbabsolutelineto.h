#ifndef __BBABSOLUTELINETO__
#define __BBABSOLUTELINETO__
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
#include "bbpathcommand.h"

#define BB_TYPE_ABSOLUTE_LINE_TO bb_absolute_line_to_get_type()
G_DECLARE_FINAL_TYPE(BbAbsoluteLineTo, bb_absolute_line_to, BB, ABSOLUTE_LINE_TO, BbPathCommand)

struct _BbAbsoluteLineToClass
{
    BbPathCommandClass parent_class;
};


int
bb_absolute_line_to_get_x(const BbAbsoluteLineTo *command);


int
bb_absolute_line_to_get_y(const BbAbsoluteLineTo *command);


BbAbsoluteLineTo*
bb_absolute_line_to_new(int x, int y);


void
bb_absolute_line_to_set_x(BbAbsoluteLineTo *command, int x);


void
bb_absolute_line_to_set_y(BbAbsoluteLineTo *command, int y);

#endif
