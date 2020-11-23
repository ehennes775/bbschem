#ifndef __BBTEXTPROPERTYEDITOR__
#define __BBTEXTPROPERTYEDITOR__
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


#define BB_TYPE_TEXT_PROPERTY_EDITOR bb_text_property_editor_get_type()
G_DECLARE_FINAL_TYPE(BbTextPropertyEditor, bb_text_property_editor, BB, TEXT_PROPERTY_EDITOR, GtkExpander)


BbMainWindow*
bb_text_property_editor_get_main_window(BbTextPropertyEditor *editor);

void
bb_text_property_editor_set_main_window(BbTextPropertyEditor *editor, BbMainWindow *window);


#endif
