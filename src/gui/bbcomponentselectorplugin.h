#ifndef __BBCOMPONENTSELECTORPLUGIN__
#define __BBCOMPONENTSELECTORPLUGIN__
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
#include <libpeas/peas.h>


#define BB_TYPE_COMPONENT_SELECTOR_PLUGIN bb_component_selector_plugin_get_type()
G_DECLARE_FINAL_TYPE(BbComponentSelectorPlugin, bb_component_selector_plugin, BB, COMPONENT_SELECTOR_PLUGIN, PeasExtensionBase)


GObject*
bb_component_selector_plugin_get_object(BbComponentSelectorPlugin *plugin);

void
bb_component_selector_plugin_set_object(BbComponentSelectorPlugin *plugin, GObject *object);


#endif
