#ifndef __BBTOOLPALETTE__
#define __BBTOOLPALETTE__
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
#include "bbmainwindow.h"

#define BB_TYPE_TOOL_PALETTE bb_tool_palette_get_type()
G_DECLARE_FINAL_TYPE(BbToolPalette, bb_tool_palette, BB, TOOL_PALETTE, GtkBox)


BbMainWindow*
bb_tool_palette_get_main_window(BbToolPalette *palette);

void
bb_tool_palette_set_main_window(BbToolPalette *palette, BbMainWindow *window);

#endif
