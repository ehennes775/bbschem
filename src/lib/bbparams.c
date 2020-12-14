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
