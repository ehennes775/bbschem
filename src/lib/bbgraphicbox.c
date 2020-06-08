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
#include "bbgraphicbox.h"
#include "bbcoord.h"
#include "bbitemparams.h"


enum
{
    PROP_0,
    PROP_WIDTH,
    PROP_X0,
    PROP_X1,
    PROP_Y0,
    PROP_Y1,
    N_PROPERTIES
};


struct _BbGraphicBox
{
    BbSchematicItem parent;

    BbItemParams *params;

    int width;

    int x[2];
    int y[2];
};


G_DEFINE_TYPE(BbGraphicBox, bb_graphic_box, BB_TYPE_SCHEMATIC_ITEM)


static BbBounds*
bb_graphic_box_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static void
bb_graphic_box_dispose(GObject *object);

static void
bb_graphic_box_finalize(GObject *object);

static void
bb_graphic_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphic_box_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_graphic_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_graphic_box_translate(BbSchematicItem *item, int dx, int dy);

static void
bb_graphic_box_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static void
bb_graphic_box_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


GParamSpec *properties[N_PROPERTIES];


static BbBounds*
bb_graphic_box_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator)
{
    BbGraphicBox *box = BB_GRAPHIC_BOX(item);

    g_return_val_if_fail(box != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        box->x[0],
        box->y[0],
        box->x[1],
        box->y[1],
        box->width
        );
}


static void
bb_graphic_box_class_init(BbGraphicBoxClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_graphic_box_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_graphic_box_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_graphic_box_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_graphic_box_set_property;

    BB_SCHEMATIC_ITEM_CLASS(klasse)->calculate_bounds = bb_graphic_box_calculate_bounds;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->render = bb_graphic_box_render;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->translate = bb_graphic_box_translate;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_async = bb_graphic_box_write_async;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_finish = bb_graphic_box_write_finish;

    properties[PROP_WIDTH] = g_param_spec_int(
        "width",
        "Line Width",
        "The line width",
        0,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_X0] = g_param_spec_int(
        "x0",
        "x0",
        "The x coordinate of the first endpoint",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_X1] = g_param_spec_int(
        "x1",
        "x1",
        "The x coordinate of the second endpoint",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_Y0] = g_param_spec_int(
        "y0",
        "y0",
        "The y coordinate of the first endpoint",
        INT_MIN,
        INT_MAX,
        0,
        G_PARAM_READWRITE
        );

    properties[PROP_Y1] = g_param_spec_int(
        "y1",
        "y1",
        "The y coordinate of the second endpoint",
        INT_MIN,
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
bb_graphic_box_dispose(GObject *object)
{
}


static void
bb_graphic_box_finalize(GObject *object)
{
}


static void
bb_graphic_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WIDTH:
            g_value_set_int(value, bb_graphic_box_get_width(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_X0:
            g_value_set_int(value, bb_graphic_box_get_x0(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_X1:
            g_value_set_int(value, bb_graphic_box_get_x1(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_Y0:
            g_value_set_int(value, bb_graphic_box_get_y0(BB_GRAPHIC_BOX(object)));
            break;

        case PROP_Y1:
            g_value_set_int(value, bb_graphic_box_get_y1(BB_GRAPHIC_BOX(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_graphic_box_get_width(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->width;
}


int
bb_graphic_box_get_x0(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->x[0];
}


int
bb_graphic_box_get_x1(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->x[1];
}


int
bb_graphic_box_get_y0(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->y[0];
}


int
bb_graphic_box_get_y1(BbGraphicBox *box)
{
    g_return_val_if_fail(box != NULL, 0);

    return box->y[1];
}


static void
bb_graphic_box_init(BbGraphicBox *window)
{
}


BbGraphicBox*
bb_graphic_box_new()
{
    return g_object_new(BB_TYPE_GRAPHIC_BOX, NULL);
}


__attribute__((constructor)) void
bb_graphic_box_register()
{
    bb_graphic_box_get_type();
}


static void
bb_graphic_box_render(BbSchematicItem *item, BbItemRenderer *renderer)
{
    bb_item_renderer_render_graphic_box(renderer, BB_GRAPHIC_BOX(item));
}


static void
bb_graphic_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WIDTH:
            bb_graphic_box_set_width(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_X0:
            bb_graphic_box_set_x0(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_X1:
            bb_graphic_box_set_x1(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_Y0:
            bb_graphic_box_set_y0(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        case PROP_Y1:
            bb_graphic_box_set_y1(BB_GRAPHIC_BOX(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_graphic_box_set_width(BbGraphicBox *box, int width)
{
    g_return_if_fail(box != NULL);

    if (box->width != width)
    {
        box->width = width;

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_WIDTH]);
    }
}


void
bb_graphic_box_set_x0(BbGraphicBox *box, int x)
{
    g_return_if_fail(box != NULL);

    if (box->x[0] != x)
    {
        box->x[0] = x;

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X0]);
    }
}


void
bb_graphic_box_set_x1(BbGraphicBox *box, int x)
{
    g_return_if_fail(box != NULL);

    if (box->x[1] != x)
    {
        box->x[1] = x;

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X1]);
    }
}


void
bb_graphic_box_set_y0(BbGraphicBox *box, int y)
{
    g_return_if_fail(box != NULL);

    if (box->y[0] != y)
    {
        box->y[0] = y;

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y0]);
    }
}


void
bb_graphic_box_set_y1(BbGraphicBox *box, int y)
{
    g_return_if_fail(box != NULL);

    if (box->y[1] != y)
    {
        box->y[1] = y;

        g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y1]);
    };
}


static void
bb_graphic_box_translate(BbSchematicItem *item, int dx, int dy)
{
    BbGraphicBox *box = BB_GRAPHIC_BOX(item);
    g_return_if_fail(box != NULL);

    bb_coord_translate(dx, dy, box->x, box->y, 2);

    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X0]);
    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y0]);
    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_X1]);
    g_object_notify_by_pspec(G_OBJECT(box), properties[PROP_Y1]);
}


static void
bb_graphic_box_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGraphicBox *box = BB_GRAPHIC_BOX(item);

    bb_item_params_write_async(
        box->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static void
bb_graphic_box_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    )
{
    g_output_stream_write_all_finish(
        stream,
        result,
        NULL,
        error
        );
}
