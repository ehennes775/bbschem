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
#include "bbzoomsubject.h"


static void
bb_zoom_subject_zoom_extents_missing(BbZoomSubject *zoom_subject);

static void
bb_zoom_subject_zoom_in_missing(BbZoomSubject *zoom_subject);

static void
bb_zoom_subject_zoom_out_missing(BbZoomSubject *zoom_subject);


G_DEFINE_INTERFACE(
    BbZoomSubject,
    bb_zoom_subject,
    G_TYPE_OBJECT
    )


static void
bb_zoom_subject_default_init(BbZoomSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->zoom_extents = bb_zoom_subject_zoom_extents_missing;
    iface->zoom_in = bb_zoom_subject_zoom_in_missing;
    iface->zoom_out = bb_zoom_subject_zoom_out_missing;

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-zoom-extents",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-zoom-in",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-zoom-out",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );
}


gboolean
bb_zoom_subject_get_can_zoom_extents(BbZoomSubject *zoom_subject)
{
    g_return_val_if_fail(BB_IS_ZOOM_SUBJECT(zoom_subject), FALSE);

    gboolean can_zoom_extents;

    g_object_get(
        zoom_subject,
        "can-zoom-extents", &can_zoom_extents,
        NULL
        );

    return can_zoom_extents;
}


void
bb_zoom_subject_zoom_extents(BbZoomSubject *zoom_subject)
{
    BbZoomSubjectInterface *iface = BB_ZOOM_SUBJECT_GET_IFACE(zoom_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->zoom_extents != NULL);

    return iface->zoom_extents(zoom_subject);
}


static void
bb_zoom_subject_zoom_extents_missing(BbZoomSubject *zoom_subject)
{
    g_error("bb_zoom_subject_zoom_extents() not overridden");
}


void
bb_zoom_subject_zoom_in(BbZoomSubject *zoom_subject)
{
    BbZoomSubjectInterface *iface = BB_ZOOM_SUBJECT_GET_IFACE(zoom_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->zoom_in != NULL);

    return iface->zoom_in(zoom_subject);
}


static void
bb_zoom_subject_zoom_in_missing(BbZoomSubject *zoom_subject)
{
    g_error("bb_zoom_subject_zoom_in() not overridden");
}


void
bb_zoom_subject_zoom_out(BbZoomSubject *zoom_subject)
{
    BbZoomSubjectInterface *iface = BB_ZOOM_SUBJECT_GET_IFACE(zoom_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->zoom_out != NULL);

    return iface->zoom_out(zoom_subject);
}


static void
bb_zoom_subject_zoom_out_missing(BbZoomSubject *zoom_subject)
{
    g_error("bb_zoom_subject_zoom_out() not overridden");
}
