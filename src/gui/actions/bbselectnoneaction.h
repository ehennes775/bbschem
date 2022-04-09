#ifndef __BBSELECTNONEACTION__
#define __BBSELECTNONEACTION__
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

#define BB_TYPE_SELECT_NONE_ACTION bb_select_none_action_get_type()
G_DECLARE_FINAL_TYPE(BbSelectNoneAction, bb_select_none_action, BB, SELECT_NONE_ACTION, GObject)

BbSelectNoneAction*
bb_select_none_action_new();

GObject*
bb_select_none_action_get_receiver(BbSelectNoneAction *receiver);

void
bb_select_none_action_set_receiver(BbSelectNoneAction *receiver, GObject *object);

G_END_DECLS

#endif
