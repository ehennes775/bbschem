#ifndef __BBTEXTSIZE__
#define __BBTEXTSIZE__
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


#define BB_TEXT_SIZE_DEFAULT (10)


/**
 * @brief Minimum text size, inclusive, defined by the file format
 */
#define BB_TEXT_SIZE_MIN (2)


/**
 * @brief Maximum text size, inclusive, defined by the file format
 */
#define BB_TEXT_SIZE_MAX (G_MAXINT)


/**
 * @brief Checks the text size for validity
 *
 * @param size A text size in points
 * @return TRUE if the text size represents a valid value
 */
gboolean
bb_text_size_is_valid(int size);

#endif
