#ifndef __BBDASHTYPE__
#define __BBDASHTYPE__
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


/**
 * @brief A default value when uninitialized or logic errors occur
 */
#define BB_DASH_TYPE_DEFAULT (BB_DASH_TYPE_SOLID)


/**
 * These items must match the gEDA file format numerically.
 */
typedef enum _BbDashType BbDashType;

enum _BbDashType
{
    BB_DASH_TYPE_SOLID,
    BB_DASH_TYPE_DOTTED,
    BB_DASH_TYPE_DASHED,
    BB_DASH_TYPE_CENTER,
    BB_DASH_TYPE_PHANTOM,
    N_DASH_TYPES
};


/**
 * Convert the dash type from parameters
 *
 * @param params A BbParams
 * @param index The index of the parameter
 * @param error Any error encountered converting the parameter
 * @return The dash type, or BB_DASH_TYPE_DEFAULT on an error
 */
BbDashType
bb_dash_type_from_params(BbParams *params, int index, GError **error);


/**
 * @brief Checks the dash type for validity
 *
 * @param type A BbDashType
 * @return TRUE if the type represents a valid value
 */
gboolean
bb_dash_type_is_valid(BbDashType type);


gboolean
bb_dash_type_uses_dash_length(BbDashType dash_type);


gboolean
bb_dash_type_uses_dash_space(BbDashType dash_type);


#endif
