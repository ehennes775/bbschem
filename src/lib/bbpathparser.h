#ifndef __BBPATHPARSER__
#define __BBPATHPARSER__
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


/**
 * @brief Parse the path string into a list of BbPathCommand items
 *
 * @param input The input path string
 * @param error An errors encountered
 * @return A list of path commands, or NULL on error.
 */
GSList*
bb_path_parser_parse(const char *input, GError **error);


#endif
