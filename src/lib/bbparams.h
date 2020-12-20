#ifndef __BBPARAMS__
#define __BBPARAMS__
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


typedef struct _BbParams BbParams;


/**
 * Free resources associated with BbParams
 *
 * @param params
 */
void
bb_params_free(BbParams *params);


/**
 * Parse a 32 bit integer parameter
 *
 * @param params A BbParams
 * @param index The zero based index of the parameter
 * @param error
 * @return The 32 bit integer, or 0, G_MININT, G_MAXINT on error.
 */
int
bb_params_get_int(BbParams *params, int index, GError **error);


const gchar*
bb_params_get_string(BbParams *params, int index, GError **error);


const gchar*
bb_params_get_token(BbParams *params);


/**
 *
 * Returns FALSE and leaves error unset on programming errors
 *
 * Use bb_params_free() to free release all associated resources
 *
 * @param line
 * @param error
 * @return
 */
BbParams*
bb_params_new_with_line(const char *line, GError **error);


/**
 *
 * Returns FALSE on programming errors
 *
 * @param params
 * @param token
 * @return
 */
gboolean
bb_params_token_matches(BbParams *params, const char *token);


#endif
