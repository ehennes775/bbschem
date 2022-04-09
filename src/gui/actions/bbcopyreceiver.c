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
#include "bbcopyreceiver.h"


G_DEFINE_INTERFACE(BbCopyReceiver, bb_copy_receiver, G_TYPE_OBJECT)


gboolean
bb_copy_receiver_can_copy(BbCopyReceiver *receiver)
{
    g_return_val_if_fail(BB_IS_COPY_RECEIVER(receiver), FALSE);

    BbCopyReceiverInterface *iface = BB_COPY_RECEIVER_GET_IFACE(receiver);

    g_return_val_if_fail(iface != NULL, FALSE);

    return iface->can_copy(receiver);
}


static void
bb_copy_receiver_default_init(BbCopyReceiverInterface *class)
{
    g_return_if_fail(class != NULL);
}


void
bb_copy_receiver_copy(BbCopyReceiver *receiver)
{
    g_return_if_fail(BB_IS_COPY_RECEIVER(receiver));

    BbCopyReceiverInterface *iface = BB_COPY_RECEIVER_GET_IFACE(receiver);

    g_return_if_fail(iface != NULL);

    iface->copy(receiver);
}
