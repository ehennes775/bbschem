#ifndef __BBPRED__
#define __BBPRED__
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
 * A predicate function type for processing lists
 *
 * @param data The element data
 * @param user_data The data passed to the calling function
 * @return The predicate result
 */
typedef gboolean (*BbPred)(gpointer data, gpointer user_data);


/**
 * A predicate that always returns TRUE
 *
 * @param data Unused
 * @param user_data Unused
 * @return Always TRUE
 */
gboolean
bb_pred_always(gpointer data, gpointer user_data);


/**
 * A predicate that always returns FALSE
 *
 * @param data Unused
 * @param user_data Unused
 * @return Always FALSE
 */
gboolean
bb_pred_never(gpointer data, gpointer user_data);

#endif
