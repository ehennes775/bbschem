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
#include "bbquitreceiver.h"


G_DEFINE_INTERFACE(BbQuitReceiver, bb_quit_receiver, G_TYPE_OBJECT)


void
bb_quit_receiver_quit_default(BbQuitReceiver *receiver);


static void
bb_quit_receiver_default_init(BbQuitReceiverInterface *klasse)
{
    g_return_if_fail(klasse != NULL);

    klasse->quit = bb_quit_receiver_quit_default;
}


void
bb_quit_receiver_quit(BbQuitReceiver *receiver)
{
    g_return_if_fail(BB_IS_QUIT_RECEIVER(receiver));

    BbQuitReceiverInterface *iface = BB_QUIT_RECEIVER_GET_IFACE(receiver);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->quit != NULL);

    iface->quit(receiver);
}


void
bb_quit_receiver_quit_default(BbQuitReceiver *receiver)
{
    g_message("Not implemented: bb_quit_receiver_quit");
}
