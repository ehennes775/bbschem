#ifndef __BBVALUECOUNT__
#define __BBVALUECOUNT__
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

typedef enum _BbValueCount BbValueCount;

enum _BbValueCount
{
    BB_VALUE_COUNT_NONE,

    BB_VALUE_COUNT_ONE,

    BB_VALUE_COUNT_MANY
};


BbValueCount
bb_value_count_from_count(int count);


gboolean
bb_value_count_inconsistent(BbValueCount count);


gboolean
bb_value_count_sensitive(BbValueCount count);


#endif
