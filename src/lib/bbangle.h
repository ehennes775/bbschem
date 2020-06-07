#ifndef __BBANGLE__
#define __BBANGLE__
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
 * Calculate the sweep between two angles
 *
 * Calculates the sweep from a0, counterclockwise to a1.
 *
 * @param a0 The starting angle
 * @param a1 The ending angle
 */
int
bb_angle_calculate_sweep(int a0, int a1);


/**
 * Convert radians to degrees
 *
 * @param radians the angle in radians
 * @return the angle in degrees
 */
int
bb_angle_from_radians(double radians);


/**
 * Checks if an angle is [0,360)
 *
 * @param angle the angle in degrees
 * @return TRUE if the angle is [0,360)
 */
gboolean
bb_angle_is_normal(int angle);


/**
 * Checks if an angle is orthographic
 *
 * @param angle the angle in degrees
 * @return TRUE if the angle is a multiple of 90 degrees
 */
gboolean
bb_angle_is_orthogonal(int angle);


/**
 * Make an angle orthographic
 *
 * Snaps the angle to the nearest 90 degrees
 *
 * @param angle the angle in degrees
 * @return the orthographic angle
 */
int
bb_angle_make_orthogonal(int angle);


/**
 * Normalize an angle to [0,360)
 *
 * @param angle the angle in degrees
 * @return the normalized angle inside [0,360)
 */
int
bb_angle_normalize(int angle);


/**
 * Convert degrees to radians
 *
 * @param angle the angle in degrees
 * @return the angle in radians
 */
double
bb_angle_to_radians(int angle);


#endif
