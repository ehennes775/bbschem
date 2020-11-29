#ifndef __BBZOOMSUBJECT__
#define __BBZOOMSUBJECT__
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

#define BB_TYPE_ZOOM_SUBJECT bb_zoom_subject_get_type()
G_DECLARE_INTERFACE(BbZoomSubject, bb_zoom_subject, BB, ZOOM_SUBJECT, GObject)

struct _BbZoomSubjectInterface
{
    GTypeInterface g_iface;

    void (*zoom_extents)(BbZoomSubject *zoom_subject);
    void (*zoom_in)(BbZoomSubject *zoom_subject);
    void (*zoom_out)(BbZoomSubject *zoom_subject);
};

gboolean
bb_zoom_subject_get_can_zoom_extents(BbZoomSubject *zoom_subject);

gboolean
bb_zoom_subject_get_can_zoom_in(BbZoomSubject *zoom_subject);

gboolean
bb_zoom_subject_get_can_zoom_out(BbZoomSubject *zoom_subject);

void
bb_zoom_subject_zoom_extents(BbZoomSubject *zoom_subject);

void
bb_zoom_subject_zoom_in(BbZoomSubject *zoom_subject);

void
bb_zoom_subject_zoom_out(BbZoomSubject *zoom_subject);

#endif
