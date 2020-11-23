#ifndef __BBSAVEACTION__
#define __BBSAVEACTION__
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
#include "bbmainwindow.h"

#define BB_TYPE_SAVE_ACTION bb_save_action_get_type()
G_DECLARE_FINAL_TYPE(BbSaveAction, bb_save_action, BB, SAVE_ACTION, GObject)


BbMainWindow*
bb_save_action_get_window(BbSaveAction *action);


BbSaveAction*
bb_save_action_new(BbMainWindow *window);


void
bb_save_action_set_window(BbSaveAction *action, BbMainWindow* window);


#endif