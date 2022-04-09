#ifndef __BBSAVERECEIVER__
#define __BBSAVERECEIVER__
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

#include <gtk/gtk.h>

#define BB_TYPE_SAVE_RECEIVER bb_save_receiver_get_type()
G_DECLARE_INTERFACE(BbSaveReceiver, bb_save_receiver, BB, SAVE_RECEIVER, GObject)

struct _BbSaveReceiverInterface
{
    GTypeInterface g_iface;

    gboolean (*get_can_save)(BbSaveReceiver *save_receiver);
    gboolean (*get_can_save_as)(BbSaveReceiver *save_receiver);
    void (*save)(BbSaveReceiver *save_receiver, GError **error);
    void (*save_as)(BbSaveReceiver *save_receiver, GError **error);
};


/**
 * Indicates the underlying document can be saved
 *
 * @param save_receiver A BbSaveReceiver
 * @return TRUE if the bb_save_receiver_save() functionality is available
 */
gboolean
bb_save_receiver_get_can_save(BbSaveReceiver *save_receiver);


/**
 * Indicates the underlying document can be saved with an alternate filename
 *
 * @param save_receiver A BbSaveReceiver
 * @return TRUE if the bb_save_receiver_save_as() functionality is available
 */
gboolean
bb_save_receiver_get_can_save_as(BbSaveReceiver *save_receiver);


/**
 * Save the underlying document
 *
 * @param save_receiver A BbSaveReceiver
 */
void
bb_save_receiver_save(BbSaveReceiver *save_receiver, GError **error);


/**
 * Save the underlying document with an alternate filename
 *
 * @param save_receiver A BbSaveReceiver
 */
void
bb_save_receiver_save_as(BbSaveReceiver *save_receiver, GError **error);


#endif
