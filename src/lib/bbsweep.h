#ifndef __BBSWEEP__
#define __BBSWEEP__
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


/**
 * Calculate the sweep between two angles
 *
 * Calculates the sweep from a0, counterclockwise to a1.
 *
 * @param a0 The starting angle
 * @param a1 The ending angle
 */
int
bb_sweep_from_angles(int a0, int a1);


/**
 * Checks if the given sweep is clockwise
 *
 * Positive sweeps are clockwise. A sweep angle of 0 is
 * considered counterclockwise.
 *
 * @param sweep The sweep to check
 * @return If the sweep is clockwise
 */
gboolean
bb_sweep_is_clockwise(int sweep);


/**
 * Checks if the given sweep is counterclockwise
 *
 * Positive sweeps are clockwise. A sweep angle of 0 is
 * considered counterclockwise.
 *
 * @param sweep The sweep to check
 * @return If the sweep is counterclockwise
 */
gboolean
bb_sweep_is_counterclockwise(int sweep);


/**
 * Checks if the given sweep is normal
 *
 * Normal sweeps are in the interval [-360,360].
 *
 * @param sweep The sweep to check
 * @return If the sweep is normal
 */
gboolean
bb_sweep_is_normal(int sweep);


/**
 * Normalize a sweep angle
 *
 * Normal sweeps are in the interval [-360,360].
 *
 * @param sweep The sweep to normalize
 * @return The normalized sweep
 */
int
bb_sweep_normalize(int sweep);


/**
 * Reverse the direction of a sweep
 *
 * This function provides an alternate form of an arc for
 * drawing operations.
 *
 * This function reverses the direction of the sweep. Both
 * starting angle and ending angle would remain the same. If
 * the input sweep is counterclockwise (positive), the returned
 * sweep will be clockwise (negative). Similarly, if the input
 * sweep is clockwise, the returned sweep will be
 * counterclockwise.
 *
 * Sweeps of -360, 0, and 360 are handled as special cases. The
 * function does not return a sweep of 0, which would result in
 * a degenerate arc.
 *
 * ||''Argument''||''Result''||
 * ||(-inf,-360]||360||
 * ||[-359,-1]||argument + 360||
 * ||0||360||
 * ||[1,359]||argument - 360||
 * ||[360,+inf)||-360||
 *
 * @param sweep The sweep angle
 * @return The sweep in the opposite direction
 */
int
bb_sweep_reverse(int sweep);


#endif
