#ifndef __BBSELECTRECEIVER__
#define __BBSELECTRECEIVER__
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

G_BEGIN_DECLS

#define BB_TYPE_SELECT_RECEIVER bb_select_receiver_get_type()
G_DECLARE_INTERFACE(BbSelectReceiver, bb_select_receiver, BB, SELECT_RECEIVER, GObject)

struct _BbSelectReceiverInterface
{
    GTypeInterface g_iface;

    gboolean (*can_select_all)(BbSelectReceiver *receiver);
    gboolean (*can_select_none)(BbSelectReceiver *receiver);
    void (*select_all)(BbSelectReceiver *receiver);
    void (*select_none)(BbSelectReceiver *receiver);

};

gboolean
bb_select_receiver_can_select_all(BbSelectReceiver *receiver);

gboolean
bb_select_receiver_can_select_none(BbSelectReceiver *receiver);

void
bb_select_receiver_select_all(BbSelectReceiver *receiver);

void
bb_select_receiver_select_none(BbSelectReceiver *receiver);

G_END_DECLS

#endif
