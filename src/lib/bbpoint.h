#ifndef __BBPOINT__
#define __BBPOINT__
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
#include "bbbounds.h"
#include "bbmatrix.h"


typedef struct _BbPoint BbPoint;

struct _BbPoint
{
    double x;
    double y;
};


void
bb_points_calculate_bounds(int count, BbPoint point[count], BbBounds *bounds);


void
bb_point_transform(BbPoint *point, BbMatrix *matrix);


void
bb_point_transform_array(int count, BbPoint point[count],  BbMatrix *matrix);


#endif
