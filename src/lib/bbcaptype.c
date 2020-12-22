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
#include "bbcaptype.h"
#include "bberror.h"


BbCapType
bb_cap_type_from_params(BbParams *params, int index, GError **error)
{
    GError *local_error = NULL;

    int cap_type = bb_params_get_int(params, index, &local_error);

    if (local_error == NULL)
    {
        if (!bb_cap_type_is_valid(cap_type))
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_VALUE_OUT_OF_RANGE,
                "Cap type of %d out of range [%d,%d)",
                cap_type,
                0,
                N_CAP_TYPES
            );
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);

        cap_type = BB_CAP_TYPE_DEFAULT;
    }

    return cap_type;
}


gboolean
bb_cap_type_is_valid(BbCapType cap_type)
{
    return (cap_type >= 0) && (cap_type < N_CAP_TYPES);
}
