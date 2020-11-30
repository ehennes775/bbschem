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
#include "bbgridsubject.h"


static gboolean
bb_grid_subject_get_can_scale_down_missing(BbGridSubject *grid_subject);

static gboolean
bb_grid_subject_get_can_scale_up_missing(BbGridSubject *grid_subject);

static void
bb_grid_subject_scale_down_missing(BbGridSubject *grid_subject);

static void
bb_grid_subject_scale_up_missing(BbGridSubject *grid_subject);


G_DEFINE_INTERFACE(
    BbGridSubject,
    bb_grid_subject,
    G_TYPE_OBJECT
    )


static void
bb_grid_subject_default_init(BbGridSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->can_scale_down = bb_grid_subject_get_can_scale_down_missing;
    iface->can_scale_up = bb_grid_subject_get_can_scale_up_missing;
    iface->scale_down = bb_grid_subject_scale_down_missing;
    iface->scale_up = bb_grid_subject_scale_up_missing;

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-scale-down",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-scale-up",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );
}


gboolean
bb_grid_subject_get_can_scale_down(BbGridSubject *grid_subject)
{
    BbGridSubjectInterface *iface = BB_GRID_SUBJECT_GET_IFACE(grid_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->can_scale_down != NULL, FALSE);

    return iface->can_scale_down(grid_subject);
}


static gboolean
bb_grid_subject_get_can_scale_down_missing(BbGridSubject *grid_subject)
{
    g_error("bb_grid_subject_get_can_scale_down() not overridden");
}

gboolean
bb_grid_subject_get_can_scale_up(BbGridSubject *grid_subject)
{
    BbGridSubjectInterface *iface = BB_GRID_SUBJECT_GET_IFACE(grid_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->can_scale_up != NULL, FALSE);

    return iface->can_scale_up(grid_subject);
}


static gboolean
bb_grid_subject_get_can_scale_up_missing(BbGridSubject *grid_subject)
{
    g_error("bb_grid_subject_get_can_scale_up() not overridden");
}

void
bb_grid_subject_scale_down(BbGridSubject *grid_subject)
{
    BbGridSubjectInterface *iface = BB_GRID_SUBJECT_GET_IFACE(grid_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->scale_down != NULL);

    iface->scale_down(grid_subject);
}


static void
bb_grid_subject_scale_down_missing(BbGridSubject *grid_subject)
{
    g_error("bb_grid_subject_scale_down() not overridden");
}


void
bb_grid_subject_scale_up(BbGridSubject *grid_subject)
{
    BbGridSubjectInterface *iface = BB_GRID_SUBJECT_GET_IFACE(grid_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->scale_up != NULL);

    iface->scale_up(grid_subject);
}


static void
bb_grid_subject_scale_up_missing(BbGridSubject *grid_subject)
{
    g_error("bb_grid_subject_scale_up() not overridden");
}
