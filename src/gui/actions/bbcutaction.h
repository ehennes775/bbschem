#ifndef __BBCUTACTION__
#define __BBCUTACTION__
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

G_BEGIN_DECLS

#define BB_TYPE_CUT_ACTION bb_cut_action_get_type()
G_DECLARE_FINAL_TYPE(BbCutAction, bb_cut_action, BB, CUT_ACTION, GObject)

BbCutAction*
bb_cut_action_new(GtkClipboard *clipboard);

GtkClipboard *
bb_cut_action_get_clipboard(BbCutAction *action);

GObject*
bb_cut_action_get_receiver(BbCutAction *action);

void
bb_cut_action_set_clipboard(BbCutAction *action, GtkClipboard *clipboard);

void
bb_cut_action_set_receiver(BbCutAction *action, GObject *object);

G_END_DECLS

#endif
