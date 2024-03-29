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

#include <gtk/gtk.h>
#include "bbopenreceiver.h"


G_DEFINE_INTERFACE(BbOpenReceiver, bb_open_receiver, G_TYPE_OBJECT)


static void
bb_open_receiver_default_init(BbOpenReceiverInterface *class)
{
    g_return_if_fail(class != NULL);
}


void
bb_open_receiver_open(BbOpenReceiver *receiver, BbDocumentWindow *window)
{
    g_return_if_fail(BB_IS_OPEN_RECEIVER(receiver));
    g_return_if_fail(BB_IS_DOCUMENT_WINDOW(window));

    BbOpenReceiverInterface *iface = BB_OPEN_RECEIVER_GET_IFACE(receiver);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->add_window != NULL);

    iface->add_window(receiver, window);
}
