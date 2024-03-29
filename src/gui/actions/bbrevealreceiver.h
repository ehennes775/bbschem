#ifndef __BBREVEALRECEIVER__
#define __BBREVEALRECEIVER__
/*
 * bbschem
 * Copyright (C) 2020 Edward C. Hennessy
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

#define BB_TYPE_REVEAL_RECEIVER bb_reveal_receiver_get_type()
G_DECLARE_INTERFACE(BbRevealReceiver, bb_reveal_receiver, BB, REVEAL_RECEIVER, GObject)

struct _BbRevealReceiverInterface
{
    GTypeInterface g_iface;

    gboolean (*get_reveal)(BbRevealReceiver *reveal_receiver);
    void (*set_reveal)(BbRevealReceiver *reveal_receiver, gboolean reveal);
};

gboolean
bb_reveal_receiver_get_reveal(BbRevealReceiver *reveal_receiver);

void
bb_reveal_receiver_set_reveal(BbRevealReceiver *reveal_receiver, gboolean reveal);

#endif
