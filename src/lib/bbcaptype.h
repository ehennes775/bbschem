#ifndef __BBCAPTYPE__
#define __BBCAPTYPE__
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
#include "bbparams.h"


#define BB_CAP_TYPE_DEFAULT (BB_CAP_TYPE_NONE)


typedef enum _BbCapType BbCapType;

enum _BbCapType
{
    BB_CAP_TYPE_NONE,
    BB_CAP_TYPE_SQUARE,
    BB_CAP_TYPE_ROUND,
    N_CAP_TYPES
};

/**
 * Convert the cap type from parameters
 *
 * @param params A BbParams
 * @param index The index of the parameter
 * @param error Any error encountered converting the parameter
 * @return The cap type, or BB_CAP_TYPE_DEFAULT on an error
 */
BbCapType
bb_cap_type_from_params(BbParams *params, int index, GError **error);


/**
 * @brief Checks the cap type for validity
 *
 * @param type A BbCapType
 * @return TRUE if the type represents a valid value
 */
gboolean
bb_cap_type_is_valid(BbCapType type);


#endif
