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
#include "bbgenericreceiver.h"


G_DEFINE_INTERFACE(BbGenericReceiver, bb_generic_receiver, G_TYPE_OBJECT)


GObject *
bb_delete_receiver_get_receiver(BbGenericReceiver *object)
{
    g_return_val_if_fail(BB_IS_GENERIC_RECEIVER(object), NULL);

    BbGenericReceiverInterface *iface = BB_GENERIC_RECEIVER_GET_IFACE(object);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->get_receiver != NULL, NULL);

    return iface->get_receiver(object);
}


static void
bb_generic_receiver_default_init(BbGenericReceiverInterface *klasse)
{
    g_return_if_fail(klasse != NULL);
}


void
bb_generic_receiver_set_receiver(BbGenericReceiver *object, GObject *receiver)
{
    g_return_if_fail(BB_IS_GENERIC_RECEIVER(object));

    BbGenericReceiverInterface *iface = BB_GENERIC_RECEIVER_GET_IFACE(object);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->set_receiver != NULL);

    iface->set_receiver(object, receiver);
}
