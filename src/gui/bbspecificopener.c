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


static void
bb_specific_opener_open_async_missing(
    BbSpecificOpener *specific_opener,
    GFile *file,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );

gboolean
bb_specific_opener_open_finish_missing(
    BbSpecificOpener *specific_opener,
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


void
bb_specific_opener_open_async(
    BbSpecificOpener *specific_opener,
    GFile *file,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    g_return_if_fail(BB_IS_SPECIFIC_OPENER(specific_opener));

    BbSpecificOpenerInterface *iface = BB_SPECIFIC_OPENER_GET_IFACE(specific_opener);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->open_async != NULL);

    return iface->open_async(specific_opener, file, cancellable, callback, user_data);
}


static void
bb_specific_opener_open_async_missing(
    BbSpecificOpener *specific_opener,
    GFile *file,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    g_error("bb_specific_opener_open_async() not overridden");
}


gboolean
bb_specific_opener_open_finish(
    BbSpecificOpener *specific_opener,
    GAsyncResult *result,
    GError **error
    )
{
    g_return_val_if_fail(BB_IS_SPECIFIC_OPENER(specific_opener), FALSE);

    BbSpecificOpenerInterface *iface = BB_SPECIFIC_OPENER_GET_IFACE(specific_opener);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->open_finish != NULL, FALSE);

    return iface->open_finish(specific_opener, result, error);
}


gboolean
bb_specific_opener_open_finish_missing(
    BbSpecificOpener *specific_opener,
    GAsyncResult *result,
    GError **error
    )
{
    g_error("bb_specific_opener_open_finish() not overridden");
}
