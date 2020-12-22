#ifndef __BBTEXTVISIBILITY__
#define __BBTEXTVISIBILITY__
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


#define BB_TEXT_VISIBILITY_DEFAULT (BB_TEXT_VISIBILITY_VISIBLE)


/**
 * These items must match the gEDA file format numerically.
 */
typedef enum _BbTextVisibility BbTextVisibility;

enum _BbTextVisibility
{
    BB_TEXT_VISIBILITY_INVISIBLE,
    BB_TEXT_VISIBILITY_VISIBLE,
    N_TEXT_VISIBILITY
};


/**
 * Convert the text visibility from parameters
 *
 * @param params A BbParams
 * @param index The index of the parameter
 * @param error Any error encountered converting the parameter
 * @return The text visibility, or BB_TEXT_VISIBILITY_DEFAULT on an error
 */
BbTextVisibility
bb_text_visibility_from_params(BbParams *params, int index, GError **error);


/**
 * @brief Checks the text visibility for validity
 *
 * @param visibility A BbTextVisibility
 * @return TRUE if the text visibility represents a valid value
 */
gboolean
bb_text_visibility_is_valid(BbTextVisibility visibility);

#endif
