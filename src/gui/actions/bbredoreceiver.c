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
#include "bbredoreceiver.h"


G_DEFINE_INTERFACE(BbRedoReceiver, bb_redo_receiver, G_TYPE_OBJECT)


gboolean
bb_redo_receiver_can_redo(BbRedoReceiver *receiver)
{
    g_return_val_if_fail(BB_IS_REDO_RECEIVER(receiver), FALSE);

    BbRedoReceiverInterface *iface = BB_REDO_RECEIVER_GET_IFACE(receiver);

    g_return_val_if_fail(iface != NULL, FALSE);

    return iface->can_redo(receiver);
}


static void
bb_redo_receiver_default_init(BbRedoReceiverInterface *class)
{
    g_return_if_fail(class != NULL);
}


void
bb_redo_receiver_redo(BbRedoReceiver *receiver)
{
    g_return_if_fail(BB_IS_REDO_RECEIVER(receiver));

    BbRedoReceiverInterface *iface = BB_REDO_RECEIVER_GET_IFACE(receiver);

    g_return_if_fail(iface != NULL);

    iface->redo(receiver);
}
