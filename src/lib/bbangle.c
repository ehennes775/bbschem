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

#include <math.h>
#include <gtk/gtk.h>
#include "bbangle.h"


int
bb_angle_calculate_sweep(int a0, int a1)
{
    int na0 = bb_angle_normalize(a0);
    int na1 = bb_angle_normalize(a1);

    int sweep = na1 - na0;

    if (sweep <= 0)
    {
        sweep += 360;
    }

    g_warn_if_fail(sweep > 0);
    g_warn_if_fail(sweep <= 360);

    return sweep;
}


int
bb_angle_from_radians(double radians)
{
    return (int) round(180.0 * radians / M_PI);
}


gboolean
bb_angle_is_normal(int angle)
{
    return ((0 <= angle) && (angle < 360));
}


gboolean
bb_angle_is_orthogonal(int angle)
{
    return ((angle % 90) == 0);
}


int
bb_angle_make_orthogonal(int angle)
{
    int result = (int) round(angle / 90.0) * 90;

    g_warn_if_fail(bb_angle_is_orthogonal(result));

    return result;
}


int
bb_angle_normalize(int angle)
{
    if (angle < 0)
    {
        angle = 360 - (-angle % 360);
    }
    if (angle >= 360)
    {
        angle %= 360;
    }

    g_warn_if_fail(bb_angle_is_normal(angle));

    return angle;
}


double
bb_angle_to_radians(int angle)
{
    return M_PI * angle / 180.0;
}
