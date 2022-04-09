#ifndef __BBRELOADACTION__
#define __BBRELOADACTION__
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

#define BB_TYPE_RELOAD_ACTION bb_reload_action_get_type()
G_DECLARE_FINAL_TYPE(BbReloadAction, bb_reload_action, BB, RELOAD_ACTION, GObject)

BbReloadAction*
bb_reload_action_new();

GObject*
bb_reload_action_get_receiver(BbReloadAction *action);

void
bb_reload_action_set_receiver(BbReloadAction *action, GObject *receiver);

G_END_DECLS

#endif
