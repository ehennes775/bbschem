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
#include "bbboundscalculator.h"


static void
bb_bounds_calculator_default_init(BbBoundsCalculatorInterface *class);


G_DEFINE_INTERFACE(BbBoundsCalculator, bb_bounds_calculator, G_TYPE_OBJECT)


BbBounds*
bb_bounds_calculator_calculate_from_corners(BbBoundsCalculator *calculator, int x0, int y0, int x1, int y1, int width)
{
    BbBoundsCalculatorInterface *iface = BB_BOUNDS_CALCULATOR_GET_IFACE(calculator);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->calculate_from_corners != NULL, NULL);

    return iface->calculate_from_corners(calculator, x0, y0, x1, y1, width);
}


static BbBounds*
bb_bounds_calculator_calculate_from_corners_missing(BbBoundsCalculator *calculator, int x0, int y0, int x1, int y1, int width)
{
    g_error("bb_bounds_calculator_calculate_from_corners() not overridden");
}


static void
bb_bounds_calculator_default_init(BbBoundsCalculatorInterface *class)
{
    g_return_if_fail(class != NULL);

    class->calculate_from_corners = bb_bounds_calculator_calculate_from_corners_missing;
}
