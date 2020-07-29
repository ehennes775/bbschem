#ifndef __BBQUERYFUNC__
#define __BBQUERYFUNC__
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
#include "bbschematicitem.h"

/**
 * A function for querying items
 *
 * @param item A schematic item
 * @param user_data User data passed in from the caller
 */
typedef gboolean (*BbQueryFunc)(BbSchematicItem *item, gpointer user_data);


/**
 * A predicate always returning TRUE
 *
 * @param item Unused
 * @param user_data Unused
 */
gboolean
bb_query_func_always(BbSchematicItem *item, gpointer user_data);


/**
 * A predicate always returning FALSE
 *
 * @param item Unused
 * @param user_data Unused
 */
gboolean
bb_query_func_never(BbSchematicItem *item, gpointer user_data);


#endif
