#ifndef __BBTEXTALIGNMENT__
#define __BBTEXTALIGNMENT__
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
 * These items must match the gEDA file format numerically.
 */
typedef enum _BbTextAlignment BbTextAlignment;

enum _BbTextAlignment
{
    BB_TEXT_ALIGNMENT_LOWER_LEFT,
    BB_TEXT_ALIGNMENT_CENTER_LEFT,
    BB_TEXT_ALIGNMENT_UPPER_LEFT,
    BB_TEXT_ALIGNMENT_LOWER_CENTER,
    BB_TEXT_ALIGNMENT_CENTER_CENTER,
    BB_TEXT_ALIGNMENT_UPPER_CENTER,
    BB_TEXT_ALIGNMENT_LOWER_RIGHT,
    BB_TEXT_ALIGNMENT_CENTER_RIGHT,
    BB_TEXT_ALIGNMENT_UPPER_RIGHT,
    N_TEXT_ALIGNMENT
};


/**
 * @brief Convert to a floating point representation of the horizontal alignment
 *
 * @param alignment A BbTextAlignment
 * @return Floating point representation of the horizontal alignment
 * <table>
 * <tr>
 * <th>Alignment</th>
 * <th>Value</th>
 * </tr>
 * <tr>
 * <td>left</td>
 * <td>0.0</td>
 * </tr>
 * <tr>
 * <td>middle</td>
 * <td>0.5</td>
 * </tr>
 * <tr>
 * <td>right</td>
 * <td>1.0</td>
 * </tr>
 * </table>
 */
double
bb_text_alignment_get_alignment_x(BbTextAlignment alignment);


/**
 * @brief Convert to a floating point representation of the vertical alignment
 *
 * @param alignment A BbTextAlignment
 * @return Floating point representation of the vertical alignment
 * <table>
 * <tr>
 * <th>Alignment</th>
 * <th>Value</th>
 * </tr>
 * <tr>
 * <td>upper</td>
 * <td>0.0</td>
 * </tr>
 * <tr>
 * <td>middle</td>
 * <td>0.5</td>
 * </tr>
 * <tr>
 * <td>lower</td>
 * <td>1.0</td>
 * </tr>
 * </table>
 */
double
bb_text_alignment_get_alignment_y(BbTextAlignment alignment);


/**
 * @brief Checks the text alignment for validity
 *
 * @param alignment A BbTextAlignment
 * @return TRUE if the alignment represents a valid value
 */
gboolean
bb_text_alignment_is_valid(BbTextAlignment alignment);

#endif
