#ifndef __BBMATRIX__
#define __BBMATRIX__
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

typedef struct _BbMatrix BbMatrix;

struct _BbMatrix
{
    cairo_matrix_t matrix;
};


void
bb_matrix_init(BbMatrix *matrix);


void
bb_matrix_rotate(BbMatrix *matrix, double radians);


void
bb_matrix_transform_point(BbMatrix *matrix, int count, double x[count], double y[count]);

#endif
