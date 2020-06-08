#ifndef __BBITEMPARAMS__
#define __BBITEMPARAMS__
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

#define BB_TYPE_ITEM_PARAMS bb_item_params_get_type()

typedef struct _BbItemParams BbItemParams;


BbItemParams*
bb_item_params_copy(const BbItemParams *params);

void
bb_item_params_free(BbItemParams *params);

void
bb_item_params_write_async(
    BbItemParams *params,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

void
bb_item_params_write_finish(
    BbItemParams *params,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );

#endif
