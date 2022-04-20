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
#include "bbdeletereceiver.h"


G_DEFINE_INTERFACE(BbDeleteReceiver, bb_delete_receiver, G_TYPE_OBJECT)


gboolean
bb_delete_receiver_can_delete(BbDeleteReceiver *receiver)
{
    g_return_val_if_fail(BB_IS_DELETE_RECEIVER(receiver), FALSE);

    BbDeleteReceiverInterface *iface = BB_DELETE_RECEIVER_GET_IFACE(receiver);

    g_return_val_if_fail(iface != NULL, FALSE);

    return iface->can_delete(receiver);
}


static void
bb_delete_receiver_default_init(BbDeleteReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-delete",
            "",
            "",
            FALSE,
            G_PARAM_READABLE
            )
        );
}


void
bb_delete_receiver_delete(BbDeleteReceiver *receiver)
{
    g_return_if_fail(BB_IS_DELETE_RECEIVER(receiver));

    BbDeleteReceiverInterface *iface = BB_DELETE_RECEIVER_GET_IFACE(receiver);

    g_return_if_fail(iface != NULL);

    iface->delete(receiver);
}
