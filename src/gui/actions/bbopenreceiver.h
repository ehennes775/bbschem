#ifndef __BBOPENRECEIVER__
#define __BBOPENRECEIVER__
/*
 * bbschem
 * Copyright (C) 2022 Edward C. Hennessy
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
#include "../bbdocumentwindow.h"

G_BEGIN_DECLS

#define BB_TYPE_OPEN_RECEIVER bb_open_receiver_get_type()
G_DECLARE_INTERFACE(BbOpenReceiver, bb_open_receiver, BB, OPEN_RECEIVER, GObject)

struct _BbOpenReceiverInterface
{
    GTypeInterface g_iface;

    void (*add_window)(BbOpenReceiver *receiver, BbDocumentWindow *window);
};

void
bb_open_receiver_add_window(BbOpenReceiver *receiver, BbDocumentWindow *window);

G_END_DECLS

#endif
