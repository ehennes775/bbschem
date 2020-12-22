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
#include "bbtextalignment.h"


double
bb_text_alignment_get_alignment_x(BbTextAlignment alignment)
{
    switch (alignment)
    {
        case BB_TEXT_ALIGNMENT_LOWER_RIGHT:
        case BB_TEXT_ALIGNMENT_CENTER_RIGHT:
        case BB_TEXT_ALIGNMENT_UPPER_RIGHT:
            return 1.0;

        case BB_TEXT_ALIGNMENT_LOWER_CENTER:
        case BB_TEXT_ALIGNMENT_CENTER_CENTER:
        case BB_TEXT_ALIGNMENT_UPPER_CENTER:
            return 0.5;
            break;

        default:
            return 0.0;
    }
}


double
bb_text_alignment_get_alignment_y(BbTextAlignment alignment)
{
    switch (alignment)
    {
        case BB_TEXT_ALIGNMENT_LOWER_LEFT:
        case BB_TEXT_ALIGNMENT_LOWER_CENTER:
        case BB_TEXT_ALIGNMENT_LOWER_RIGHT:
            return 1.0;

        case BB_TEXT_ALIGNMENT_CENTER_LEFT:
        case BB_TEXT_ALIGNMENT_CENTER_CENTER:
        case BB_TEXT_ALIGNMENT_CENTER_RIGHT:
            return 0.5;

        default:
            return 0.0;
    }
}


gboolean
bb_text_alignment_is_valid(BbTextAlignment alignment)
{
    return (alignment >= 0) && (alignment < N_TEXT_ALIGNMENT);
}


