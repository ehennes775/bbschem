#ifndef __BBUNDOACTION__
#define __BBUNDOACTION__
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
#include "bbmainwindow.h"

G_BEGIN_DECLS

#define BB_TYPE_UNDO_ACTION bb_undo_action_get_type()
G_DECLARE_FINAL_TYPE(BbUndoAction, bb_undo_action, BB, UNDO_ACTION, GObject)

BbUndoAction*
bb_undo_action_new();

GObject*
bb_undo_action_get_receiver(BbUndoAction *action);

void
bb_undo_action_set_receiver(BbUndoAction *action, GObject *receiver);

G_END_DECLS

#endif
