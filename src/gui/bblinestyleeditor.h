#ifndef __BBLINESTYLEEDITOR__
#define __BBLINESTYLEEDITOR__
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


#define BB_TYPE_LINE_STYLE_EDITOR bb_line_style_editor_get_type()
G_DECLARE_FINAL_TYPE(BbLineStyleEditor, bb_line_style_editor, BB, LINE_STYLE_EDITOR, GtkExpander)


BbMainWindow*
bb_line_style_editor_get_main_window(BbLineStyleEditor *editor);

void
bb_line_style_editor_set_main_window(BbLineStyleEditor *editor, BbMainWindow *window);


#endif
