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
#include "bbspecificopener.h"


static BbDocumentWindow*
bb_specific_opener_open_async_missing(
    BbSpecificOpener *opener,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );

void*
bb_specific_opener_open_finish_missing(
    BbSpecificOpener *opener,
    GAsyncResult *result,
    GError **error
    );


G_DEFINE_INTERFACE(BbSpecificOpener, bb_specific_opener, G_TYPE_OBJECT)


static void
bb_specific_opener_default_init(BbSpecificOpenerInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->open_async = bb_specific_opener_open_async_missing;
    iface->open_finish = bb_specific_opener_open_finish_missing;
}


BbDocumentWindow*
bb_specific_opener_open_async(
    BbSpecificOpener *opener,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    g_return_val_if_fail(opener != NULL, NULL);

    BbSpecificOpenerInterface *iface = BB_SPECIFIC_OPENER_GET_IFACE(opener);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->open_async != NULL, NULL);

    return iface->open_async(opener, stream, cancellable, callback, user_data);
}


static BbDocumentWindow*
bb_specific_opener_open_async_missing(
    BbSpecificOpener *opener,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    g_error("bb_specific_opener_open_async() not overridden");
}


void*
bb_specific_opener_open_finish(
    BbSpecificOpener *opener,
    GAsyncResult *result,
    GError **error
    )
{
    g_return_val_if_fail(opener != NULL, NULL);

    BbSpecificOpenerInterface *iface = BB_SPECIFIC_OPENER_GET_IFACE(opener);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->open_finish != NULL, NULL);

    return iface->open_finish(opener, result, error);
}


gpointer
bb_specific_opener_open_finish_missing(
    BbSpecificOpener *opener,
    GAsyncResult *result,
    GError **error
    )
{
    g_error("bb_specific_opener_open_finish() not overridden");
}
