#ifndef __BBBOUNDS__
#define __BBBOUNDS__
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

#define BB_TYPE_BOUNDS bb_bounds_get_type()


typedef struct _BbBounds BbBounds;

struct _BbBounds
{
    int min_x;
    int min_y;
    int max_x;
    int max_y;
};


BbBounds*
bb_bounds_copy(const BbBounds *bounds);

void
bb_bounds_free(BbBounds *bounds);

gboolean
bb_bounds_is_empty(const BbBounds *bounds);

BbBounds*
bb_bounds_new();

BbBounds*
bb_bounds_new_with_points(int x0, int y0, int x1, int y1);

void
bb_bounds_translate(BbBounds *bounds, int dx, int dy);


#endif
