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
#include "bbitemparams.h"
#include "bbpathcommand.h"


enum
{
    PROP_0,
    PROP_WIDTH,
    N_PROPERTIES
};


struct _BbGraphicPath
{
    BbSchematicItem parent;

    BbItemParams *params;

    int width;

    GSList *commands;
};


G_DEFINE_TYPE(BbGraphicPath, bb_graphic_path, BB_TYPE_SCHEMATIC_ITEM)


typedef struct _MirrorXCapture MirrorXCapture;

struct _MirrorXCapture
{
    int cx;
};


typedef struct _MirrorYCapture MirrorYCapture;

struct _MirrorYCapture
{
    int cy;
};


typedef struct _RotateCapture RotateCapture;

struct _RotateCapture
{
    int cx;
    int cy;
    int angle;
};


typedef struct _TranslateCapture TranslateCapture;

struct _TranslateCapture
{
    int dx;
    int dy;
};


static BbBounds*
bb_graphic_path_calculate_bounds(BbSchematicItem *item, BbBoundsCalculator *calculator);

static void
bb_graphic_path_dispose(GObject *object);

static void
bb_graphic_path_finalize(GObject *object);

static void
bb_graphic_path_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphic_path_mirror_x(BbSchematicItem *item, int cx);

static void
bb_graphic_path_mirror_x_lambda(BbPathCommand *command, MirrorXCapture *capture);

static void
bb_graphic_path_mirror_y(BbSchematicItem *item, int cy);

static void
bb_graphic_path_mirror_y_lambda(BbPathCommand *command, MirrorYCapture *capture);

static void
bb_graphic_path_render(BbSchematicItem *item, BbItemRenderer *renderer);

static void
bb_graphic_path_rotate(BbSchematicItem *item, int cx, int cy, int angle);

static void
bb_graphic_path_rotate_lambda(BbPathCommand *command, RotateCapture *capture);

static void
bb_graphic_path_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_graphic_path_translate(BbSchematicItem *item, int dx, int dy);

static void
bb_graphic_path_translate_lambda(BbPathCommand *command, TranslateCapture *capture);

static void
bb_graphic_path_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_graphic_path_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


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
    BB_SCHEMATIC_ITEM_CLASS(klasse)->mirror_x = bb_graphic_path_mirror_x;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->mirror_y = bb_graphic_path_mirror_y;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->render = bb_graphic_path_render;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->rotate = bb_graphic_path_rotate;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->translate = bb_graphic_path_translate;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_async = bb_graphic_path_write_async;
    BB_SCHEMATIC_ITEM_CLASS(klasse)->write_finish = bb_graphic_path_write_finish;

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


static void
bb_graphic_path_mirror_x(BbSchematicItem *item, int cx)
{
    BbGraphicPath *path = BB_GRAPHIC_PATH(item);
    g_return_if_fail(path != NULL);

    MirrorXCapture capture;

    capture.cx = cx;

    g_slist_foreach(
        path->commands,
        bb_graphic_path_mirror_x_lambda,
        &capture
        );
}


static void
bb_graphic_path_mirror_x_lambda(BbPathCommand *command, MirrorXCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_mirror_x(command, capture->cx);
}


static void
bb_graphic_path_mirror_y(BbSchematicItem *item, int cy)
{
    BbGraphicPath *path = BB_GRAPHIC_PATH(item);
    g_return_if_fail(path != NULL);

    MirrorYCapture capture;

    capture.cy = cy;

    g_slist_foreach(
        path->commands,
        bb_graphic_path_mirror_y_lambda,
        &capture
        );
}


static void
bb_graphic_path_mirror_y_lambda(BbPathCommand *command, MirrorYCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_mirror_y(command, capture->cy);
}


static void
bb_graphic_path_rotate(BbSchematicItem *item, int cx, int cy, int angle)
{
    BbGraphicPath *path = BB_GRAPHIC_PATH(item);
    g_return_if_fail(path != NULL);

    RotateCapture capture;

    capture.cx = cx;
    capture.cy = cy;
    capture.angle = angle;

    g_slist_foreach(
        path->commands,
        bb_graphic_path_rotate_lambda,
        &capture
        );
}


static void
bb_graphic_path_rotate_lambda(BbPathCommand *command, RotateCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_rotate(command, capture->cx, capture->cy, capture->angle);
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
bb_graphic_path_translate(BbSchematicItem *item, int dx, int dy)
{
    BbGraphicPath *path = BB_GRAPHIC_PATH(item);
    g_return_if_fail(path != NULL);

    TranslateCapture capture;

    capture.dx = dx;
    capture.dy = dy;

    g_slist_foreach(
        path->commands,
        bb_graphic_path_translate_lambda,
        &capture
        );
}


static void
bb_graphic_path_translate_lambda(BbPathCommand *command, TranslateCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_translate(command, capture->dx, capture->dy);
}


static void
bb_graphic_path_write_async(
    BbSchematicItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGraphicPath *path = BB_GRAPHIC_PATH(item);

    bb_item_params_write_async(
        path->params,
        stream,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_graphic_path_write_finish(
    BbSchematicItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    )
{
    return g_output_stream_write_all_finish(
        stream,
        result,
        NULL,
        error
        );
}
