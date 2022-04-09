#ifndef __BBDELETEACTION__
#define __BBDELETEACTION__
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

#define BB_TYPE_DELETE_ACTION bb_delete_action_get_type()
G_DECLARE_FINAL_TYPE(BbDeleteAction, bb_delete_action, BB, DELETE_ACTION, GObject)

BbDeleteAction*
bb_delete_action_new();

GObject*
bb_delete_action_get_receiver(BbDeleteAction *receiver);

void
bb_delete_action_set_receiver(BbDeleteAction *receiver, GObject *object);

G_END_DECLS

#endif
