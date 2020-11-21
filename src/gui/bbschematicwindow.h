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
#include <src/lib/bbapplyfunc.h>
#include <src/lib/bbqueryfunc.h>
#include "bbdocumentwindow.h"


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
 * Copy the selection to the clipboard
 *
 * @param window This window
 */
void
bb_schematic_window_copy(BbSchematicWindow *window);


/**
 * Cut the selection and place in the clipboard
 *
 * @param window This window
 */
void
bb_schematic_window_cut(BbSchematicWindow *window);


/**
 * Indicates a copy operation could be performed
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_copy(BbSchematicWindow *window);


/**
 * Indicates a cut operation could be performed
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_cut(BbSchematicWindow *window);


/**
 * Indicates a delete operation could be performed
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_delete(BbSchematicWindow *window);


/**
 * Indicates a paste operation could be performed
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_paste(BbSchematicWindow *window);


/**
 * Indicates a redo operation could be performed
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_redo(BbSchematicWindow *window);


/**
 * Indicates a select all operation could be performed
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_select_all(BbSchematicWindow *window);


/**
 * Indicates clearing the selection operation could be performed
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_select_none(BbSchematicWindow *window);


/**
 * Indicates an undo operation could be performed
 *
 * @param window This window
 * @return TRUE if the operation can be performed
 */
gboolean
bb_schematic_window_get_can_undo(BbSchematicWindow *window);


/**
 * Paste the contents of the clipboard
 *
 * @param window This window
 */
void
bb_schematic_window_paste(BbSchematicWindow *window);


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
 * Redo the last undo operation
 *
 * @param window This window
 */
void
bb_schematic_window_select_redo(BbSchematicWindow *window);


/**
 * Select the entire document
 *
 * @param window This window
 */
void
bb_schematic_window_select_all(BbSchematicWindow *window);


/**
 * Clear the selection
 *
 * @param window This window
 */
void
bb_schematic_window_select_none(BbSchematicWindow *window);


/**
 * Undo the last operation
 *
 * @param window This window
 */
void
bb_schematic_window_select_undo(BbSchematicWindow *window);


#endif
