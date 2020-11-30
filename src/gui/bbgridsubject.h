#ifndef __BBGRIDSUBJECT__
#define __BBGRIDSUBJECT__
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

#define BB_TYPE_GRID_SUBJECT bb_grid_subject_get_type()
G_DECLARE_INTERFACE(BbGridSubject, bb_grid_subject, BB, GRID_SUBJECT, GObject)

struct _BbGridSubjectInterface
{
    GTypeInterface g_iface;

    gboolean (*can_scale_down)(BbGridSubject *grid_subject);
    gboolean (*can_scale_up)(BbGridSubject *grid_subject);
    void (*scale_down)(BbGridSubject *grid_subject);
    void (*scale_up)(BbGridSubject *grid_subject);
};


gboolean
bb_grid_subject_get_can_scale_down(BbGridSubject *grid_subject);

gboolean
bb_grid_subject_get_can_scale_up(BbGridSubject *grid_subject);

void
bb_grid_subject_scale_down(BbGridSubject *grid_subject);

void
bb_grid_subject_scale_up(BbGridSubject *grid_subject);

#endif
