#ifndef __BBGEDAEDITOR__
#define __BBGEDAEDITOR__
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
#include <bbapplyfunc.h>
#include <bbqueryfunc.h>
#include "bbdocumentwindow.h"
#include "bbdrawingtool.h"
#include "bbtoolchanger.h"


#define BB_TYPE_GEDA_EDITOR bb_geda_editor_get_type()
G_DECLARE_FINAL_TYPE(BbGedaEditor, bb_geda_editor, BB, GEDA_EDITOR, BbDocumentWindow)


/**
 * Indicates a the document can be reloaded
 *
 * @param editor This editor
 * @return TRUE if the operation can be performed
 */
gboolean
bb_geda_editor_get_can_reload(BbGedaEditor *editor);


/**
 * Get the drawing tool for this editor
 *
 * @param editor This editor
 */
BbDrawingTool*
bb_geda_editor_get_drawing_tool(BbGedaEditor *editor);


BbGedaEditor*
bb_geda_editor_new(BbSchematic *schematic);


/**
 * Reload the underlying document
 *
 * @param editor This editor
 */
void
bb_geda_editor_reload(BbGedaEditor *editor, GError **error);


/**
 * Set the drawing tool for this editor
 *
 * @param editor This editor
 */
void
bb_geda_editor_set_drawing_tool(BbGedaEditor *editor, BbDrawingTool *tool);

void
bb_geda_editor_set_tool_changer(BbGedaEditor *editor, BbToolChanger *tool_changer);


#endif
