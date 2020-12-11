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
#include <math.h>
#include "bbangle.h"
#include "bbcoord.h"

/**
 * Calculate the distance between two points
 *
 * @param x0 The x coordinate of the first point
 * @param y0 The y coordinate of the first point
 * @param x1 The x coordinate of the second point
 * @param y1 The y coordinate of the second point
 * @return the distance between the two points
 */
double
bb_coord_distance(int x0, int y0, int x1, int y1)
{
    return hypot(x1 - x0, y1 - y0);
}


double
bb_coord_radians(int x0, int y0, int x1, int y1)
{
    return atan2(y1 - y0, x1 - x0);
}


/**
 * Rotate a point
 *
 * @param cx The center of rotation x coordinate
 * @param cy The center of rotation y coordinate
 * @param angle The angle of rotation
 * @param x The rotated x coordinate
 * @param y The rotated y coordinate
 */
void
bb_coord_rotate(int cx, int cy, int angle, int *x, int *y)
{
    g_return_if_fail(x != NULL);
    g_return_if_fail(y != NULL);

    double radians = bb_angle_to_radians(angle);

    double cos_theta = cos(radians);
    double sin_theta = sin(radians);

    int temp_x = *x - cx;
    int temp_y = *y - cy;

    int temp2_x = (int) round(temp_x * cos_theta - temp_y * sin_theta);
    int temp2_y = (int) round(temp_x * sin_theta + temp_y * cos_theta);

    *x = temp2_x + cx;
    *y = temp2_y + cy;
}


int
bb_coord_round(double c)
{
    return (int) round(c);
}


/**
 * Calculate the shortest distance between a point and a line
 *
 * @param x0 The x coordinate of the first endpoint on the line
 * @param y0 The y coordinate of the first endpoint on the line
 * @param x1 The x coordinate of the second endpoint on the line
 * @param y1 The y coordinate of the second endpoint on the line
 * @param x The x coordinate of the point
 * @param y The y coordinate of the point
 * @return The shortest distance from the point to the line
 */
double
bb_coord_shortest_distance_line(
    int x0,
    int y0,
    int x1,
    int y1,
    int x,
    int y
)
{
    double dx;
    double dy;

    double lx0 = (double)x0;
    double ly0 = (double)y0;
    double ldx = (double)(x1 - lx0);
    double ldy = (double)(y1 - ly0);

    if (ldx == 0.0 && ldy == 0.0)
    {
        dx = x - lx0;
        dy = y - ly0;
    }
    else
    {
        double dx0 = ldx * (x - lx0);
        double dy0 = ldy * (y - ly0);

        double t = (dx0 + dy0) / (ldx * ldx + ldy * ldy);

        t = CLAMP(t, 0.0, 1.0);

        double cx = t * ldx + lx0;
        double cy = t * ldy + ly0;

        dx = x - cx;
        dy = y - cy;
    }

    return hypot(dx, dy);
}


/**
 * Snap a coordinate to the nearest grid
 *
 * @param coord the coordinate
 * @param grid the grid size
 * @return the snapped coordinate
 */
int
bb_coord_snap(int coord, int grid)
{
    g_return_val_if_fail(grid > 0, coord);

    int sign = (coord >= 0) ? 1 : -1;
    int val = ABS(coord);

    int dividend = val / grid;
    int remainder = val % grid;

    int result2 = dividend * grid;

    if (remainder > (grid / 2))
    {
        result2 += grid;
    }

    return sign * result2;
}


/**
 * Snap a line to an orthographic angle
 *
 * @param x0 The x coordinate of the first point
 * @param y0 The y coordinate of the first point
 * @param x1 The x coordinate of the second point
 * @param y1 The y coordinate of the second point
 */
void
bb_coord_snap_orthogonal(int x0, int y0, int *x1, int *y1)
{
    g_return_if_fail(x1 != NULL);
    g_return_if_fail(y1 != NULL);

    int dx = ABS(*x1 - x0);
    int dy = ABS(*y1 - y0);

    if (dx < dy)
    {
        *x1 = x0;
    }
    else
    {
        *y1 = y0;
    }
}


void
bb_coord_translate(int dx, int dy, int *x, int *y, int count)
{
    g_return_if_fail(x != NULL);
    g_return_if_fail(y != NULL);
    g_return_if_fail(count >= 0);

    int *x1 = x + count;

    while(x < x1)
    {
        *x += dx;
        *y += dy;

        x++;
        y++;
    }
}
