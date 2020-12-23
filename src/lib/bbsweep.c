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
#include "bbangle.h"
#include "bbsweep.h"


int
bb_sweep_from_angles(int a0, int a1)
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


gboolean
bb_sweep_is_clockwise(int sweep)
{
    return !bb_sweep_is_counterclockwise(sweep);
}


gboolean
bb_sweep_is_counterclockwise(int sweep)
{
    return (sweep >= 0);
}


gboolean
bb_sweep_is_normal(int sweep)
{
    return ((sweep >= -360) && (sweep <= 360));
}


int
bb_sweep_normalize(int sweep)
{
    int result = CLAMP(sweep, -360, 360);

    g_warn_if_fail(bb_sweep_is_normal(result));

    return result;
}


int
bb_sweep_reverse(int sweep)
{
    int result = 360;

    if (sweep <= -360)
    {
        result = 360;
    }
    else if (sweep <= 0)
    {
        result = sweep + 360;
    }
    else if (sweep >= 360)
    {
        result = -360;
    }
    else if (sweep > 0)
    {
        result = sweep - 360;
    }
    else
    {
        g_warn_if_reached();
    }

    g_warn_if_fail(bb_sweep_is_normal(result));

    return result;
}
