#ifndef __BBFILLTYPE__
#define __BBFILLTYPE__
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
#include "bbparams.h"


#define BB_FILL_TYPE_DEFAULT (BB_FILL_TYPE_HOLLOW)


typedef enum _BbFillType BbFillType;

enum _BbFillType
{
    BB_FILL_TYPE_HOLLOW,
    BB_FILL_TYPE_SOLID,
    BB_FILL_TYPE_MESH,
    BB_FILL_TYPE_HATCH,
    N_FILL_TYPES
};


/**
 * Convert the fill type from parameters
 *
 * @param params A BbParams
 * @param index The index of the parameter
 * @param error Any error encountered converting the parameter
 * @return The fill type, or BB_FILL_TYPE_DEFAULT on an error
 */
BbFillType
bb_fill_type_from_params(BbParams *params, int index, GError **error);


/**
 * @brief Checks the fill type for validity
 *
 * @param type A BbFillType
 * @return TRUE if the type represents a valid value
 */
gboolean
bb_fill_type_is_valid(BbFillType type);


gboolean
bb_fill_type_uses_fill_width(BbFillType fill_type);


gboolean
bb_fill_type_uses_first_set(BbFillType fill_type);


gboolean
bb_fill_type_uses_second_set(BbFillType fill_type);

#endif