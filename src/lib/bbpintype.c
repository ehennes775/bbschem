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
#include "bbpintype.h"


BbPinType
bb_pin_type_from_params(BbParams *params, int index, GError **error)
{
    GError *local_error = NULL;

    int type = bb_params_get_int(params, index, &local_error);

    if (local_error == NULL)
    {
        if (!bb_pin_type_is_valid(type))
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_VALUE_OUT_OF_RANGE,
                "Pin type of %d out of range [%d,%d)",
                type,
                0,
                N_PIN_TYPES
            );
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);

        type = BB_PIN_TYPE_DEFAULT;
    }

    return type;
}


gboolean
bb_pin_type_is_valid(BbPinType type)
{
    return (type >= 0) && (type < N_PIN_TYPES);
}

