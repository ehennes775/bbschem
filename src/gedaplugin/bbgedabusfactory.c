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
#include "bbgedabusfactory.h"
#include "bbgedaitemfactory.h"
#include "bberror.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


struct _BbGedaBusFactory
{
    GObject parent;
};


BbGedaItem*
bb_geda_bus_factory_create(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GError **error
    );

static void
bb_geda_bus_factory_create_async(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );

static void
bb_geda_bus_factory_dispose(GObject *object);

static void
bb_geda_bus_factory_finalize(GObject *object);

static void
bb_geda_bus_factory_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_bus_factory_item_factory_init(BbGedaItemFactoryInterface *iface);

static void
bb_geda_bus_factory_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_DYNAMIC_TYPE_EXTENDED(
    BbGedaBusFactory,
    bb_geda_bus_factory,
    G_TYPE_OBJECT,
    0,
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_GEDA_ITEM_FACTORY, bb_geda_bus_factory_item_factory_init)
    )


BbGedaItem*
bb_geda_bus_factory_create(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GError **error
    )
{
    return BB_GEDA_ITEM(bb_geda_bus_new_with_params(params, error));
}


static void
bb_geda_bus_factory_create_async(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    GTask *task = g_task_new(factory, cancellable, callback, user_data);

    GError *local_error = NULL;

    BbGedaBus *bus = bb_geda_bus_new_with_params(params, &local_error);

    if (!g_task_return_error_if_cancelled(task))
    {
        if (local_error != NULL)
        {
            g_task_return_error(task, local_error);
        }
        else if (bus == NULL)
        {
            g_task_return_new_error(
                task,
                BB_ERROR_DOMAIN,
                0,
                "Internal error: " __FILE__ " line %d", __LINE__
                );
        }
        else
        {
            g_task_return_pointer(task, bus, NULL);
        }
    }

    g_object_unref(task);
}


static void
bb_geda_bus_factory_class_init(BbGedaBusFactoryClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_geda_bus_factory_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_geda_bus_factory_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_geda_bus_factory_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_geda_bus_factory_set_property;
}


static void
bb_geda_bus_factory_class_finalize(BbGedaBusFactoryClass *klasse)
{
}


static void
bb_geda_bus_factory_dispose(GObject *object)
{
}


static void
bb_geda_bus_factory_finalize(GObject *object)
{
}


static void
bb_geda_bus_factory_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_1:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_bus_factory_init(BbGedaBusFactory *window)
{
}


static void
bb_geda_bus_factory_item_factory_init(BbGedaItemFactoryInterface *iface)
{
    iface->create = bb_geda_bus_factory_create;
    iface->create_async = bb_geda_bus_factory_create_async;
}

BbGedaItemFactory*
bb_geda_bus_factory_new()
{
    return BB_GEDA_ITEM_FACTORY(g_object_new(
        BB_TYPE_GEDA_BUS_FACTORY,
        NULL
        ));
}


void
bb_geda_bus_factory_register(GTypeModule *module)
{
    bb_geda_bus_factory_register_type(module);
}


static void
bb_geda_bus_factory_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_1:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
