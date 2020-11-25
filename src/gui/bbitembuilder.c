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
#include <bbextensions.h>
#include "bbitembuilder.h"


enum
{
    PROP_0,
    N_PROPERTIES
};


GSList*
bb_item_builder_create_items_missing(BbItemBuilder *builder);

static void
bb_item_builder_dispose(GObject *object);

static void
bb_item_builder_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_item_builder_put_point_missing(BbItemBuilder *builder, int index, int x, int y);

static void
bb_item_builder_render_items_missing(BbItemBuilder *builder, BbItemRenderer *renderer);

static void
bb_item_builder_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


G_DEFINE_ABSTRACT_TYPE(BbItemBuilder, bb_item_builder, G_TYPE_OBJECT)


static GParamSpec *properties[N_PROPERTIES];


static void
bb_item_builder_class_init(BbItemBuilderClass *class)
{
    g_return_if_fail(G_OBJECT_CLASS(class) != NULL);
    g_return_if_fail(BB_ITEM_BUILDER_CLASS(class) != NULL);

    G_OBJECT_CLASS(class)->dispose = bb_item_builder_dispose;
    G_OBJECT_CLASS(class)->get_property = bb_item_builder_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_item_builder_set_property;

    BB_ITEM_BUILDER_CLASS(class)->create_items = bb_item_builder_create_items_missing;
    BB_ITEM_BUILDER_CLASS(class)->put_point = bb_item_builder_put_point_missing;
    BB_ITEM_BUILDER_CLASS(class)->render_items = bb_item_builder_render_items_missing;

    for (int index = PROP_0 + 1; index < N_PROPERTIES; ++index)
    {
        bb_object_class_install_property(
            G_OBJECT_CLASS(class),
            index,
            properties[index]
        );
    }
}

GSList*
bb_item_builder_create_items(BbItemBuilder *builder)
{
    g_return_val_if_fail(BB_ITEM_BUILDER_GET_CLASS(builder) != NULL, NULL);

    return BB_ITEM_BUILDER_GET_CLASS(builder)->create_items(builder);
}


GSList*
bb_item_builder_create_items_missing(BbItemBuilder *builder)
{
    g_error("bb_item_builder_create_items() not overridden");
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


void
bb_item_builder_put_point(BbItemBuilder *builder, int index, int x, int y)
{
    g_return_if_fail(BB_ITEM_BUILDER_GET_CLASS(builder) != NULL);

    BB_ITEM_BUILDER_GET_CLASS(builder)->put_point(builder, index, x, y);
}


static void
bb_item_builder_put_point_missing(BbItemBuilder *builder, int index, int x, int y)
{
    g_error("bb_item_builder_put_point() not overridden");
}


void
bb_item_builder_render_items(BbItemBuilder *builder, BbItemRenderer *renderer)
{
    BbItemBuilderClass *class = BB_ITEM_BUILDER_GET_CLASS(builder);

    g_return_if_fail(class != NULL);
    g_return_if_fail(class->render_items != NULL);

    class->render_items(builder, renderer);
}


static void
bb_item_builder_render_items_missing(BbItemBuilder *builder, BbItemRenderer *renderer)
{
    g_error("bb_item_builder_render_items() not overridden");
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
