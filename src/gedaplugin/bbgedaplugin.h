#ifndef __BBGEDAPLUGIN__
#define __BBGEDAPLUGIN__
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
#include <bbmainwindow.h>

#define BB_TYPE_GEDA_PLUGIN bb_geda_plugin_get_type()
G_DECLARE_FINAL_TYPE(BbGedaPlugin, bb_geda_plugin, BB, GEDA_PLUGIN, PeasExtensionBase)

void
bb_geda_plugin_register(GTypeModule *type_module);

#endif
