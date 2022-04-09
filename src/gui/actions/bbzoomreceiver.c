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
#include "bbzoomreceiver.h"


static void
bb_zoom_receiver_pan_missing(BbZoomReceiver *zoom_receiver, BbPanDirection direction);

static void
bb_zoom_receiver_pan_point_missing(BbZoomReceiver *zoom_receiver);

static void
bb_zoom_receiver_zoom_extents_missing(BbZoomReceiver *zoom_receiver);

static void
bb_zoom_receiver_zoom_in_missing(BbZoomReceiver *zoom_receiver);

static void
bb_zoom_receiver_zoom_out_missing(BbZoomReceiver *zoom_receiver);

static void
bb_zoom_receiver_zoom_point_missing(BbZoomReceiver *zoom_receiver, BbZoomDirection direction);


G_DEFINE_INTERFACE(
    BbZoomReceiver,
    bb_zoom_receiver,
    G_TYPE_OBJECT
    )


static void
bb_zoom_receiver_default_init(BbZoomReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->pan = bb_zoom_receiver_pan_missing;
    iface->pan_point = bb_zoom_receiver_pan_point_missing;
    iface->zoom_extents = bb_zoom_receiver_zoom_extents_missing;
    iface->zoom_in = bb_zoom_receiver_zoom_in_missing;
    iface->zoom_out = bb_zoom_receiver_zoom_out_missing;
    iface->zoom_point = bb_zoom_receiver_zoom_point_missing;

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
bb_zoom_receiver_get_can_zoom_extents(BbZoomReceiver *zoom_receiver)
{
    g_return_val_if_fail(BB_IS_ZOOM_RECEIVER(zoom_receiver), FALSE);

    gboolean can_zoom_extents;

    g_object_get(
        zoom_receiver,
        "can-zoom-extents", &can_zoom_extents,
        NULL
        );

    return can_zoom_extents;
}


gboolean
bb_zoom_receiver_get_can_zoom_in(BbZoomReceiver *zoom_receiver)
{
    g_return_val_if_fail(BB_IS_ZOOM_RECEIVER(zoom_receiver), FALSE);

    gboolean can_zoom_in;

    g_object_get(
        zoom_receiver,
        "can-zoom-in", &can_zoom_in,
        NULL
    );

    return can_zoom_in;
}


gboolean
bb_zoom_receiver_get_can_zoom_out(BbZoomReceiver *zoom_receiver)
{
    g_return_val_if_fail(BB_IS_ZOOM_RECEIVER(zoom_receiver), FALSE);

    gboolean can_zoom_out;

    g_object_get(
        zoom_receiver,
        "can-zoom-out", &can_zoom_out,
        NULL
    );

    return can_zoom_out;
}

void
bb_zoom_receiver_pan(BbZoomReceiver *zoom_receiver, BbPanDirection direction)
{
    BbZoomReceiverInterface *iface = BB_ZOOM_RECEIVER_GET_IFACE(zoom_receiver);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->pan != NULL);

    return iface->pan(zoom_receiver, direction);
}


static void
bb_zoom_receiver_pan_missing(BbZoomReceiver *zoom_receiver, BbPanDirection direction)
{
    g_error("bb_zoom_receiver_pan() not overridden");
}


void
bb_zoom_receiver_pan_point(BbZoomReceiver *zoom_receiver)
{
    BbZoomReceiverInterface *iface = BB_ZOOM_RECEIVER_GET_IFACE(zoom_receiver);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->pan_point != NULL);

    return iface->pan_point(zoom_receiver);
}


static void
bb_zoom_receiver_pan_point_missing(BbZoomReceiver *zoom_receiver)
{
    g_error("bb_zoom_receiver_pan_point() not overridden");
}


void
bb_zoom_receiver_zoom_extents(BbZoomReceiver *zoom_receiver)
{
    BbZoomReceiverInterface *iface = BB_ZOOM_RECEIVER_GET_IFACE(zoom_receiver);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->zoom_extents != NULL);

    return iface->zoom_extents(zoom_receiver);
}


static void
bb_zoom_receiver_zoom_extents_missing(BbZoomReceiver *zoom_receiver)
{
    g_error("bb_zoom_receiver_zoom_extents() not overridden");
}


void
bb_zoom_receiver_zoom_in(BbZoomReceiver *zoom_receiver)
{
    BbZoomReceiverInterface *iface = BB_ZOOM_RECEIVER_GET_IFACE(zoom_receiver);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->zoom_in != NULL);

    return iface->zoom_in(zoom_receiver);
}


static void
bb_zoom_receiver_zoom_in_missing(BbZoomReceiver *zoom_receiver)
{
    g_error("bb_zoom_receiver_zoom_in() not overridden");
}


void
bb_zoom_receiver_zoom_out(BbZoomReceiver *zoom_receiver)
{
    BbZoomReceiverInterface *iface = BB_ZOOM_RECEIVER_GET_IFACE(zoom_receiver);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->zoom_out != NULL);

    return iface->zoom_out(zoom_receiver);
}


static void
bb_zoom_receiver_zoom_out_missing(BbZoomReceiver *zoom_receiver)
{
    g_error("bb_zoom_receiver_zoom_out() not overridden");
}


void
bb_zoom_receiver_zoom_point(BbZoomReceiver *zoom_receiver, BbZoomDirection direction)
{
    BbZoomReceiverInterface *iface = BB_ZOOM_RECEIVER_GET_IFACE(zoom_receiver);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->zoom_point != NULL);

    return iface->zoom_point(zoom_receiver, direction);
}


static void
bb_zoom_receiver_zoom_point_missing(BbZoomReceiver *zoom_receiver, BbZoomDirection direction)
{
    g_error("bb_zoom_receiver_zoom_point() not overridden");
}
