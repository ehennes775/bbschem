#ifndef __BBCELLRENDERERCOLORSWATCH__
#define __BBCELLRENDERERCOLORSWATCH__
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

#define BB_TYPE_CELL_RENDERER_COLOR_SWATCH bb_cell_renderer_color_swatch_get_type()
G_DECLARE_FINAL_TYPE(BbCellRendererColorSwatch, bb_cell_renderer_color_swatch, BB, CELL_RENDERER_COLOR_SWATCH, GtkCellRendererText)

const GdkRGBA*
bb_cell_renderer_color_swatch_get_color(BbCellRendererColorSwatch *renderer);

void
bb_cell_renderer_color_swatch_set_color(BbCellRendererColorSwatch *renderer, GdkRGBA *color);

#endif
