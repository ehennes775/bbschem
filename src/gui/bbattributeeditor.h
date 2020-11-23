#ifndef __BBATTRIBUTEEDITOR__
#define __BBATTRIBUTEEDITOR__
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
#include "bbdocumentwindow.h"


#define BB_TYPE_ATTRIBUTE_EDITOR bb_attribute_editor_get_type()
G_DECLARE_FINAL_TYPE(BbAttributeEditor, bb_attribute_editor, BB, ATTRIBUTE_EDITOR, GtkBox)

BbDocumentWindow*
bb_attribute_editor_get_document_window(BbAttributeEditor *editor);

void
bb_attribute_editor_set_document_window(BbAttributeEditor *editor, BbDocumentWindow *window);

#endif
