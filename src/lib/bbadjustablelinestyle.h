#ifndef __BBADJUSTABLELINESTYLE__
#define __BBADJUSTABLELINESTYLE__
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

#define BB_TYPE_ADJUSTABLE_LINE_STYLE bb_adjustable_line_style_get_type()
G_DECLARE_INTERFACE(BbAdjustableLineStyle, bb_adjustable_line_style, BB, ADJUSTABLE_LINE_STYLE, GObject)


struct _BbAdjustableLineStyleInterface
{
    GTypeInterface parent;
};


int
bb_adjustable_line_style_get_cap_type(BbAdjustableLineStyle *item);


int
bb_adjustable_line_style_get_dash_length(BbAdjustableLineStyle *item);


int
bb_adjustable_line_style_get_dash_space(BbAdjustableLineStyle *item);


int
bb_adjustable_line_style_get_dash_type(BbAdjustableLineStyle *item);


int
bb_adjustable_line_style_get_line_width(BbAdjustableLineStyle *item);


void
bb_adjustable_line_style_set_cap_type(BbAdjustableLineStyle *item, int type);


void
bb_adjustable_line_style_set_dash_length(BbAdjustableLineStyle *item, int length);


void
bb_adjustable_line_style_set_dash_space(BbAdjustableLineStyle *item, int space);


void
bb_adjustable_line_style_set_dash_type(BbAdjustableLineStyle *item, int type);


void
bb_adjustable_line_style_set_line_width(BbAdjustableLineStyle *item, int width);


#endif
