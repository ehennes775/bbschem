#ifndef __BBDELETERECEIVER__
#define __BBDELETERECEIVER__
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

G_BEGIN_DECLS

#define BB_TYPE_DELETE_RECEIVER bb_delete_receiver_get_type()
G_DECLARE_INTERFACE(BbDeleteReceiver, bb_delete_receiver, BB, DELETE_RECEIVER, GObject)

struct _BbDeleteReceiverInterface
{
    GTypeInterface g_iface;

    gboolean (*can_delete)(BbDeleteReceiver *receiver);
    void (*delete)(BbDeleteReceiver *receiver);

};

gboolean
bb_delete_receiver_can_delete(BbDeleteReceiver *receiver);

void
bb_delete_receiver_delete(BbDeleteReceiver *receiver);

G_END_DECLS

#endif
