#ifndef __BBOPENACTION__
#define __BBOPENACTION__
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

#define BB_TYPE_OPEN_ACTION bb_open_action_get_type()
G_DECLARE_FINAL_TYPE(BbOpenAction, bb_open_action, BB, OPEN_ACTION, GObject)

BbMainWindow*
bb_open_action_get_window(BbOpenAction *open_action);


BbOpenAction*
bb_open_action_new(BbMainWindow *window);

#endif
