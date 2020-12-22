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
#include "bbfillstyle.h"
#include "bbangle.h"


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


void
bb_fill_style_from_params(BbParams *params, int index, BbFillStyle *fill_style, GError **error)
{
    GError *local_error = NULL;

    fill_style->type = bb_fill_type_from_params(params, index++, &local_error);

    if (local_error == NULL)
    {
        fill_style->width = bb_params_get_int(params, index++, &local_error);

        if (local_error == NULL && !bb_fill_type_uses_fill_width(fill_style->type))
        {
            fill_style->width = BB_FILL_STYLE_DEFAULT_WIDTH;
        }
    }

    if (local_error == NULL)
    {
        fill_style->angle[0] = bb_params_get_int(params, index++, &local_error);

        if (local_error == NULL && !bb_fill_type_uses_first_set(fill_style->type))
        {
            fill_style->width = BB_FILL_STYLE_DEFAULT_ANGLE1;
        }
    }

    if (local_error == NULL)
    {
        fill_style->pitch[0] = bb_params_get_int(params, index++, &local_error);

        if (local_error == NULL && !bb_fill_type_uses_first_set(fill_style->type))
        {
            fill_style->pitch[0] = BB_FILL_STYLE_DEFAULT_PITCH;
        }
    }

    if (local_error == NULL)
    {
        fill_style->angle[1] = bb_params_get_int(params, index++, &local_error);

        if (local_error == NULL && !bb_fill_type_uses_second_set(fill_style->type))
        {
            fill_style->angle[1] = BB_FILL_STYLE_DEFAULT_ANGLE2;
        }
    }

    if (local_error == NULL)
    {
        fill_style->pitch[1] = bb_params_get_int(params, index++, &local_error);

        if (local_error == NULL && !bb_fill_type_uses_second_set(fill_style->type))
        {
            fill_style->pitch[1] = BB_FILL_STYLE_DEFAULT_PITCH;
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
    }
}


int
bb_fill_style_get_fill_width_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    return bb_fill_type_uses_fill_width(fill_style->type)
        ? fill_style->width
        : BB_FILL_STYLE_UNUSED_NUMBER;
}


int
bb_fill_style_get_fill_angle_1_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    /*
     * An angle of -1 is a both valid angle and an indicator for an unused number. For maximum compatibility with other
     * programs, if an angle of -1 is output, the angle is normalized to 359. Similarly, on input, the value of -1
     * is not interpreted. The input only uses the fill_type for logic.
     */

    return bb_fill_type_uses_second_set(fill_style->type)
        ? fill_style->angle[0] == BB_FILL_STYLE_UNUSED_NUMBER
            ? bb_angle_normalize(fill_style->angle[0])
            : fill_style->angle[0]
        : BB_FILL_STYLE_UNUSED_NUMBER;
}


int
bb_fill_style_get_fill_pitch_1_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    return bb_fill_type_uses_first_set(fill_style->type)
        ? fill_style->pitch[0]
        : BB_FILL_STYLE_UNUSED_NUMBER;
}


int
bb_fill_style_get_fill_angle_2_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    /*
     * An angle of -1 is a both valid angle and an indicator for an unused number. For maximum compatibility with other
     * programs, if an angle of -1 is output, the angle is normalized to 359. Similarly, on input, the value of -1
     * is not interpreted. The input only uses the fill_type for logic.
     */

    return bb_fill_type_uses_second_set(fill_style->type)
        ? fill_style->angle[1] == BB_FILL_STYLE_UNUSED_NUMBER
            ? bb_angle_normalize(fill_style->angle[1])
            : fill_style->angle[1]
        : BB_FILL_STYLE_UNUSED_NUMBER;
}


int
bb_fill_style_get_fill_pitch_2_for_file(BbFillStyle *fill_style)
{
    g_return_val_if_fail(fill_style != NULL, BB_FILL_STYLE_UNUSED_NUMBER);

    return bb_fill_type_uses_second_set(fill_style->type)
        ? fill_style->pitch[1]
        : BB_FILL_STYLE_UNUSED_NUMBER;
}


BbFillStyle*
bb_fill_style_new()
{
    BbFillStyle *style = g_slice_new(BbFillStyle);

    style->type = BB_FILL_TYPE_DEFAULT;
    style->width = BB_FILL_STYLE_DEFAULT_WIDTH;
    style->angle[0] = BB_FILL_STYLE_DEFAULT_ANGLE1;
    style->angle[1] = BB_FILL_STYLE_DEFAULT_ANGLE2;
    style->pitch[0] = BB_FILL_STYLE_DEFAULT_PITCH;
    style->pitch[1] = BB_FILL_STYLE_DEFAULT_PITCH;

    return style;
}
