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
#include "bbschematicwrapper.h"

#define BB_TYPE_SCHEMATIC_WINDOW bb_schematic_window_get_type()
G_DECLARE_FINAL_TYPE(BbSchematicWindow, bb_schematic_window, BB, SCHEMATIC_WINDOW, BbDocumentWindow)


BbSchematicWrapper*
bb_schematic_window_get_schematic_wrapper(BbSchematicWindow *window);


/**
 * Perform an operation to modify all items in the selection
 *
 * @param window
 * @param func The operation to perform on each item in the selection
 * @param user_data Data to pass through to the operation as a parameter
 */
void
bb_schematic_window_apply_selection(BbSchematicWindow *window, BbApplyFunc func, gpointer user_data);


/**
 * Perform an operation to query all items in the selection
 *
 * @param window
 * @param func The operation to perform on each item in the selection
 * @param user_data Data to pass through to the operation as a parameter
 */
void
bb_schematic_window_query_selection(BbSchematicWindow *window, BbQueryFunc func, gpointer user_data);


void
bb_schematic_window_set_schematic_wrapper(BbSchematicWindow *window, BbSchematicWrapper *wrapper);

#endif
