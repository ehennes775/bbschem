#ifndef __BBTEXTPRESENTATION__
#define __BBTEXTPRESENTATION__
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


#define BB_TEXT_PRESENTATION_DEFAULT (BB_TEXT_PRESENTATION_BOTH)


/**
 * These items must match the gEDA file format numerically. Also, the value is used as an array index requiring
 * contiguous values starting at index 0.
 */
typedef enum _BbTextPresentation BbTextPresentation;

enum _BbTextPresentation
{
    BB_TEXT_PRESENTATION_BOTH,
    BB_TEXT_PRESENTATION_VALUE,
    BB_TEXT_PRESENTATION_NAME,
    N_TEXT_PRESENTATION
};


/**
 * Convert the text presentation from parameters
 *
 * @param params A BbParams
 * @param index The index of the parameter
 * @param error Any error encountered converting the parameter
 * @return The text presentation, or BB_TEXT_PRESENTATION_DEFAULT on an error
 */
BbTextPresentation
bb_text_presentation_from_params(BbParams *params, int index, GError **error);


/**
 * @brief Checks the text presentation for validity
 *
 * @param presentation A BbTextPresentation
 * @return TRUE if the presentation represents a valid value
 */
gboolean
bb_text_presentation_is_valid(BbTextPresentation presentation);


#endif
