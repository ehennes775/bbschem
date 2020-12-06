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
#include "bbfillstyle.h"


#define BB_FILL_STYLE_UNUSED_NUMBER (-1)


BbFillStyle*
bb_fill_style_copy(BbFillStyle* style)
{
    g_slice_dup(BbFillStyle, style);
}


void
bb_fill_style_free(BbFillStyle* style)
{
    g_slice_free(BbFillStyle, style);
}

int
bb_fill_style_get_fill_width_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    return bb_fill_type_uses_fill_width(fill_style->type) ? fill_style->width : BB_FILL_STYLE_UNUSED_NUMBER;
}


int
bb_fill_style_get_fill_angle_1_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    return bb_fill_type_uses_first_set(fill_style->type) ? fill_style->angle[0] : BB_FILL_STYLE_UNUSED_NUMBER;
}


int
bb_fill_style_get_fill_pitch_1_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    return bb_fill_type_uses_first_set(fill_style->type) ? fill_style->pitch[0] : BB_FILL_STYLE_UNUSED_NUMBER;
}


int
bb_fill_style_get_fill_angle_2_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    return bb_fill_type_uses_second_set(fill_style->type) ? fill_style->angle[1] : BB_FILL_STYLE_UNUSED_NUMBER;
}


int
bb_fill_style_get_fill_pitch_2_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    return bb_fill_type_uses_second_set(fill_style->type) ? fill_style->pitch[1] : BB_FILL_STYLE_UNUSED_NUMBER;
}


BbFillStyle*
bb_fill_style_new()
{
    BbFillStyle *style = g_slice_new(BbFillStyle);

    style->type = BB_FILL_TYPE_HOLLOW;
    style->width = 10;
    style->angle[0] = 45;
    style->angle[1] = 135;
    style->pitch[0] = 100;
    style->pitch[1] = 100;

    return style;
}
