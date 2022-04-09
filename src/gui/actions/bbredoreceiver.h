#ifndef __BBREDORECEIVER__
#define __BBREDORECEIVER__
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

#define BB_TYPE_REDO_RECEIVER bb_redo_receiver_get_type()
G_DECLARE_INTERFACE(BbRedoReceiver, bb_redo_receiver, BB, REDO_RECEIVER, GObject)

struct _BbRedoReceiverInterface
{
    GTypeInterface g_iface;

    gboolean (*can_redo)(BbRedoReceiver *receiver);
    void (*redo)(BbRedoReceiver *receiver);

};

gboolean
bb_redo_receiver_can_redo(BbRedoReceiver *receiver);

void
bb_redo_receiver_redo(BbRedoReceiver *receiver);

#endif
