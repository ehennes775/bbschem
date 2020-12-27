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


G_DEFINE_BOXED_TYPE(BbBounds, bb_bounds, bb_bounds_copy, bb_bounds_free)


BbBounds*
bb_bounds_copy(const BbBounds *bounds)
{
    return g_memdup(bounds, sizeof(BbBounds));
}


void
bb_bounds_free(BbBounds *bounds)
{
    g_free(bounds);
}


gboolean
bb_bounds_is_empty(const BbBounds *bounds)
{
    return (bounds == NULL) || (bounds->min_x > bounds->max_x) || (bounds->min_y > bounds->max_y);
}


BbBounds*
bb_bounds_new()
{
    BbBounds *bounds = g_new(BbBounds, 1);

    bounds->min_x = G_MAXINT;
    bounds->min_y = G_MAXINT;
    bounds->max_x = G_MININT;
    bounds->max_y = G_MININT;

    return bounds;
}


BbBounds*
bb_bounds_new_with_points(int x0, int y0, int x1, int y1)
{
    BbBounds *bounds = g_new(BbBounds, 1);

    bounds->min_x = MIN(x0, x1);
    bounds->min_y = MIN(y0, y1);
    bounds->max_x = MAX(x0, x1);
    bounds->max_y = MAX(y0, y1);

    return bounds;
}


void
bb_bounds_translate(BbBounds *bounds, int dx, int dy)
{
    if (!bb_bounds_is_empty(bounds))
    {
        bounds->min_x += dx;
        bounds->min_y += dy;
        bounds->max_x += dx;
        bounds->max_y += dy;
    }
}


void
bb_bounds_union(BbBounds *result, BbBounds *a, BbBounds *b)
{
    g_return_if_fail(result != NULL);
    g_return_if_fail(a != NULL);
    g_return_if_fail(b != NULL);

    result->min_x = MIN(a->min_x, b->min_x);
    result->min_y = MIN(a->min_y, b->min_y);
    result->max_x = MAX(a->max_x, b->max_x);
    result->max_y = MAX(a->max_y, b->max_y);
}
