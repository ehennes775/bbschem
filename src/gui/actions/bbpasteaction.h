#ifndef __BBPASTEACTION__
#define __BBPASTEACTION__
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

#include <glib-object.h>
#include "bbmainwindow.h"

G_BEGIN_DECLS

#define BB_TYPE_PASTE_ACTION bb_paste_action_get_type()
G_DECLARE_FINAL_TYPE(BbPasteAction, bb_paste_action, BB, PASTE_ACTION, GObject)

BbPasteAction*
bb_paste_action_new(GtkClipboard *clipboard);

GtkClipboard *
bb_paste_action_get_clipboard(BbPasteAction *action);

GObject*
bb_paste_action_get_receiver(BbPasteAction *receiver);

void
bb_paste_action_set_clipboard(BbPasteAction *action, GtkClipboard *clipboard);

void
bb_paste_action_set_receiver(BbPasteAction *receiver, GObject *object);

G_END_DECLS

#endif
