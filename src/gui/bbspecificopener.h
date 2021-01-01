#ifndef __BBSPECIFICOPENER__
#define __BBSPECIFICOPENER__
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
#include "bbdocumentwindow.h"


#define BB_TYPE_SPECIFIC_OPENER bb_specific_opener_get_type()
G_DECLARE_INTERFACE(BbSpecificOpener, bb_specific_opener, BB, SPECIFIC_OPENER, GObject)


struct _BbSpecificOpenerInterface
{
    GTypeInterface g_iface;

    void (*open_async)(
        BbSpecificOpener *specific_opener,
        GFile *file,
        GCancellable *cancellable,
        GAsyncReadyCallback callback,
        gpointer user_data
        );

    gboolean (*open_finish)(
        BbSpecificOpener *specific_opener,
        GAsyncResult *result,
        GError **error
        );
};


void
bb_specific_opener_open_async(
    BbSpecificOpener *specific_opener,
    GFile *file,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );


gboolean
bb_specific_opener_open_finish(
    BbSpecificOpener *specific_opener,
    GAsyncResult *result,
    GError **error
    );

#endif
