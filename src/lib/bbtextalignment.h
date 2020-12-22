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


gboolean
bb_text_alignment_is_valid(BbTextAlignment alignment);

#endif
