#ifndef __BBABSOLUTEMOVETO__
#define __BBABSOLUTEMOVETO__
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

#define BB_TYPE_ABSOLUTE_MOVE_TO bb_absolute_move_to_get_type()
G_DECLARE_FINAL_TYPE(BbAbsoluteMoveTo, bb_absolute_move_to, BB, ABSOLUTE_MOVE_TO, BbPathCommand)

struct _BbAbsoluteMoveToClass
{
    BbPathCommandClass parent_class;
};


int
bb_absolute_move_to_get_x(BbAbsoluteMoveTo *command);


int
bb_absolute_move_to_get_y(BbAbsoluteMoveTo *command);


BbAbsoluteMoveTo*
bb_absolute_move_to_new(int x, int y);


void
bb_absolute_move_to_set_x(BbAbsoluteMoveTo *command, int x);


void
bb_absolute_move_to_set_y(BbAbsoluteMoveTo *command, int y);

#endif
