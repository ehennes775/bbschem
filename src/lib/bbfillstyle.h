#ifndef __BBFILLSTYLE__
#define __BBFILLSTYLE__
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
#include "bbfilltype.h"
#include "bbparams.h"

/**
 * @brief A default value when uninitialized, unused from the input file, or logic errors occur
 */
#define BB_FILL_STYLE_DEFAULT_ANGLE1 (45)


/**
 * @brief A default value when uninitialized, unused from the input file, or logic errors occur
 */
#define BB_FILL_STYLE_DEFAULT_ANGLE2 (135)


/**
 * @brief A default value when uninitialized, unused from the input file, or logic errors occur
 */
#define BB_FILL_STYLE_DEFAULT_PITCH (100)


/**
 * @brief A default value when uninitialized, unused from the input file, or logic errors occur
 */
#define BB_FILL_STYLE_DEFAULT_WIDTH (10)


/**
 * @brief
 */
typedef struct _BbFillStyle BbFillStyle;

struct _BbFillStyle
{
    BbFillType type;
    int width;
    int angle[2];
    int pitch[2];
};


BbFillStyle*
bb_fill_style_copy(BbFillStyle* style);


void
bb_fill_style_free(BbFillStyle* style);


/**
 * @brief Convert parameters into the fill style
 *
 * This function reads the values consecutively from the first index.
 *
 * Unused values in the input file are initialized with default values
 *
 * @param params A BbParams
 * @param index The index of the first fill style parameter: PARAM_FILL_TYPE
 * @param fill_style The fill style to initialize with values
 * @param error Any error encountered during conversion
 */
void
bb_fill_style_from_params(BbParams *params, int index, BbFillStyle *fill_style, GError **error);


int
bb_fill_style_get_fill_width_for_file(BbFillStyle *fill_style);


int
bb_fill_style_get_fill_angle_1_for_file(BbFillStyle *fill_style);


int
bb_fill_style_get_fill_pitch_1_for_file(BbFillStyle *fill_style);


int
bb_fill_style_get_fill_angle_2_for_file(BbFillStyle *fill_style);


int
bb_fill_style_get_fill_pitch_2_for_file(BbFillStyle *fill_style);


BbFillStyle*
bb_fill_style_new();


#endif
