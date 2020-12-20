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
#include "bberror.h"
#include "bbgedafactory.h"
#include "bbgedaitemfactory.h"
#include "bbgedalinefactory.h"
#include "bbgedaarc.h"
#include "bbgedaarcfactory.h"
#include "bbgedabox.h"
#include "bbgedacircle.h"
#include "bbgedacirclefactory.h"
#include "bbgedaboxfactory.h"
#include "bbgedabusfactory.h"
#include "bbgedanetfactory.h"
#include "bbgedapinfactory.h"
#include "bbgedablockfactory.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


struct _BbGedaFactory
{
    GObject parent;

    /**
     * A lookup table for obtaining a specific item factory given the item token
     *
     * The key contains the token for the item type (e.g. "L" for line, "V" for circle)
     * The value contains a BbGedaItemFactory for the specific item type
     */
    GHashTable *table;
};


static void
bb_geda_factory_create_ready(BbGedaFactory *factory, GAsyncResult *result, GTask *task);

static void
bb_geda_factory_dispose(GObject *object);

static void
bb_geda_factory_finalize(GObject *object);

static void
bb_geda_factory_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_factory_geda_item_factory(BbGedaItemFactoryInterface *iface);

static void
bb_geda_factory_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbGedaFactory,
    bb_geda_factory,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(BB_TYPE_GEDA_ITEM_FACTORY, bb_geda_factory_geda_item_factory)
    )


void
bb_geda_factory_add_factory(BbGedaFactory *factory, const gchar *token, BbGedaItemFactory *item_factory)
{
    g_hash_table_insert(
        factory->table,
        g_strdup(token),
        g_object_ref(item_factory)
        );
}


static void
bb_geda_factory_class_init(BbGedaFactoryClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_geda_factory_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_geda_factory_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_geda_factory_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_geda_factory_set_property;
}


BbGedaItem*
bb_geda_factory_create(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GError **error
    )
{
    BbGedaFactory *geda_factory = BB_GEDA_FACTORY(factory);
    g_return_val_if_fail(geda_factory != NULL, NULL);

    BbGedaItem *item = NULL;
    GError *local_error = NULL;

    BbGedaItemFactory *specific_factory = g_hash_table_lookup(
        geda_factory->table,
        bb_params_get_token(params)
        );

    if (specific_factory == NULL)
    {
        local_error = g_error_new(
            BB_ERROR_DOMAIN,
            ERROR_UNKNOWN_ITEM_TOKEN,
            "Unknown gEDA item type: '%s'",
            bb_params_get_token(params)
            );
    }
    else
    {
        item = bb_geda_item_factory_create(
            specific_factory,
            version,
            params,
            stream,
            &local_error
            );
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        g_clear_object(&item);
    }

    return item;
}


void
bb_geda_factory_create_async(
    BbGedaItemFactory *factory,
    BbGedaVersion *version,
    BbParams *params,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    )
{
    BbGedaFactory *geda_factory = BB_GEDA_FACTORY(factory);
    g_return_if_fail(geda_factory != NULL);

    GTask *task = g_task_new(
        factory,
        cancellable,
        callback,
        user_data
        );

    const gchar *token = bb_params_get_token(params);

    BbGedaItemFactory *specific_factory = g_hash_table_lookup(
        geda_factory->table,
        token
        );

    if (specific_factory == NULL)
    {
        g_task_return_new_error(
            task,
            BB_ERROR_DOMAIN,
            ERROR_UNKNOWN_ITEM_TOKEN,
            "Unknown gEDA item type: '%s'",
            token
            );

        g_object_unref(task);
    }
    else
    {
        bb_geda_item_factory_create_async(
            specific_factory,
            version,
            params,
            stream,
            cancellable,
            (GAsyncReadyCallback) bb_geda_factory_create_ready,
            task
            );
    }
}


static void
bb_geda_factory_create_ready(BbGedaFactory *factory, GAsyncResult *result, GTask *task)
{
    GError *local_error = NULL;

    BbGedaItem *item = bb_geda_item_factory_create_finish(
        BB_GEDA_ITEM_FACTORY(factory),
        result,
        &local_error
        );

    if (local_error != NULL)
    {
        g_task_return_error(task, local_error);
        g_object_unref(task);
    }
    else if (item == NULL)
    {
        g_task_return_new_error(
            task,
            BB_ERROR_DOMAIN,
            0,
            "Internal error"
            );

        g_object_unref(task);
    }
    else
    {
        g_task_return_pointer(task, item, NULL);
        g_object_unref(task);
    }
}


static void
bb_geda_factory_dispose(GObject *object)
{
}


static void
bb_geda_factory_finalize(GObject *object)
{
    BbGedaFactory *factory = BB_GEDA_FACTORY(object);
    g_return_if_fail(factory != NULL);

    g_hash_table_destroy(factory->table);
    factory->table = NULL;
}


static void
bb_geda_factory_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
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
bb_geda_factory_init(BbGedaFactory *factory)
{
    g_return_if_fail(BB_IS_GEDA_FACTORY(factory));

    factory->table = g_hash_table_new_full(
        g_str_hash,
        g_str_equal,
        g_free,
        g_object_unref
        );

    bb_geda_factory_add_factory(
        factory,
        BB_GEDA_ARC_TOKEN,
        bb_geda_arc_factory_new()
        );

    bb_geda_factory_add_factory(
        factory,
        BB_GEDA_BOX_TOKEN,
        bb_geda_box_factory_new()
        );

    bb_geda_factory_add_factory(
        factory,
        BB_GEDA_CIRCLE_TOKEN,
        bb_geda_circle_factory_new()
        );

    bb_geda_factory_add_factory(
        factory,
        BB_GEDA_LINE_TOKEN,
        bb_geda_line_factory_new()
        );

    bb_geda_factory_add_factory(
        factory,
        BB_GEDA_BUS_TOKEN,
        bb_geda_bus_factory_new()
        );

    bb_geda_factory_add_factory(
        factory,
        BB_GEDA_NET_TOKEN,
        bb_geda_net_factory_new()
        );

    bb_geda_factory_add_factory(
        factory,
        BB_GEDA_PIN_TOKEN,
        bb_geda_pin_factory_new()
        );

    bb_geda_factory_add_factory(
        factory,
        BB_GEDA_BLOCK_TOKEN,
        bb_geda_block_factory_new()
        );
}


static void
bb_geda_factory_geda_item_factory(BbGedaItemFactoryInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->create = bb_geda_factory_create;
    iface->create_async = bb_geda_factory_create_async;
}


BbGedaFactory*
bb_geda_factory_new()
{
    return BB_GEDA_FACTORY(g_object_new(
        BB_TYPE_GEDA_FACTORY,
        NULL
        ));
}


static void
bb_geda_factory_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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
