#ifndef __BBLINESTYLE__
#define __BBLINESTYLE__
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
#include "bbcaptype.h"
#include "bbdashtype.h"
#include "bbparams.h"


typedef struct _BbLineStyle BbLineStyle;

struct _BbLineStyle
{
    int line_width;
    BbCapType cap_type;
    BbDashType dash_type;
    int dash_length;
    int dash_space;
};


BbLineStyle*
bb_line_style_copy(BbLineStyle* style);


void
bb_line_style_free(BbLineStyle* style);

void
bb_line_style_from_params(BbParams *params, int index, BbLineStyle *line_style, GError **error);


int
bb_line_style_get_dash_length_for_file(BbLineStyle *line_style);

int
bb_line_style_get_dash_space_for_file(BbLineStyle *line_style);

BbLineStyle*
bb_line_style_new();


#endif
