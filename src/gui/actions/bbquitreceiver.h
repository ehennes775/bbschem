#ifndef __BBQUITRECEIVER__
#define __BBQUITRECEIVER__
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

#include <glib-object.h>

#define BB_TYPE_QUIT_RECEIVER bb_quit_receiver_get_type()
G_DECLARE_INTERFACE(BbQuitReceiver, bb_quit_receiver, BB, QUIT_RECEIVER, GObject)

struct _BbQuitReceiverInterface
{
    GTypeInterface g_iface;

    void (*quit)(BbQuitReceiver *receiver);
};

void
bb_quit_receiver_quit(BbQuitReceiver *receiver);

#endif
