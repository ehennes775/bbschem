#ifndef __BBSCALEGRIDDIRECTION__
#define __BBSCALEGRIDDIRECTION__
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

typedef enum _BbScaleGridDirection BbScaleGridDirection;

enum _BbScaleGridDirection
{
    /**
     * Select a finer graticule
     */
    BB_SCALE_GRID_DIRECTION_DOWN,

    /**
     * Reset the grid scale to the default
     */
    BB_SCALE_GRID_DIRECTION_RESET,

    /**
     * Select a coarser graticule
     */
    BB_SCALE_GRID_DIRECTION_UP
};

#endif
