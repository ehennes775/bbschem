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
#include "bberror.h"
#include "bbtextpresentation.h"


BbTextPresentation
bb_text_presentation_from_params(BbParams *params, int index, GError **error)
{
    GError *local_error = NULL;

    int presentation = bb_params_get_int(params, index, &local_error);

    if (local_error == NULL)
    {
        if (!bb_text_presentation_is_valid(presentation))
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_VALUE_OUT_OF_RANGE,
                "Text presentation of %d out of range [%d,%d)",
                presentation,
                0,
                N_TEXT_PRESENTATION
                );
        }
    }

    if (error != NULL)
    {
        g_propagate_error(error, local_error);

        presentation = BB_TEXT_PRESENTATION_DEFAULT;
    }

    return presentation;
}


gboolean
bb_text_presentation_is_valid(BbTextPresentation presentation)
{
    return (presentation >= 0) && (presentation < N_TEXT_PRESENTATION);
}