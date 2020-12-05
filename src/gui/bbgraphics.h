#ifndef __BBGRAPHICS__
#define __BBGRAPHICS__
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

#define BB_TYPE_GRAPHICS bb_graphics_get_type()
G_DECLARE_FINAL_TYPE(BbGraphics, bb_graphics, BB, GRAPHICS, GObject)


cairo_t*
bb_graphics_get_cairo(BbGraphics *graphics);

GtkStyleContext*
bb_graphics_get_style(BbGraphics *graphics);

cairo_matrix_t*
bb_graphics_get_widget_matrix(BbGraphics *graphics);


/**
 *
 * @param cairo
 * @param widget_matrix A matrix for converting widget coordinates to window coordinates
 * @param style
 * @return
 */
BbGraphics*
bb_graphics_new(cairo_t *cairo, cairo_matrix_t *widget_matrix, GtkStyleContext *style);


/**
 * Draw a selection box
 *
 * @param graphics
 * @param x0 The x coordinate of the first corner in widget coordinates
 * @param y0 The y coordinate of the first corner in widget coordinates
 * @param x1 The x coordinate of the second corner in widget coordinates
 * @param y1 The y coordinate of the second corner in widget coordinates
 */
void
bb_graphics_draw_select_box(BbGraphics *graphics, int x0, int y0, int x1, int y1);


/**
 * Draw a zoom box
 *
 * @param graphics
 * @param x0 The x coordinate of the first corner in widget coordinates
 * @param y0 The y coordinate of the first corner in widget coordinates
 * @param x1 The x coordinate of the second corner in widget coordinates
 * @param y1 The y coordinate of the second corner in widget coordinates
 */
void
bb_graphics_draw_zoom_box(BbGraphics *graphics, int x0, int y0, int x1, int y1);

#endif
