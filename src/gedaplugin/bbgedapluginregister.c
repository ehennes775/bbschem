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
#include <libpeas/peas.h>
#include "bbgedaplugin.h"


void
bb_arc_tool_register(GTypeModule *module);

void
bb_attribute_tool_register(GTypeModule *module);

void
bb_block_tool_register(GTypeModule *module);

void
bb_box_tool_register(GTypeModule *module);

void
bb_bus_tool_register(GTypeModule *module);

void
bb_circle_tool_register(GTypeModule *module);

void
bb_pin_tool_register(GTypeModule *module);

void
bb_select_tool_register(GTypeModule *module);

void
bb_text_tool_register(GTypeModule *module);

void
bb_zoom_tool_register(GTypeModule *module);


void
bb_geda_arc_factory_register(GTypeModule *module);

void
bb_geda_block_factory_register(GTypeModule *module);

void
bb_geda_box_factory_register(GTypeModule *module);

void
bb_geda_bus_factory_register(GTypeModule *module);

void
bb_geda_circle_factory_register(GTypeModule *module);

void
bb_geda_editor_register(GTypeModule *module);

void
bb_geda_factory_register(GTypeModule *module);

void
bb_geda_item_factory_register(GTypeModule *module);

void
bb_geda_line_factory_register(GTypeModule *module);

void
bb_geda_net_factory_register(GTypeModule *module);

void
bb_geda_opener_register(GTypeModule *module);

void
bb_geda_path_factory_register(GTypeModule *module);

void
bb_geda_pin_factory_register(GTypeModule *module);

void
bb_geda_plugin_register(GTypeModule *module);

void
bb_geda_reader_register(GTypeModule *module);

void
bb_geda_text_factory_register(GTypeModule *module);

void
bb_geda_view_register(GTypeModule *module);


G_MODULE_EXPORT void
peas_register_types(PeasObjectModule *module)
{
    /* Must be called before classes that implement this interface */
    bb_geda_item_factory_register(G_TYPE_MODULE(module));

    /* These will be renamed to include geda */
    bb_arc_tool_register(G_TYPE_MODULE(module));
    bb_attribute_tool_register(G_TYPE_MODULE(module));
    bb_block_tool_register(G_TYPE_MODULE(module));
    bb_box_tool_register(G_TYPE_MODULE(module));
    bb_bus_tool_register(G_TYPE_MODULE(module));
    bb_circle_tool_register(G_TYPE_MODULE(module));
    bb_pin_tool_register(G_TYPE_MODULE(module));
    bb_select_tool_register(G_TYPE_MODULE(module));
    bb_text_tool_register(G_TYPE_MODULE(module));
    bb_zoom_tool_register(G_TYPE_MODULE(module));

    bb_geda_arc_factory_register(G_TYPE_MODULE(module));
    bb_geda_block_factory_register(G_TYPE_MODULE(module));
    bb_geda_box_factory_register(G_TYPE_MODULE(module));
    bb_geda_bus_factory_register(G_TYPE_MODULE(module));
    bb_geda_circle_factory_register(G_TYPE_MODULE(module));
    bb_geda_editor_register(G_TYPE_MODULE(module));
    bb_geda_factory_register(G_TYPE_MODULE(module));
    bb_geda_line_factory_register(G_TYPE_MODULE(module));
    bb_geda_net_factory_register(G_TYPE_MODULE(module));
    bb_geda_opener_register(G_TYPE_MODULE(module));
    bb_geda_path_factory_register(G_TYPE_MODULE(module));
    bb_geda_pin_factory_register(G_TYPE_MODULE(module));
    bb_geda_plugin_register(G_TYPE_MODULE(module));
    bb_geda_reader_register(G_TYPE_MODULE(module));
    bb_geda_text_factory_register(G_TYPE_MODULE(module));
    bb_geda_view_register(G_TYPE_MODULE(module));

    peas_object_module_register_extension_type(
        module,
        PEAS_TYPE_ACTIVATABLE,
        BB_TYPE_GEDA_PLUGIN
        );
}
