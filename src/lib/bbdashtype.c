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
#include "bbdashtype.h"
#include "bberror.h"


BbDashType
bb_dash_type_from_params(BbParams *params, int index, GError **error)
{
    GError *local_error = NULL;

    int dash_type = bb_params_get_int(params, index, &local_error);

    if (local_error == NULL)
    {
        if (!bb_dash_type_is_valid(dash_type))
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_VALUE_OUT_OF_RANGE,
                "Dash type of %d out of range [%d,%d)",
                dash_type,
                0,
                N_DASH_TYPES
            );
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);

        dash_type = BB_DASH_TYPE_DEFAULT;
    }

    return dash_type;
}


gboolean
bb_dash_type_is_valid(BbDashType dash_type)
{
    return (dash_type >= 0) && (dash_type < N_DASH_TYPES);
}


gboolean
bb_dash_type_uses_dash_length(BbDashType dash_type)
{
    switch (dash_type)
    {
        case BB_DASH_TYPE_SOLID:
        case BB_DASH_TYPE_DOTTED:
            return FALSE;

        case BB_DASH_TYPE_DASHED:
        case BB_DASH_TYPE_CENTER:
        case BB_DASH_TYPE_PHANTOM:
            return TRUE;

        default:
            g_return_val_if_reached(FALSE);
    }
}


gboolean
bb_dash_type_uses_dash_space(BbDashType dash_type)
{
    switch (dash_type)
    {
        case BB_DASH_TYPE_SOLID:
            return FALSE;

        case BB_DASH_TYPE_DOTTED:
        case BB_DASH_TYPE_DASHED:
        case BB_DASH_TYPE_CENTER:
        case BB_DASH_TYPE_PHANTOM:
            return TRUE;

        default:
            g_return_val_if_reached(FALSE);
    }
}
