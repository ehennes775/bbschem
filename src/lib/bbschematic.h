#ifndef __BBSCHEMATIC__
#define __BBSCHEMATIC__
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

#define BB_TYPE_SCHEMATIC bb_schematic_get_type()
G_DECLARE_FINAL_TYPE(BbSchematic, bb_schematic, BB, SCHEMATIC, GObject)


void
bb_schematic_add_items(BbSchematic *schematic, GSList *items);

void
bb_schematic_apply_item_property(BbSchematic *schematic, const char *name, const GValue *value);

void
bb_schematic_foreach(BbSchematic *schematic, GFunc func, gpointer user_data);

BbSchematic*
bb_schematic_new();

#endif
