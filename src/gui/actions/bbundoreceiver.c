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
#include "bbundoreceiver.h"


G_DEFINE_INTERFACE(BbUndoReceiver, bb_undo_receiver, G_TYPE_OBJECT)


gboolean
bb_undo_receiver_can_undo(BbUndoReceiver *receiver)
{
    g_return_val_if_fail(BB_IS_UNDO_RECEIVER(receiver), FALSE);

    BbUndoReceiverInterface *iface = BB_UNDO_RECEIVER_GET_IFACE(receiver);

    g_return_val_if_fail(iface != NULL, FALSE);

    return iface->can_undo(receiver);
}


static void
bb_undo_receiver_default_init(BbUndoReceiverInterface *class)
{
    g_return_if_fail(class != NULL);
}


void
bb_undo_receiver_undo(BbUndoReceiver *receiver)
{
    g_return_if_fail(BB_IS_UNDO_RECEIVER(receiver));

    BbUndoReceiverInterface *iface = BB_UNDO_RECEIVER_GET_IFACE(receiver);

    g_return_if_fail(iface != NULL);

    iface->undo(receiver);
}
