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
#include "bbcutreceiver.h"


G_DEFINE_INTERFACE(BbCutReceiver, bb_cut_receiver, G_TYPE_OBJECT)


gboolean
bb_cut_receiver_can_cut(BbCutReceiver *receiver)
{
    g_return_val_if_fail(BB_IS_CUT_RECEIVER(receiver), FALSE);

    BbCutReceiverInterface *iface = BB_CUT_RECEIVER_GET_IFACE(receiver);

    g_return_val_if_fail(iface != NULL, FALSE);

    return iface->can_cut(receiver);
}


static void
bb_cut_receiver_default_init(BbCutReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-cut",
            "",
            "",
            FALSE,
            G_PARAM_READABLE
            )
        );
}


void
bb_cut_receiver_cut(BbCutReceiver *receiver, GtkClipboard *clipboard)
{
    g_return_if_fail(BB_IS_CUT_RECEIVER(receiver));

    BbCutReceiverInterface *iface = BB_CUT_RECEIVER_GET_IFACE(receiver);

    g_return_if_fail(iface != NULL);

    iface->cut(receiver, clipboard);
}
