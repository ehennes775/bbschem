#ifndef __BBCOORD__
#define __BBCOORD__
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
bb_coord_distance(int x0, int y0, int x1, int y1);


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
bb_coord_rotate(int cx, int cy, int angle, int *x, int *y);


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
    );


/**
 * Snap a coordinate to the nearest grid
 *
 * @param coord the coordinate
 * @param grid the grid size
 * @return the snapped coordinate
 */
int
bb_coord_snap(int coord, int grid);


/**
 * Snap a line to an orthographic angle
 *
 * @param x0 The x coordinate of the first point
 * @param y0 The y coordinate of the first point
 * @param x1 The x coordinate of the second point
 * @param y1 The y coordinate of the second point
 */
void
bb_coord_snap_orthogonal(int x0, int y0, int* x1, int* y1);


/**
 * Translate coordinates
 *
 * @param dx The displacement along the x axis
 * @param dy The displacement along the y axis
 * @param x The x coordinates to translate
 * @param y The y coordinates to translate
 * @param count The number of coordinate pairs to translate
 */
void
bb_coord_translate(int dx, int dy, int *x, int *y, int count);


#endif
