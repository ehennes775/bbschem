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
#include "bbgraphicpath.h"


enum
{
    PROP_0,
    PROP_WIDTH,
    N_PROPERTIES
};


struct _BbGraphicPath
{
    BbSchematicItem parent;

    int width;
};


G_DEFINE_TYPE(BbGraphicPath, bb_graphic_path, BB_TYPE_SCHEMATIC_ITEM)


static BbBounds*
bb_graphic_path_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static void
bb_graphic_path_dispose(GObject *object);

static void
bb_graphic_path_finalize(GObject *object);

static void
bb_graphic_path_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphic_path_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_graphic_path_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


static BbBounds*
bb_graphic_path_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator)
{
    BbGraphicPath *path = BB_GRAPHIC_PATH(item);

    g_return_val_if_fail(path != NULL, NULL);

    return NULL;
}


static void
bb_graphic_path_class_init(BbGraphicPathClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_graphic_path_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_graphic_path_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_graphic_path_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_graphic_path_set_property;

    BB_SCHEMATIC_ITEM_CLASS(klasse)->calculate_bounds = bb_graphic_path_calculate_bounds;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->render = bb_graphic_path_render;

    properties[PROP_WIDTH] = g_param_spec_int(
        "width",
        "Line Width",
        "The line width",
        0,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    for (int index = PROP_0 + 1; index < N_PROPERTIES; ++index)
    {
        g_object_class_install_property(
            G_OBJECT_CLASS(klasse),
            index,
            properties[index]
            );
    }
}


static void
bb_graphic_path_dispose(GObject *object)
{
}


static void
bb_graphic_path_finalize(GObject *object)
{
}


static void
bb_graphic_path_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WIDTH:
            g_value_set_int(value, bb_graphic_path_get_width(BB_GRAPHIC_PATH(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_graphic_path_get_width(BbGraphicPath *path)
{
    g_return_val_if_fail(path != NULL, 0);

    return path->width;
}


static void
bb_graphic_path_init(BbGraphicPath *window)
{
}


__attribute__((constructor)) void
bb_graphic_path_register()
{
    bb_graphic_path_get_type();
}


static void
bb_graphic_path_render(BbSchematicItem *item, BbItemRenderer *renderer)
{
    bb_item_renderer_render_graphic_path(renderer, BB_GRAPHIC_PATH(item));
}


static void
bb_graphic_path_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WIDTH:
            bb_graphic_path_set_width(BB_GRAPHIC_PATH(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_graphic_path_set_width(BbGraphicPath *path, int width)
{
    g_return_if_fail(path != NULL);

    if (path->width != width)
    {
        path->width = width;

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_WIDTH]);
    }
}


static void
bb_graphic_line_translate(BbSchematicItem *item, int dx, int dy)
{
    BbGraphicPath *path = BB_GRAPHIC_PATH(item);
    g_return_if_fail(path != NULL);
}
