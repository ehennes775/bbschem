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
#include "bberror.h"

BbFillType
bb_fill_type_from_params(BbParams *params, int index, GError **error)
{
    GError *local_error = NULL;

    int fill_type = bb_params_get_int(params, index, &local_error);

    if (local_error == NULL)
    {
        if (!bb_fill_type_is_valid(fill_type))
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_VALUE_OUT_OF_RANGE,
                "Fill type of %d out of range [%d,%d)",
                fill_type,
                0,
                N_FILL_TYPES
            );
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);

        fill_type = BB_FILL_TYPE_DEFAULT;
    }

    return fill_type;
}


gboolean
bb_fill_type_is_valid(BbFillType fill_type)
{
    return (fill_type >= 0) && (fill_type < N_FILL_TYPES);
}


gboolean
bb_fill_type_uses_fill_width(BbFillType fill_type)
{
    switch (fill_type)
    {
        case BB_FILL_TYPE_HOLLOW:
        case BB_FILL_TYPE_SOLID:
            return FALSE;

        case BB_FILL_TYPE_MESH:
        case BB_FILL_TYPE_HATCH:
            return TRUE;

        default:
            g_return_val_if_reached(FALSE);
    }
}


gboolean
bb_fill_type_uses_first_set(BbFillType fill_type)
{
    switch (fill_type)
    {
        case BB_FILL_TYPE_HOLLOW:
        case BB_FILL_TYPE_SOLID:
            return FALSE;

        case BB_FILL_TYPE_MESH:
        case BB_FILL_TYPE_HATCH:
            return TRUE;

        default:
            g_return_val_if_reached(FALSE);
    }
}


gboolean
bb_fill_type_uses_second_set(BbFillType fill_type)
{
    switch (fill_type)
    {
        case BB_FILL_TYPE_HOLLOW:
        case BB_FILL_TYPE_SOLID:
            return FALSE;

        case BB_FILL_TYPE_MESH:
            return TRUE;

        case BB_FILL_TYPE_HATCH:
            return FALSE;

        default:
            g_return_val_if_reached(FALSE);
    }
}
