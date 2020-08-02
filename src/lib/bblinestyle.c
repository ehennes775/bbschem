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
#include "bblinestyle.h"
#include "bbdashtype.h"
#include "bbcaptype.h"


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
