/*
 * bbsch
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
#include "bbitemparams.h"


struct _BbItemParams
{
};


G_DEFINE_BOXED_TYPE(BbItemParams, bb_item_params, bb_item_params_copy, bb_item_params_free)


BbItemParams*
bb_item_params_copy(const BbItemParams *bounds)
{
    return g_memdup(bounds, sizeof(BbItemParams));
}


void
bb_item_params_free(BbItemParams *bounds)
{
    g_free(bounds);
}


gboolean
bb_item_params_write(
    BbItemParams *params,
    GOutputStream *stream,
    GCancellable *cancellable,
    GError **error
    )
{
    GString *line = NULL;

    return g_output_stream_write_all(
        stream,
        line->str,
        line->len,
        NULL,
        cancellable,
        error
        );
}


void
bb_item_params_write_async(
    BbItemParams *params,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    g_output_stream_write_all_async(
        stream,
        "",
        0,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


void
bb_item_params_write_finish(
    BbItemParams *params,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    )
{
    g_output_stream_write_all_finish(
        stream,
        result,
        NULL,
        error
        );
}
