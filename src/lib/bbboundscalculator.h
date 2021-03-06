#ifndef __BBBOUNDSCALCULATOR__
#define __BBBOUNDSCALCULATOR__
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

#define BB_TYPE_BOUNDS_CALCULATOR bb_bounds_calculator_get_type()
G_DECLARE_INTERFACE(BbBoundsCalculator, bb_bounds_calculator, BB, BOUNDS_CALCULATOR, GObject)

struct _BbBoundsCalculatorInterface
{
    GTypeInterface g_iface;

    BbBounds* (*calculate_from_corners)(BbBoundsCalculator *calculator, int x0, int y0, int x1, int y1, int width);
};


BbBounds*
bb_bounds_calculator_calculate_from_corners(BbBoundsCalculator *calculator, int x0, int y0, int x1, int y1, int width);

#endif
