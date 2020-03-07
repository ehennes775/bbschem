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
#include "bbitembuilder.h"


enum
{
    PROP_0,
    N_PROPERTIES
};


static void
bb_item_builder_dispose(GObject *object);

static void
bb_item_builder_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_item_builder_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


G_DEFINE_ABSTRACT_TYPE(BbItemBuilder, bb_item_builder, G_TYPE_OBJECT)


static GParamSpec *properties[N_PROPERTIES];


static void
bb_item_builder_class_init(BbItemBuilderClass *class)
{
    g_return_if_fail(G_OBJECT_CLASS(class) != NULL);

    G_OBJECT_CLASS(class)->dispose = bb_item_builder_dispose;
    G_OBJECT_CLASS(class)->get_property = bb_item_builder_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_item_builder_set_property;

    for (int index = PROP_0 + 1; index < N_PROPERTIES; ++index)
    {
        g_object_class_install_property(
            G_OBJECT_CLASS(class),
            index,
            properties[index]
        );
    }
}


static void
bb_item_builder_dispose(GObject *object)
{
    g_return_if_fail(object != NULL);
}


static void
bb_item_builder_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_item_builder_init(BbItemBuilder *plugin)
{
    g_return_if_fail(plugin != NULL);
}


static void
bb_item_builder_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
