#ifndef __BBZOOMRECEIVER__
#define __BBZOOMRECEIVER__
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

#include <glib-object.h>
#include "actions/types/bbzoomdirection.h"
#include "actions/types/bbpandirection.h"

G_BEGIN_DECLS

#define BB_TYPE_ZOOM_RECEIVER bb_zoom_receiver_get_type()
G_DECLARE_INTERFACE(BbZoomReceiver, bb_zoom_receiver, BB, ZOOM_RECEIVER, GObject)

struct _BbZoomReceiverInterface
{
    GTypeInterface g_iface;

    void (*pan)(BbZoomReceiver *zoom_receiver, BbPanDirection direction);
    void (*pan_point)(BbZoomReceiver *zoom_receiver);
    void (*zoom_extents)(BbZoomReceiver *zoom_receiver);
    void (*zoom_in)(BbZoomReceiver *zoom_receiver);
    void (*zoom_out)(BbZoomReceiver *zoom_receiver);
    void (*zoom_point)(BbZoomReceiver *zoom_receiver, BbZoomDirection direction);
};


/**
 * Indicates the zoom extents action operation can be performed
 *
 * @param zoom_receiver The object to test
 */
gboolean
bb_zoom_receiver_get_can_zoom_extents(BbZoomReceiver *zoom_receiver);


/**
 * Indicates the zoom in action operation can be performed
 *
 * @param zoom_receiver The object to test
 */
gboolean
bb_zoom_receiver_get_can_zoom_in(BbZoomReceiver *zoom_receiver);


/**
 * Indicates the zoom out action operation can be performed
 *
 * @param zoom_receiver The object to test
 */
gboolean
bb_zoom_receiver_get_can_zoom_out(BbZoomReceiver *zoom_receiver);


/**
 * Pan the window contents in a direction
 *
 * @param zoom_receiver The object to perform the pan operation
 * @param direction The direction to pan the window
 */
void
bb_zoom_receiver_pan(BbZoomReceiver *zoom_receiver, BbPanDirection direction);


/**
 * Center the location of the mouse cursor on the screen
 *
 * @param zoom_receiver The object to perform the pan operation
 */
void
bb_zoom_receiver_pan_point(BbZoomReceiver *zoom_receiver);


/**
 * Zoom to fit the contents into the visible window
 *
 * @param zoom_receiver The object to perform the zoom extents
 */
void
bb_zoom_receiver_zoom_extents(BbZoomReceiver *zoom_receiver);


/**
 * Zoom to in at the center of the window
 *
 * Zoom in and zoom out actions are separate, since they can be enabled and disabled independently.
 *
 * @param zoom_receiver The object to perform the zoom extents
 */
void
bb_zoom_receiver_zoom_in(BbZoomReceiver *zoom_receiver);


/**
 * Zoom to out on the center of the window
 *
 * Zoom in and zoom out actions are separate, since they can be enabled and disabled independently.
 *
 * @param zoom_receiver The object to perform the zoom extents
 */
void
bb_zoom_receiver_zoom_out(BbZoomReceiver *zoom_receiver);


/**
 * Zoom in or out at the location of the mouse cursor
 *
 * Zooming in or out at a point expects to be activated by a hot key. A corresponding can-zoom-point doesn't exist. If
 * the operation cannot be performed, the action is ignored.
 *
 * @param zoom_receiver The object to perform the zoom in or out
 * @param direction Indicates whether to zoom in or out
 */
void
bb_zoom_receiver_zoom_point(BbZoomReceiver *zoom_receiver, BbZoomDirection direction);

G_END_DECLS

#endif
