#ifndef __BBERROR__
#define __BBERROR__
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

GQuark bb_error_quark();

#define BB_ERROR_DOMAIN (bb_error_quark())

enum
{
    ERROR_EXPECTED_MOVETO,
    ERROR_EXPECTED_VERSION,
    ERROR_INTEGER_EXPECTED,
    ERROR_NOT_SUPPORTED,
    ERROR_TOO_FEW_PARAMETERS,
    ERROR_UNEXPECTED_EMPTY_LINE,
    ERROR_UNEXPECTED_EOF,
    ERROR_UNKNOWN_ITEM_TOKEN,
    ERROR_UNKNOWN_PATH_COMMAND,
    ERROR_UNTERMINATED_ATTRIBUTES,
    ERROR_VALUE_OUT_OF_RANGE
};


#endif
