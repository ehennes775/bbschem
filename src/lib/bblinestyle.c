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
#include "bblinestyle.h"
#include "bbdashtype.h"
#include "bbcaptype.h"

#define BB_LINE_STYLE_UNUSED_NUMBER (-1)


BbLineStyle*
bb_line_style_copy(BbLineStyle* style)
{
    return g_slice_dup(BbLineStyle, style);
}


void
bb_line_style_free(BbLineStyle* style)
{
    g_slice_free(BbLineStyle, style);
}


void
bb_line_style_from_params(BbParams *params, int index, BbLineStyle *line_style, GError **error)
{
    GError *local_error = NULL;

    line_style->line_width = bb_params_get_int(params, index++, &local_error);

    if (local_error == NULL)
    {
        line_style->cap_type = bb_cap_type_from_params(params, index++, &local_error);
    }

    if (local_error == NULL)
    {
        line_style->dash_type = bb_dash_type_from_params(params, index++, &local_error);
    }

    if (local_error == NULL)
    {
        line_style->dash_length = bb_params_get_int(params, index++, &local_error);

        if (!bb_dash_type_uses_dash_length(line_style->dash_type))
        {
            line_style->dash_length = 100;
        }
    }

    if (local_error == NULL)
    {
        line_style->dash_space = bb_params_get_int(params, index++, &local_error);

        if (!bb_dash_type_uses_dash_space(line_style->dash_type))
        {
            line_style->dash_space = 100;
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
    }
}


int
bb_line_style_get_dash_length_for_file(BbLineStyle *line_style)
{
    g_return_val_if_fail(line_style != NULL, BB_LINE_STYLE_UNUSED_NUMBER);

    return bb_dash_type_uses_dash_length(line_style->dash_type) ? line_style->dash_length : BB_LINE_STYLE_UNUSED_NUMBER;
}


int
bb_line_style_get_dash_space_for_file(BbLineStyle *line_style)
{
    g_return_val_if_fail(line_style != NULL, BB_LINE_STYLE_UNUSED_NUMBER);

    return bb_dash_type_uses_dash_space(line_style->dash_type) ? line_style->dash_space : BB_LINE_STYLE_UNUSED_NUMBER;
}


BbLineStyle*
bb_line_style_new()
{
    BbLineStyle *style = g_slice_new(BbLineStyle);

    style->line_width = 10;
    style->cap_type = BB_CAP_TYPE_NONE;
    style->dash_type = BB_DASH_TYPE_SOLID;
    style->dash_length = 10;
    style->dash_space = 10;

    return style;
}
