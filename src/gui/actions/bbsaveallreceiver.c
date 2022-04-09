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
#include "bbsaveallreceiver.h"


static gboolean
bb_save_all_receiver_get_save_all_missing(BbSaveAllReceiver *save_all_receiver);

void
bb_save_all_receiver_set_save_all_missing(BbSaveAllReceiver *save_all_receiver, gboolean save_all);


G_DEFINE_INTERFACE(
    BbSaveAllReceiver,
    bb_save_all_receiver,
    G_TYPE_OBJECT
    )


static void
bb_save_all_receiver_default_init(BbSaveAllReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "save_all",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );
}
