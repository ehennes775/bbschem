#ifndef __BBCUTRECEIVER__
#define __BBCUTRECEIVER__
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

#define BB_TYPE_CUT_RECEIVER bb_cut_receiver_get_type()
G_DECLARE_INTERFACE(BbCutReceiver, bb_cut_receiver, BB, CUT_RECEIVER, GObject)

struct _BbCutReceiverInterface
{
    GTypeInterface g_iface;

    gboolean (*can_cut)(BbCutReceiver *receiver);
    void (*cut)(BbCutReceiver *receiver);

};

gboolean
bb_cut_receiver_can_cut(BbCutReceiver *receiver);

void
bb_cut_receiver_cut(BbCutReceiver *receiver);

#endif
