#ifndef __BBGRAPHICPATH__
#define __BBGRAPHICPATH__
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
#include "bbgedaitem.h"


#define BB_GEDA_PATH_TOKEN "H"


#define BB_TYPE_GEDA_PATH bb_geda_path_get_type()
G_DECLARE_FINAL_TYPE(BbGedaPath, bb_geda_path, BB, GEDA_PATH, BbGedaItem)


/**
 * @brief Get the line count from the parameters
 *
 * Returns the number of lines of text following the parameter line inside a gEDA file.
 *
 * @param params The parameters to extract the line count from
 * @param error Any errors encountered
 * @return A valid line count (result &gt; 0) or an error (result &lt;= 0)
 */
int
bb_geda_path_get_line_count(BbParams *params, GError **error);


BbGedaPath*
bb_geda_path_new_with_params(BbParams *params, GSList *commands, GError **error);

#endif
