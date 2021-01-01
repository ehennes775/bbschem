#ifndef __BBGEDAOPENER__
#define __BBGEDAOPENER__
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
#include "bbgedaschematicreader.h"

#define BB_TYPE_GEDA_OPENER bb_geda_opener_get_type()
G_DECLARE_FINAL_TYPE(BbGedaOpener, bb_geda_opener, BB, GEDA_OPENER, GObject)

BbGedaSchematicReader*
bb_geda_opener_get_reader(BbGedaOpener *opener);

BbGedaOpener*
bb_geda_opener_new(BbMainWindow *main_window);

#endif
