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
#include "bbpastereceiver.h"


G_DEFINE_INTERFACE(BbPasteReceiver, bb_paste_receiver, G_TYPE_OBJECT)


gboolean
bb_paste_receiver_can_paste(BbPasteReceiver *receiver, GtkSelectionData *selection_data)
{
    g_return_val_if_fail(BB_IS_PASTE_RECEIVER(receiver), FALSE);

    BbPasteReceiverInterface *iface = BB_PASTE_RECEIVER_GET_IFACE(receiver);

    g_return_val_if_fail(iface != NULL, FALSE);

    return iface->can_paste(receiver, selection_data);
}


static void
bb_paste_receiver_default_init(BbPasteReceiverInterface *class)
{
    g_return_if_fail(class != NULL);
}


void
bb_paste_receiver_paste(BbPasteReceiver *receiver, GtkClipboard *clipboard)
{
    g_return_if_fail(BB_IS_PASTE_RECEIVER(receiver));

    BbPasteReceiverInterface *iface = BB_PASTE_RECEIVER_GET_IFACE(receiver);

    g_return_if_fail(iface != NULL);

    iface->paste(receiver, clipboard);
}
