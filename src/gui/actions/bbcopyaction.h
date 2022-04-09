#ifndef __BBCOPYACTION__
#define __BBCOPYACTION__
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

#define BB_TYPE_COPY_ACTION bb_copy_action_get_type()
G_DECLARE_FINAL_TYPE(BbCopyAction, bb_copy_action, BB, COPY_ACTION, GObject)

BbCopyAction*
bb_copy_action_new();

GObject*
bb_copy_action_get_receiver(BbCopyAction *receiver);

void
bb_copy_action_set_receiver(BbCopyAction *receiver, GObject *object);

G_END_DECLS

#endif
