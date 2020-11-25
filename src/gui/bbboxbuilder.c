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
#include <bbgraphicbox.h>
#include "bbboxbuilder.h"


enum
{
    PROP_0,
    PROP_X0,
    PROP_X1,
    PROP_Y0,
    PROP_Y1,
    N_PROPERTIES
};


struct _BbBoxBuilder
{
    BbItemBuilder parent;

    BbGraphicBox *prototype;

    int x[2];
    int y[2];
};


static void
bb_box_builder_dispose(GObject *object);

static void
bb_box_builder_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_box_builder_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


G_DEFINE_TYPE(BbBoxBuilder, bb_box_builder, BB_TYPE_ITEM_BUILDER)


static GParamSpec *properties[N_PROPERTIES];


static void
bb_box_builder_class_init(BbBoxBuilderClass *class)
{
    g_return_if_fail(G_OBJECT_CLASS(class) != NULL);

    G_OBJECT_CLASS(class)->dispose = bb_box_builder_dispose;
    G_OBJECT_CLASS(class)->get_property = bb_box_builder_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_box_builder_set_property;

    properties[PROP_X0] = g_param_spec_int(
        "x0",
        "x0",
        "The starting X coordinate",
        G_MININT,
        G_MAXINT,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_X1] = g_param_spec_int(
        "x1",
        "x1",
        "The ending X coordinate",
        G_MININT,
        G_MAXINT,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_Y0] = g_param_spec_int(
        "y0",
        "y0",
        "The starting Y coordinate",
        G_MININT,
        G_MAXINT,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_Y1] = g_param_spec_int(
        "y1",
        "y1",
        "The ending Y coordinate",
        G_MININT,
        G_MAXINT,
        0,
        G_PARAM_READWRITE
        );

    for (int index = PROP_0 + 1; index < N_PROPERTIES; ++index)
    {
        bb_object_class_install_property(
            G_OBJECT_CLASS(class),
            index,
            properties[index]
            );
    }
}


static void
bb_box_builder_dispose(GObject *object)
{
    BbBoxBuilder *box_builder = BB_BOX_BUILDER(object);

    g_return_if_fail(box_builder != NULL);

    g_clear_object(&box_builder->prototype);
}


int
bb_box_builder_get_x0(BbBoxBuilder *builder)
{
    g_return_val_if_fail(builder != NULL, 0);

    return builder->x[0];
}


int
bb_box_builder_get_x1(BbBoxBuilder *builder)
{
    g_return_val_if_fail(builder != NULL, 0);

    return builder->x[1];
}


int
bb_box_builder_get_y0(BbBoxBuilder *builder)
{
    g_return_val_if_fail(builder != NULL, 0);

    return builder->y[0];
}


int
bb_box_builder_get_y1(BbBoxBuilder *builder)
{
    g_return_val_if_fail(builder != NULL, 0);

    return builder->y[1];
}


static void
bb_box_builder_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_X0:
            g_value_set_int(value, bb_box_builder_get_x0(BB_BOX_BUILDER(object)));
            break;

        case PROP_X1:
            g_value_set_int(value, bb_box_builder_get_x1(BB_BOX_BUILDER(object)));
            break;

        case PROP_Y0:
            g_value_set_int(value, bb_box_builder_get_y0(BB_BOX_BUILDER(object)));
            break;

        case PROP_Y1:
            g_value_set_int(value, bb_box_builder_get_y1(BB_BOX_BUILDER(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_box_builder_init(BbBoxBuilder *builder)
{
    g_return_if_fail(builder != NULL);

    builder->prototype = bb_graphic_box_new();
}


__attribute__((constructor)) void
bb_box_builder_register()
{
}


void
bb_box_builder_set_x0(BbBoxBuilder *builder, int x)
{
    g_return_if_fail(builder != NULL);

    if (builder->x[0] != x)
    {
        builder->x[0] = x;

        g_object_notify_by_pspec(G_OBJECT(builder), properties[PROP_X0]);
    }
}


void
bb_box_builder_set_x1(BbBoxBuilder *builder, int x)
{
    g_return_if_fail(builder != NULL);

    if (builder->x[1] != x)
    {
        builder->x[1] = x;

        g_object_notify_by_pspec(G_OBJECT(builder), properties[PROP_X1]);
    }
}


void
bb_box_builder_set_y0(BbBoxBuilder *builder, int y)
{
    g_return_if_fail(builder != NULL);

    if (builder->y[0] != y)
    {
        builder->y[0] = y;

        g_object_notify_by_pspec(G_OBJECT(builder), properties[PROP_Y0]);
    }
}


void
bb_box_builder_set_y1(BbBoxBuilder *builder, int y)
{
    g_return_if_fail(builder != NULL);

    if (builder->y[1] != y)
    {
        builder->y[1] = y;

        g_object_notify_by_pspec(G_OBJECT(builder), properties[PROP_Y1]);
    }
}


static void
bb_box_builder_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_X0:
            bb_box_builder_set_x0(BB_BOX_BUILDER(object), g_value_get_int(value));
            break;

        case PROP_X1:
            bb_box_builder_set_x1(BB_BOX_BUILDER(object), g_value_get_int(value));
            break;

        case PROP_Y0:
            bb_box_builder_set_y0(BB_BOX_BUILDER(object), g_value_get_int(value));
            break;

        case PROP_Y1:
            bb_box_builder_set_y1(BB_BOX_BUILDER(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
