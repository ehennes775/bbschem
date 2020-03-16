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
#include "bbschematic.h"
#include "bbschematicitem.h"
#include "bbgraphicline.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


struct _BbSchematic
{
    GObject parent;

    GSList *items;
};


G_DEFINE_TYPE(BbSchematic, bb_schematic, G_TYPE_OBJECT);

typedef struct _ApplyItemPropertyCapture ApplyItemPropertyCapture;

struct _ApplyItemPropertyCapture
{
    const char *name;
    const GValue *value;
};


static void
bb_schematic_add_items_lambda(BbSchematicItem *item, BbSchematic *schematic);

static void
bb_schematic_apply_item_property_lambda(BbSchematicItem *item, ApplyItemPropertyCapture *capture);

static void
bb_schematic_dispose(GObject *object);

static void
bb_schematic_finalize(GObject *object);

static void
bb_schematic_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_schematic_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


void
bb_schematic_add_items(BbSchematic *schematic, GSList *items)
{
    g_return_if_fail(schematic != NULL);

    g_slist_foreach(items, (GFunc) bb_schematic_add_items_lambda, schematic);

    schematic->items = g_slist_concat(schematic->items, items);
}


static void
bb_schematic_add_items_lambda(BbSchematicItem *item, BbSchematic *schematic)
{
    /* attach signal handlers */
}


void
bb_schematic_apply_item_property(BbSchematic *schematic, const char *name, const GValue *value)
{
    ApplyItemPropertyCapture capture;

    capture.name = name;
    capture.value = value;

    g_message("ApplyProperty = %s", capture.name);

    bb_schematic_foreach(schematic, (GFunc) bb_schematic_apply_item_property_lambda, &capture);
}


static void
bb_schematic_apply_item_property_lambda(BbSchematicItem *item, ApplyItemPropertyCapture *capture)
{
    BbSchematicItemClass *class = BB_SCHEMATIC_ITEM_GET_CLASS(item);
    g_return_if_fail(class != NULL);

    GParamSpec *param = g_object_class_find_property(G_OBJECT_CLASS(class), capture->name);

    if (param != NULL)
    {
        g_object_set_property(G_OBJECT(item), capture->name, capture->value);
    }
}


static void
bb_schematic_class_init(BbSchematicClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_schematic_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_schematic_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_schematic_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_schematic_set_property;
}


static void
bb_schematic_dispose(GObject *object)
{
}


static void
bb_schematic_finalize(GObject *object)
{
}


void
bb_schematic_foreach(BbSchematic *schematic, GFunc func, gpointer user_data)
{
    g_return_if_fail(schematic != NULL);

    g_slist_foreach(schematic->items, func, user_data);
}


static void
bb_schematic_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
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
bb_schematic_init(BbSchematic *schematic)
{
    schematic->items = g_slist_append(schematic->items, bb_graphic_line_new());
}


BbSchematic*
bb_schematic_new()
{
    return g_object_new(BB_TYPE_SCHEMATIC, NULL);
}


__attribute__((constructor)) void
bb_schematic_register()
{
    bb_schematic_get_type();
}


static void
bb_schematic_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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
