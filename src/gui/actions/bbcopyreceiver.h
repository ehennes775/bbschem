#ifndef __BBCOPYRECEIVER__
#define __BBCOPYRECEIVER__
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

#define BB_TYPE_COPY_RECEIVER bb_copy_receiver_get_type()
G_DECLARE_INTERFACE(BbCopyReceiver, bb_copy_receiver, BB, COPY_RECEIVER, GObject)

struct _BbCopyReceiverInterface
{
    GTypeInterface g_iface;

    gboolean (*can_copy)(BbCopyReceiver *receiver);
    void (*copy)(BbCopyReceiver *receiver);

};

gboolean
bb_copy_receiver_can_copy(BbCopyReceiver *receiver);

void
bb_copy_receiver_copy(BbCopyReceiver *receiver);

#endif
