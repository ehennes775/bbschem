#ifndef __BBRELATIVEMOVETO__
#define __BBRELATIVEMOVETO__
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

#define BB_TYPE_RELATIVE_MOVE_TO bb_relative_move_to_get_type()
G_DECLARE_FINAL_TYPE(BbRelativeMoveTo, bb_relative_move_to, BB, RELATIVE_MOVE_TO, BbPathCommand)

struct _BbRelativeMoveToClass
{
    BbPathCommandClass parent_class;
};


int
bb_relative_move_to_get_dx(const BbRelativeMoveTo *command);


int
bb_relative_move_to_get_dy(const BbRelativeMoveTo *command);


BbRelativeMoveTo*
bb_relative_move_to_new(int dx, int dy);


void
bb_relative_move_to_set_dx(BbRelativeMoveTo *command, int dx);


void
bb_relative_move_to_set_dy(BbRelativeMoveTo *command, int dy);

#endif
