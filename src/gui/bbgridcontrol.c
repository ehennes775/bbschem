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
#include "bbgridcontrol.h"


G_DEFINE_INTERFACE(BbGridControl, bb_grid_control, G_TYPE_OBJECT)


static gboolean
bb_grid_control_get_grid_visible_missing(BbGridControl *grid_control);

static gboolean
bb_grid_control_get_snap_active_missing(BbGridControl *grid_control);

static void
bb_grid_control_set_grid_visible_missing(BbGridControl *grid_control, gboolean active);

static void
bb_grid_control_set_snap_active_missing(BbGridControl *grid_control, gboolean active);


static void
bb_grid_control_default_init(BbGridControlInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_grid_visible = bb_grid_control_get_grid_visible_missing;
    iface->get_snap_active = bb_grid_control_get_snap_active_missing;
    iface->set_grid_visible = bb_grid_control_set_grid_visible_missing;
    iface->set_snap_active = bb_grid_control_set_snap_active_missing;

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "grid-visible",
            "",
            "",
            TRUE,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "snap-active",
            "",
            "",
            TRUE,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );
}


gboolean
bb_grid_control_get_grid_visible(BbGridControl *grid_control)
{
    BbGridControlInterface *iface = BB_GRID_CONTROL_GET_IFACE(grid_control);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_grid_visible != NULL, FALSE);

    return iface->get_grid_visible(grid_control);
}


static gboolean
bb_grid_control_get_grid_visible_missing(BbGridControl *grid_control)
{
    g_error("bb_grid_control_get_grid_visible() not overridden");
}


gboolean
bb_grid_control_get_snap_active(BbGridControl *grid_control)
{
    BbGridControlInterface *iface = BB_GRID_CONTROL_GET_IFACE(grid_control);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_snap_active != NULL, FALSE);

    return iface->get_snap_active(grid_control);
}


static gboolean
bb_grid_control_get_snap_active_missing(BbGridControl *grid_control)
{
    g_error("bb_grid_control_get_snap_active() not overridden");
}


void
bb_grid_control_set_grid_visible(BbGridControl *grid_control, gboolean visible)
{
    BbGridControlInterface *iface = BB_GRID_CONTROL_GET_IFACE(grid_control);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->set_grid_visible != NULL);

    return iface->set_grid_visible(grid_control, visible);
}


static void
bb_grid_control_set_grid_visible_missing(BbGridControl *grid_control, gboolean visible)
{
    g_error("bb_grid_control_set_grid_visible() not overridden");
}


void
bb_grid_control_set_snap_active(BbGridControl *grid_control, gboolean active)
{
    BbGridControlInterface *iface = BB_GRID_CONTROL_GET_IFACE(grid_control);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->set_snap_active != NULL);

    return iface->set_snap_active(grid_control, active);
}


static void
bb_grid_control_set_snap_active_missing(BbGridControl *grid_control, gboolean active)
{
    g_error("bb_grid_control_set_snap_active() not overridden");
}
