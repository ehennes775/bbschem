#ifndef __BBSCHEMATICWINDOW__
#define __BBSCHEMATICWINDOW__
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
#include <bbapplyfunc.h>
#include <bbqueryfunc.h>
#include "bbdocumentwindow.h"
#include "bbdrawingtool.h"
#include "bbtoolchanger.h"


#define BB_TYPE_SCHEMATIC_WINDOW bb_schematic_window_get_type()
G_DECLARE_FINAL_TYPE(BbSchematicWindow, bb_schematic_window, BB, SCHEMATIC_WINDOW, BbDocumentWindow)


/**
 * Perform an operation to modify all items in the selection
 *
 * @param window This window
 * @param func The operation to perform on each item in the selection
 * @param user_data Data to pass through to the operation as a parameter
 */
void
bb_schematic_window_apply_selection(BbSchematicWindow *window, BbApplyFunc func, gpointer user_data);


/**
 * Indicates a the document can be reloaded
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_reload(BbSchematicWindow *window);


/**
 * Indicates a the document can be saved
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_save(BbSchematicWindow *window);


/**
 * Indicates a the document can be saved with an alternate filename
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_save_as(BbSchematicWindow *window);


/**
 * Get the drawing tool for this window
 *
 * @param window This window
 */
BbDrawingTool*
bb_schematic_window_get_drawing_tool(BbSchematicWindow *window);


/**
 * Perform an operation to query all items in the selection
 *
 * @param window This window
 * @param func The operation to perform on each item in the selection
 * @param user_data Data to pass through to the operation as a parameter
 */
void
bb_schematic_window_query_selection(BbSchematicWindow *window, BbQueryFunc func, gpointer user_data);


/**
 * Reload the underlying document
 *
 * @param window This window
 */
void
bb_schematic_window_reload(BbSchematicWindow *window, GError **error);


/**
 * Save the underlying document
 *
 * @param window This window
 */
void
bb_schematic_window_save(BbSchematicWindow *window, GCancellable *cancellable, GError **error);


/**
 * Save the underlying document
 *
 * @param window This window
 */
void
bb_schematic_window_save_as(BbSchematicWindow *window, GError **error);


/**
 * Set the drawing tool for this window
 *
 * @param window This window
 */
void
bb_schematic_window_set_drawing_tool(BbSchematicWindow *window, BbDrawingTool *tool);

void
bb_schematic_window_set_tool_changer(BbSchematicWindow *window, BbToolChanger *tool_changer);


#endif
