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
#include <math.h>
#include "bbpoint.h"


void
bb_points_calculate_bounds(int count, BbPoint point[count], BbBounds *bounds)
{
    g_return_if_fail(point != NULL);
    g_return_if_fail(bounds != NULL);

    for (int index = 0; index < count; index++)
    {
        BbBounds b =
        {
            .min_x = floor(point[index].x),
            .min_y = floor(point[index].y),
            .max_x = ceil(point[index].x),
            .max_y = ceil(point[index].y)
        };

        bb_bounds_union(bounds, bounds, &b);
    }
}


void
bb_point_transform(BbPoint *point, BbMatrix *matrix)
{
    g_return_if_fail(point != NULL);
    g_return_if_fail(matrix != NULL);

    bb_matrix_transform_point(matrix, 1, &point->x, &point->y);
}


void
bb_point_transform_array(int count, BbPoint point[count],  BbMatrix *matrix)
{
    g_return_if_fail(point != NULL);
    g_return_if_fail(matrix != NULL);

    for (int index = 0; index < count; index++)
    {
        bb_point_transform(&point[index], matrix);
    }
}
