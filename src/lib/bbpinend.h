#ifndef __BBPINEND__
#define __BBPINEND__
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


#define BB_PIN_END_DEFAULT (0)

#define BB_PIN_END_MAX (1)

#define BB_PIN_END_MIN (0)

/**
 *
 */
typedef int BbPinEnd;


/**
 * Convert the pin end from parameters
 *
 * @param params A BbParams
 * @param index The index of the parameter
 * @param error Any error encountered converting the parameter
 * @return The pin end, or BB_PIN_END_DEFAULT on an error
 */
BbPinEnd
bb_pin_end_from_params(BbParams *params, int index, GError **error);


/**
 * @brief Checks the pin end for validity
 *
 * @param pin_end A BbPinEnd
 * @return TRUE if the pin end represents a valid value
 */
gboolean
bb_pin_end_is_valid(BbPinEnd pin_end);


#endif
