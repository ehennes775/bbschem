#ifndef __BBRELATIVELINETO__
#define __BBRELATIVELINETO__
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
#include "bbpathcommand.h"

#define BB_TYPE_RELATIVE_LINE_TO bb_relative_line_to_get_type()
G_DECLARE_FINAL_TYPE(BbRelativeLineTo, bb_relative_line_to, BB, RELATIVE_LINE_TO, BbPathCommand)

struct _BbRelativeLineToClass
{
    BbPathCommandClass parent_class;
};


int
bb_relative_line_to_get_dx(const BbRelativeLineTo *command);


int
bb_relative_line_to_get_dy(const BbRelativeLineTo *command);


BbRelativeLineTo*
bb_relative_line_to_new(int dx, int dy);


void
bb_relative_line_to_set_dx(BbRelativeLineTo *command, int dx);


void
bb_relative_line_to_set_dy(BbRelativeLineTo *command, int dy);

#endif
