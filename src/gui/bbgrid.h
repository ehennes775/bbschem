#ifndef __BBGRID__
#define __BBGRID__
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
#include "bbtoolsubject.h"
#include "bbscalegriddirection.h"
#include "bbgraphics.h"


#define BB_GRID_DEFAULT_SIZE (100)


#define BB_TYPE_GRID bb_grid_get_type()
G_DECLARE_FINAL_TYPE(BbGrid, bb_grid, BB, GRID, GObject)


gboolean
bb_grid_get_can_scale(BbGrid *grid, BbScaleGridDirection direction);

gboolean
bb_grid_get_can_scale_down(BbGrid *grid);

gboolean
bb_grid_get_can_scale_reset(BbGrid *grid);

gboolean
bb_grid_get_can_scale_up(BbGrid *grid);

int
bb_grid_get_draw_size(BbGrid *grid);

int
bb_grid_get_snap_size(BbGrid *grid);

BbToolSubject*
bb_grid_get_subject(BbGrid *grid);

BbGrid*
bb_grid_new(BbToolSubject *tool_subject);

void
bb_grid_scale(BbGrid *grid, BbScaleGridDirection direction);

void
bb_grid_scale_down(BbGrid *grid);

void
bb_grid_draw(BbGrid *grid, BbGraphics *graphics);

void
bb_grid_scale_reset(BbGrid *grid);

void
bb_grid_scale_up(BbGrid *grid);

#endif
