#ifndef __BBGEDAITEMFACTORY__
#define __BBGEDAITEMFACTORY__
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
#include "bbschematicitem.h"
#include "bbgedaversion.h"

#define BB_TYPE_GEDA_ITEM_FACTORY bb_geda_item_factory_get_type()
G_DECLARE_INTERFACE(BbGedaItemFactory, bb_geda_item_factory, BB, GEDA_ITEM_FACTORY, GObject)

struct _BbGedaItemFactoryInterface
{
    GTypeInterface g_iface;

    BbSchematicItem* (*create)(
        BbGedaItemFactory *factory,
        BbGedaVersion *version,
        BbParams *params,
        GDataInputStream *stream,
        GError **error
        );

    void (*create_async)(
        BbGedaItemFactory *factory,
        BbGedaVersion *version,
        BbParams *params,
        GDataInputStream *stream,
        GCancellable *cancellable,
        GAsyncReadyCallback callback,
        gpointer user_data
        );

    BbSchematicItem* (*create_finish)(
        BbGedaItemFactory *factory,
        GAsyncResult *result,
        GError **error
        );
};


/**
 * Synchronously read a gEDA schematic item from a GDataInputStream
 *
 * @param factory A BbGedaItemFactory
 * @param version The gEDA version from the beginning of the input stream
 * @param params The first line of the item from the input stream converted to BbParams
 * @param stream The input stream containing the rest of the item, if needed
 * @param error An optional location to store any error encountered
 * @return The BbSchematicItem created from the operation, or NULL if encountering an error
 */
BbSchematicItem*
bb_geda_item_factory_create(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GError **error
    );


/**
 * Asynchronously read a gEDA schematic item from a GDataInputStream
 *
 * @param factory A BbGedaItemFactory
 * @param version The gEDA version from the beginning of the input stream
 * @param params The first line of the item from the input stream converted to BbParams
 * @param stream The input stream containing the rest of the item, if needed
 * @param callback A function to call when the result of the asynchronous read is ready
 * @param user_data User data to pass to the GAsyncReadyCallback
 */
void
bb_geda_item_factory_create_async(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );


/**
 * Obtain the result of asynchronous creation from input stream
 *
 * @param factory The BbGedaItemFactory that initiated the asynchronous operation
 * @param result The result passed to the GAsyncReadyCallback
 * @param error An optional location to store any error encountered
 * @return The BbSchematicItem created from the operation, or NULL if encountering an error
 */
BbSchematicItem*
bb_geda_item_factory_create_finish(
    BbGedaItemFactory *factory,
    GAsyncResult *result,
    GError **error
    );


#endif
