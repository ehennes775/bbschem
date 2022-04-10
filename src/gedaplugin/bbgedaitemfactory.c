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
#include "bbgedaitemfactory.h"

G_DEFINE_INTERFACE(BbGedaItemFactory, bb_geda_item_factory, G_TYPE_OBJECT)

BbGedaItem*
bb_geda_item_factory_create_missing(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GError **error
    );

static void
bb_geda_item_factory_create_async_missing(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );

static BbGedaItem*
bb_geda_item_factory_create_finish_default(
    BbGedaItemFactory *factory,
    GAsyncResult *result,
    GError **error
    );

static void
bb_geda_item_factory_register_type(GTypeModule *module);


static GType type_id = G_TYPE_INVALID;


static void
bb_geda_item_factory_default_init(BbGedaItemFactoryInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->create = bb_geda_item_factory_create_missing;
    iface->create_async = bb_geda_item_factory_create_async_missing;
    iface->create_finish = bb_geda_item_factory_create_finish_default;
}


BbGedaItem*
bb_geda_item_factory_create(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GError **error
    )
{
    g_return_val_if_fail(BB_IS_GEDA_ITEM_FACTORY(factory), NULL);

    BbGedaItemFactoryInterface *iface = BB_GEDA_ITEM_FACTORY_GET_IFACE(factory);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->create != NULL, NULL);

    return iface->create(factory, version, params, stream, error);
}


BbGedaItem*
bb_geda_item_factory_create_missing(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GError **error
    )
{
    g_error("bb_geda_item_factory_create() not overridden");
}


void
bb_geda_item_factory_create_async(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    g_return_if_fail(BB_IS_GEDA_ITEM_FACTORY(factory));

    BbGedaItemFactoryInterface *iface = BB_GEDA_ITEM_FACTORY_GET_IFACE(factory);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->create_async != NULL);

    iface->create_async(factory, version, params, stream, cancellable, callback, user_data);
}


static void
bb_geda_item_factory_create_async_missing(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    g_error("bb_geda_item_factory_create_async() not overridden");
}


BbGedaItem*
bb_geda_item_factory_create_finish(
    BbGedaItemFactory *factory,
    GAsyncResult *result,
    GError **error
    )
{
    g_return_val_if_fail(BB_IS_GEDA_ITEM_FACTORY(factory), NULL);

    BbGedaItemFactoryInterface *iface = BB_GEDA_ITEM_FACTORY_GET_IFACE(factory);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->create_finish != NULL, NULL);

    return iface->create_finish(factory, result, error);
}


static BbGedaItem*
bb_geda_item_factory_create_finish_default(
    BbGedaItemFactory *factory,
    GAsyncResult *result,
    GError **error
    )
{
    g_return_val_if_fail(BB_IS_GEDA_ITEM_FACTORY(factory), NULL);
    g_return_val_if_fail(G_IS_TASK(result), NULL);

    return g_task_propagate_pointer(G_TASK(result), error);
}


//GType
//bb_geda_item_factory_get_type()
//{
//    /* Catch incorrect order in dynamic type registration */
//    g_assert(type_id != G_TYPE_INVALID);
//
//    return type_id;
//}


//void
//bb_geda_item_factory_register(GTypeModule *module)
//{
//    bb_geda_item_factory_register_type(module);
//}
//
//
///*
// * Registration for a dynamic interface
// */
//static void
//bb_geda_item_factory_register_type(GTypeModule *module)
//{
//    const GTypeInfo type_info =
//    {
//        .class_size = sizeof(BbGedaItemFactoryInterface),
//        .class_init = (GClassInitFunc) bb_geda_item_factory_default_init
//    };
//
//    type_id = g_type_module_register_type(
//        module,
//        G_TYPE_INTERFACE,
//        g_intern_static_string("BbGedaItemFactory"),
//        &type_info,
//        0
//        );
//}