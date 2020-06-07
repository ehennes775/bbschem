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
#include "bbschematicitem.h"


enum
{
    PROP_0,
    N_PROPERTIES
};


//struct _BbSchematicItem
//{
//   GObject parent;
//};


G_DEFINE_TYPE(BbSchematicItem, bb_schematic_item, G_TYPE_OBJECT);


static BbBounds*
bb_schematic_item_calculate_bounds_missing(BbSchematicItem *item, BbBoundsCalculator *calculator);

static BbSchematicItem*
bb_schematic_item_clone_missing(BbSchematicItem *item);

static void
bb_schematic_item_dispose(GObject *object);

static void
bb_schematic_item_finalize(GObject *object);

static void
bb_schematic_item_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static gboolean
bb_schematic_item_is_significant_missing(BbSchematicItem *item);

static void
bb_schematic_item_render_missing(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_schematic_item_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_schematic_item_translate_missing(BbSchematicItem *item, int dx, int dy);


GParamSpec *properties[N_PROPERTIES];


BbBounds*
bb_schematic_item_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator)
{
    BbSchematicItemClass *class = BB_SCHEMATIC_ITEM_GET_CLASS(item);

    g_return_val_if_fail(class != NULL, NULL);
    g_return_val_if_fail(class->calculate_bounds != NULL, NULL);

    return class->calculate_bounds(item, calculator);
}


static BbBounds*
bb_schematic_item_calculate_bounds_missing(BbSchematicItem *item, BbBoundsCalculator *calculator)
{
    g_error("bb_schematic_item_calculate_bounds() not overridden");
}


static void
bb_schematic_item_class_init(BbSchematicItemClass *class)
{
    G_OBJECT_CLASS(class)->dispose = bb_schematic_item_dispose;
    G_OBJECT_CLASS(class)->finalize = bb_schematic_item_finalize;
    G_OBJECT_CLASS(class)->get_property = bb_schematic_item_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_schematic_item_set_property;

    class->calculate_bounds = bb_schematic_item_calculate_bounds_missing;
    class->clone = bb_schematic_item_clone_missing;
    class->is_significant = bb_schematic_item_is_significant;
    class->render = bb_schematic_item_render_missing;
    class->translate = bb_schematic_item_translate_missing;
}


BbSchematicItem*
bb_schematic_item_clone(BbSchematicItem *item)
{
    BbSchematicItemClass *class = BB_SCHEMATIC_ITEM_GET_CLASS(item);

    g_return_val_if_fail(class != NULL, FALSE);
    g_return_val_if_fail(class->clone != NULL, FALSE);

    return class->clone(item);
}


static BbSchematicItem*
bb_schematic_item_clone_missing(BbSchematicItem *item)
{
    g_error("bb_schematic_item_clone() not overridden");
}


static void
bb_schematic_item_dispose(GObject *object)
{
}


static void
bb_schematic_item_finalize(GObject *object)
{
}


static void
bb_schematic_item_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_schematic_item_init(BbSchematicItem *window)
{
}


gboolean
bb_schematic_item_is_significant(BbSchematicItem *item)
{
    BbSchematicItemClass *class = BB_SCHEMATIC_ITEM_GET_CLASS(item);

    g_return_val_if_fail(class != NULL, FALSE);
    g_return_val_if_fail(class->is_significant != NULL, FALSE);

    return class->is_significant(item);
}


static gboolean
bb_schematic_item_is_significant_missing(BbSchematicItem *item)
{
    g_error("bb_schematic_item_is_significant() not overridden");
}


__attribute__((constructor)) void
bb_schematic_item_register()
{
    bb_schematic_item_get_type();
}


void
bb_schematic_item_render(BbSchematicItem *item, BbItemRenderer *renderer)
{
    BbSchematicItemClass *class = BB_SCHEMATIC_ITEM_GET_CLASS(item);

    g_return_if_fail(class != NULL);
    g_return_if_fail(class->render != NULL);

    class->render(item, renderer);
}


static void
bb_schematic_item_render_missing(BbSchematicItem *item, BbItemRenderer *renderer)
{
    g_error("bb_schematic_item_render() not overridden");
}


static void
bb_schematic_item_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_schematic_item_translate_missing(BbSchematicItem *item, int dx, int dy)
{
    g_error("bb_schematic_item_translate() not overridden");
}
