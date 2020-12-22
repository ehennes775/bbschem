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
#include "bbtextsize.h"
#include "bberror.h"


int
bb_text_size_from_params(BbParams *params, int index, GError **error)
{
    GError *local_error = NULL;

    int size = bb_params_get_int(params, index, &local_error);

    if (local_error == NULL)
    {
        if (!bb_text_size_is_valid(size))
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_VALUE_OUT_OF_RANGE,
                "Text size of %d out of range [%d,%d]",
                size,
                BB_TEXT_SIZE_MIN,
                BB_TEXT_SIZE_MAX
                );
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);

        size = BB_TEXT_SIZE_DEFAULT;
    }

    return size;
}


/**
 *
 * @param size
 * @return
 */
gboolean
bb_text_size_is_valid(int size)
{
    return (size >= BB_TEXT_SIZE_MIN) && (size <= BB_TEXT_SIZE_MAX);
}

