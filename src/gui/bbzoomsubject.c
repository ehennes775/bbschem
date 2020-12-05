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
bb_zoom_subject_pan_missing(BbZoomSubject *zoom_subject, BbPanDirection direction);

static void
bb_zoom_subject_pan_point_missing(BbZoomSubject *zoom_subject);

static void
bb_zoom_subject_zoom_extents_missing(BbZoomSubject *zoom_subject);

static void
bb_zoom_subject_zoom_in_missing(BbZoomSubject *zoom_subject);

static void
bb_zoom_subject_zoom_out_missing(BbZoomSubject *zoom_subject);

static void
bb_zoom_subject_zoom_point_missing(BbZoomSubject *zoom_subject, BbZoomDirection direction);


G_DEFINE_INTERFACE(
    BbZoomSubject,
    bb_zoom_subject,
    G_TYPE_OBJECT
    )


static void
bb_zoom_subject_default_init(BbZoomSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->pan = bb_zoom_subject_pan_missing;
    iface->pan_point = bb_zoom_subject_pan_point_missing;
    iface->zoom_extents = bb_zoom_subject_zoom_extents_missing;
    iface->zoom_in = bb_zoom_subject_zoom_in_missing;
    iface->zoom_out = bb_zoom_subject_zoom_out_missing;
    iface->zoom_point = bb_zoom_subject_zoom_point_missing;

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


gboolean
bb_zoom_subject_get_can_zoom_in(BbZoomSubject *zoom_subject)
{
    g_return_val_if_fail(BB_IS_ZOOM_SUBJECT(zoom_subject), FALSE);

    gboolean can_zoom_in;

    g_object_get(
        zoom_subject,
        "can-zoom-in", &can_zoom_in,
        NULL
    );

    return can_zoom_in;
}


gboolean
bb_zoom_subject_get_can_zoom_out(BbZoomSubject *zoom_subject)
{
    g_return_val_if_fail(BB_IS_ZOOM_SUBJECT(zoom_subject), FALSE);

    gboolean can_zoom_out;

    g_object_get(
        zoom_subject,
        "can-zoom-out", &can_zoom_out,
        NULL
    );

    return can_zoom_out;
}

void
bb_zoom_subject_pan(BbZoomSubject *zoom_subject, BbPanDirection direction)
{
    BbZoomSubjectInterface *iface = BB_ZOOM_SUBJECT_GET_IFACE(zoom_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->pan != NULL);

    return iface->pan(zoom_subject, direction);
}


static void
bb_zoom_subject_pan_missing(BbZoomSubject *zoom_subject, BbPanDirection direction)
{
    g_error("bb_zoom_subject_pan() not overridden");
}


void
bb_zoom_subject_pan_point(BbZoomSubject *zoom_subject)
{
    BbZoomSubjectInterface *iface = BB_ZOOM_SUBJECT_GET_IFACE(zoom_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->pan_point != NULL);

    return iface->pan_point(zoom_subject);
}


static void
bb_zoom_subject_pan_point_missing(BbZoomSubject *zoom_subject)
{
    g_error("bb_zoom_subject_pan_point() not overridden");
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


void
bb_zoom_subject_zoom_point(BbZoomSubject *zoom_subject, BbZoomDirection direction)
{
    BbZoomSubjectInterface *iface = BB_ZOOM_SUBJECT_GET_IFACE(zoom_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->zoom_point != NULL);

    return iface->zoom_point(zoom_subject, direction);
}


static void
bb_zoom_subject_zoom_point_missing(BbZoomSubject *zoom_subject, BbZoomDirection direction)
{
    g_error("bb_zoom_subject_zoom_point() not overridden");
}
