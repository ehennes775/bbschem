#ifndef BBSCHEM_PROPERTYID_H
#define BBSCHEM_PROPERTYID_H
/*
 * bbschem
 * Copyright (C) 2022 Edward C. Hennessy
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

namespace bb
{
    enum class PropertyId
    {
        CIRCLE_CENTER_X,    /**< The center x coordinate property for all circle based items */
        CIRCLE_CENTER_Y,    /**< The center y coordinate property for all circle based items */
        CIRCLE_RADIUS,      /**< The radius property for all circle based items */

        LINE_X0,            /**< The first x coordinate property for all line based items */
        LINE_X1,            /**< The first y coordinate property for all line based items */
        LINE_Y0,            /**< The second x coordinate property for all line based items */
        LINE_Y1             /**< The second y coordinate property for all line based items */
    };
}

#endif
