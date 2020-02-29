#ifndef __BBSCHEMATICWRAPPER__
#define __BBSCHEMATICWRAPPER__
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

#define BB_TYPE_SCHEMATIC_WRAPPER bb_schematic_wrapper_get_type()
G_DECLARE_FINAL_TYPE(BbSchematicWrapper, bb_schematic_wrapper, BB, SCHEMATIC_WRAPPER, GObject)

void
bb_schematic_wrapper_attach_actions(BbSchematicWrapper *wrapper, GActionMap *map);

void
bb_schematic_wrapper_detach_actions(BbSchematicWrapper *wrapper, GActionMap *map);

#endif
