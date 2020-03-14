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

static void
bb_schematic_item_dispose(GObject *object);

static void
bb_schematic_item_finalize(GObject *object);

static void
bb_schematic_item_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_schematic_item_render_missing(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_schematic_item_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


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
bb_schematic_item_class_init(BbSchematicItemClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_schematic_item_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_schematic_item_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_schematic_item_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_schematic_item_set_property;

    klasse->calculate_bounds = bb_schematic_item_calculate_bounds_missing;
    klasse->render = bb_schematic_item_render_missing;
}


static void
bb_schematic_item_dispose(GObject *object)
{
    // BbSchematicitem* privat = BBSCHEMATICITEM_GET_PRIVATE(object);
}


static void
bb_schematic_item_finalize(GObject *object)
{
    // BbSchematicitem* privat = BBSCHEMATICITEM_GET_PRIVATE(object);
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
