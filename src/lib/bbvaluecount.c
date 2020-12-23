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
#include "bbvaluecount.h"


BbValueCount
bb_value_count_from_count(int count)
{
    g_return_val_if_fail(count >= 0, 0);

    return CLAMP(count, BB_VALUE_COUNT_NONE, BB_VALUE_COUNT_MANY);
}


gboolean
bb_value_count_inconsistent(BbValueCount count)
{
    g_return_val_if_fail(count >= BB_VALUE_COUNT_NONE, FALSE);
    g_return_val_if_fail(count <= BB_VALUE_COUNT_MANY, FALSE);

    return (count != BB_VALUE_COUNT_ONE);
}


gboolean
bb_value_count_sensitive(BbValueCount count)
{
    g_return_val_if_fail(count >= BB_VALUE_COUNT_NONE, FALSE);
    g_return_val_if_fail(count <= BB_VALUE_COUNT_MANY, FALSE);

    return (count != BB_VALUE_COUNT_NONE);
}
