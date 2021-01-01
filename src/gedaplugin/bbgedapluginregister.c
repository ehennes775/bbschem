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
bb_geda_editor_register(GTypeModule *module);

void
bb_geda_opener_register(GTypeModule *module);

void
bb_geda_plugin_register(GTypeModule *module);

void
bb_geda_reader_register(GTypeModule *module);

void
bb_geda_view_register(GTypeModule *module);


G_MODULE_EXPORT void
peas_register_types(PeasObjectModule *module)
{
    bb_geda_editor_register(G_TYPE_MODULE(module));
    bb_geda_opener_register(G_TYPE_MODULE(module));
    bb_geda_plugin_register(G_TYPE_MODULE(module));
    bb_geda_reader_register(G_TYPE_MODULE(module));
    bb_geda_view_register(G_TYPE_MODULE(module));

    peas_object_module_register_extension_type(
        module,
        PEAS_TYPE_ACTIVATABLE,
        BB_TYPE_GEDA_PLUGIN
        );
}
