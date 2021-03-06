#ifndef __BBPINTYPE__
#define __BBPINTYPE__
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


#define BB_PIN_TYPE_DEFAULT (BB_PIN_TYPE_NET)


/**
 * These items must match the gEDA file format numerically.
 */
typedef enum _BbPinType BbPinType;

enum _BbPinType
{
    BB_PIN_TYPE_NET,
    BB_PIN_TYPE_BUS,
    N_PIN_TYPES
};


/**
 * Convert the pin type from parameters
 *
 * @param params A BbParams
 * @param index The index of the parameter
 * @param error Any error encountered converting the parameter
 * @return The pin type, or BB_PIN_TYPE_DEFAULT on an error
 */
BbPinType
bb_pin_type_from_params(BbParams *params, int index, GError **error);


/**
 * @brief Checks the pin type for validity
 *
 * @param type A BbPinType
 * @return TRUE if the pin type represents a valid value
 */
gboolean
bb_pin_type_is_valid(BbPinType type);


#endif
