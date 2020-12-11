#ifndef __BBGRIDCONTROL__
#define __BBGRIDCONTROL__
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

#define BB_TYPE_GRID_CONTROL bb_grid_control_get_type()
G_DECLARE_INTERFACE(BbGridControl, bb_grid_control, BB, GRID_CONTROL, GObject)

struct _BbGridControlInterface
{
    GTypeInterface g_iface;

    gboolean (*get_grid_visible)(BbGridControl *grid_control);
    gboolean (*get_snap_active)(BbGridControl *grid_control);
    void (*set_grid_visible)(BbGridControl *grid_control, gboolean visible);
    void (*set_snap_active)(BbGridControl *grid_control, gboolean visible);
};


gboolean
bb_grid_control_get_grid_visible(BbGridControl *grid_control);


gboolean
bb_grid_control_get_snap_active(BbGridControl *grid_control);


void
bb_grid_control_set_grid_visible(BbGridControl *grid_control, gboolean visible);


void
bb_grid_control_set_snap_active(BbGridControl *grid_control, gboolean active);

#endif
