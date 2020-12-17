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
#include "bberror.h"


struct _BbParams
{
    gchar **params;
};


void
bb_params_free(BbParams *params)
{
    if (params != NULL)
    {
        g_strfreev(params->params);
    }

    g_slice_free(BbParams, params);
}


int
bb_params_get_int(BbParams *params, int index, GError **error)
{
    GError *local_error = NULL;
    gint64 value = 0;

    if (index >= g_strv_length(params->params))
    {
        local_error = g_error_new(
            BB_ERROR_DOMAIN,
            ERROR_TOO_FEW_PARAMETERS,
            "Too few parameters"
            );
    }
    else
    {
        const gchar *ptr0 = *(params->params + index);
        gchar *ptr1;

        value = g_ascii_strtoll(ptr0, &ptr1, 10);

        if ((value == 0) && (ptr0 == ptr1))
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_INTEGER_EXPECTED,
                "Integer expected at parameter %" G_GINT32_FORMAT,
                index
                );
        }
        else if ((value < G_MININT) || (value > G_MAXINT))
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_VALUE_OUT_OF_RANGE,
                "Value %" G_GINT64_FORMAT " out of range at parameter %" G_GINT32_FORMAT,
                value,
                index
                );
        }
    }

    if (error != NULL)
    {
        g_propagate_error(error, local_error);
    }

    return CLAMP(value, G_MININT, G_MAXINT);
}


BbParams*
bb_params_new_with_line(const char *line, GError **error)
{
    g_return_val_if_fail(line != NULL, NULL);

    BbParams *params = g_slice_new(BbParams);

    params->params = g_strsplit(line, " ", 0);

    return params;
}


gboolean
bb_params_token_matches(BbParams *params, const char *token)
{
    g_return_val_if_fail(params != NULL, FALSE);
    g_return_val_if_fail(params->params != NULL, FALSE);
    g_return_val_if_fail(params->params[0] != NULL, FALSE);
    g_return_val_if_fail(token != NULL, FALSE);

    return g_strcmp0(params->params[0], token) == 0;
}
