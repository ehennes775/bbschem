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
#include "bbgedapath.h"
#include "bbitemparams.h"
#include "bbpathcommand.h"


enum
{
    PROP_0,

    PROP_ITEM_COLOR,

    PROP_CAP_TYPE,
    PROP_DASH_LENGTH,
    PROP_DASH_SPACE,
    PROP_DASH_TYPE,
    PROP_LINE_WIDTH,

    PROP_FILL_TYPE,
    PROP_FILL_WIDTH,
    PROP_ANGLE_1,
    PROP_ANGLE_2,
    PROP_PITCH_1,
    PROP_PITCH_2,

    N_PROPERTIES
};


struct _BbGraphicPath
{
    BbGedaItem parent;

    BbItemParams *params;

    GSList *commands;

    int color;

    BbFillStyle *fill_style;
    BbLineStyle *line_style;
};


G_DEFINE_TYPE(BbGraphicPath, bb_geda_path, BB_TYPE_GEDA_ITEM)


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
bb_geda_path_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator);

static void
bb_geda_path_dispose(GObject *object);

static void
bb_geda_path_finalize(GObject *object);

static int
bb_geda_path_get_cap_type(BbGraphicPath *path);

static int
bb_geda_path_get_dash_length(BbGraphicPath *path);

static int
bb_geda_path_get_dash_space(BbGraphicPath *path);

static int
bb_geda_path_get_dash_type(BbGraphicPath *path);

static int
bb_geda_path_get_item_color(BbGraphicPath *path);

static int
bb_geda_path_get_line_width(BbGraphicPath *path);

static void
bb_geda_path_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_path_mirror_x(BbGedaItem *item, int cx);

static void
bb_geda_path_mirror_x_lambda(BbPathCommand *command, MirrorXCapture *capture);

static void
bb_geda_path_mirror_y(BbGedaItem *item, int cy);

static void
bb_geda_path_mirror_y_lambda(BbPathCommand *command, MirrorYCapture *capture);

static void
bb_geda_path_render(BbGedaItem *item, BbItemRenderer *renderer);

static void
bb_geda_path_render_lambda(gpointer command, gpointer renderer);

static void
bb_geda_path_rotate(BbGedaItem *item, int cx, int cy, int angle);

static void
bb_geda_path_rotate_lambda(BbPathCommand *command, RotateCapture *capture);

static void
bb_geda_path_set_cap_type(BbGraphicPath *path, int type);

static void
bb_geda_path_set_dash_length(BbGraphicPath *path, int length);

static void
bb_geda_path_set_dash_space(BbGraphicPath *path, int space);

static void
bb_geda_path_set_dash_type(BbGraphicPath *path, int type);

static void
bb_geda_path_set_item_color(BbGraphicPath *path, int color);

static void
bb_geda_path_set_line_width(BbGraphicPath *path, int width);

static void
bb_geda_path_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_geda_path_translate(BbGedaItem *item, int dx, int dy);

static void
bb_geda_path_translate_lambda(BbPathCommand *command, TranslateCapture *capture);

static void
bb_geda_path_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_path_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];


static BbBounds*
bb_geda_path_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGraphicPath *path = BB_GEDA_PATH(item);

    g_return_val_if_fail(path != NULL, NULL);

    return NULL;
}


static void
bb_geda_path_class_init(BbGraphicPathClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_geda_path_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_geda_path_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_geda_path_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_geda_path_set_property;

    BB_GEDA_ITEM_CLASS(klasse)->calculate_bounds = bb_geda_path_calculate_bounds;
    BB_GEDA_ITEM_CLASS(klasse)->mirror_x = bb_geda_path_mirror_x;
    BB_GEDA_ITEM_CLASS(klasse)->mirror_y = bb_geda_path_mirror_y;
    BB_GEDA_ITEM_CLASS(klasse)->render = bb_geda_path_render;
    BB_GEDA_ITEM_CLASS(klasse)->rotate = bb_geda_path_rotate;
    BB_GEDA_ITEM_CLASS(klasse)->translate = bb_geda_path_translate;
    BB_GEDA_ITEM_CLASS(klasse)->write_async = bb_geda_path_write_async;
    BB_GEDA_ITEM_CLASS(klasse)->write_finish = bb_geda_path_write_finish;

    properties[PROP_CAP_TYPE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAP_TYPE,
        "cap-type"
        );

    properties[PROP_DASH_LENGTH] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_DASH_LENGTH,
        "dash-length"
        );

    properties[PROP_DASH_SPACE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_DASH_SPACE,
        "dash-space"
        );

    properties[PROP_DASH_TYPE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_DASH_TYPE,
        "dash-type"
        );

    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_ITEM_COLOR,
        "item-color"
        );

    properties[PROP_LINE_WIDTH] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_LINE_WIDTH,
        "line-width"
        );
}


static void
bb_geda_path_dispose(GObject *object)
{
    BbGraphicPath *path = BB_GEDA_PATH(object);

    g_return_if_fail(path != NULL);

    g_slist_free_full(
        g_steal_pointer(&path->commands),
        g_object_unref
        );
}


static void
bb_geda_path_finalize(GObject *object)
{
    BbGraphicPath *path = BB_GEDA_PATH(object);

    g_return_if_fail(path != NULL);

    bb_fill_style_free(path->fill_style);
    bb_line_style_free(path->line_style);
}


static int
bb_geda_path_get_cap_type(BbGraphicPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->line_style != NULL, 0);

    return path->line_style->cap_type;
}


static int
bb_geda_path_get_dash_length(BbGraphicPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->line_style != NULL, 0);

    return path->line_style->dash_length;
}


static int
bb_geda_path_get_dash_space(BbGraphicPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->line_style != NULL, 0);

    return path->line_style->dash_space;
}


static int
bb_geda_path_get_dash_type(BbGraphicPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->line_style != NULL, 0);

    return path->line_style->dash_type;
}


static int
bb_geda_path_get_item_color(BbGraphicPath *path)
{
    g_return_val_if_fail(path != NULL, 0);

    return path->color;
}


static int
bb_geda_path_get_line_width(BbGraphicPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->line_style != NULL, 0);

    return path->line_style->line_width;
}


static void
bb_geda_path_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAP_TYPE:
            g_value_set_int(value, bb_geda_path_get_cap_type(BB_GEDA_PATH(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_geda_path_get_item_color(BB_GEDA_PATH(object)));
            break;

        case PROP_DASH_LENGTH:
            g_value_set_int(value, bb_geda_path_get_dash_length(BB_GEDA_PATH(object)));
            break;

        case PROP_DASH_SPACE:
            g_value_set_int(value, bb_geda_path_get_dash_space(BB_GEDA_PATH(object)));
            break;

        case PROP_DASH_TYPE:
            g_value_set_int(value, bb_geda_path_get_dash_type(BB_GEDA_PATH(object)));
            break;

        case PROP_LINE_WIDTH:
            g_value_set_int(value, bb_geda_path_get_line_width(BB_GEDA_PATH(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_path_init(BbGraphicPath *path)
{
    g_return_if_fail(path != NULL);

    path->fill_style = bb_fill_style_new();
    path->line_style = bb_line_style_new();
}


static void
bb_geda_path_mirror_x(BbGedaItem *item, int cx)
{
    BbGraphicPath *path = BB_GEDA_PATH(item);
    g_return_if_fail(path != NULL);

    MirrorXCapture capture;

    capture.cx = cx;

    g_slist_foreach(
        path->commands,
        bb_geda_path_mirror_x_lambda,
        &capture
        );
}


static void
bb_geda_path_mirror_x_lambda(BbPathCommand *command, MirrorXCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_mirror_x(command, capture->cx);
}


static void
bb_geda_path_mirror_y(BbGedaItem *item, int cy)
{
    BbGraphicPath *path = BB_GEDA_PATH(item);
    g_return_if_fail(path != NULL);

    MirrorYCapture capture;

    capture.cy = cy;

    g_slist_foreach(
        path->commands,
        bb_geda_path_mirror_y_lambda,
        &capture
        );
}


static void
bb_geda_path_mirror_y_lambda(BbPathCommand *command, MirrorYCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_mirror_y(command, capture->cy);
}


static void
bb_geda_path_rotate(BbGedaItem *item, int cx, int cy, int angle)
{
    BbGraphicPath *path = BB_GEDA_PATH(item);
    g_return_if_fail(path != NULL);

    RotateCapture capture;

    capture.cx = cx;
    capture.cy = cy;
    capture.angle = angle;

    g_slist_foreach(
        path->commands,
        bb_geda_path_rotate_lambda,
        &capture
        );
}


static void
bb_geda_path_rotate_lambda(BbPathCommand *command, RotateCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_rotate(command, capture->cx, capture->cy, capture->angle);
}


__attribute__((constructor)) void
bb_geda_path_register()
{
    bb_geda_path_get_type();
}


static void
bb_geda_path_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGraphicPath *path = BB_GEDA_PATH(item);

    g_return_if_fail(path != NULL);

    bb_item_renderer_set_color(renderer, path->color);
    bb_item_renderer_set_fill_style(renderer, path->fill_style);
    bb_item_renderer_set_line_style(renderer, path->line_style);

    g_slist_foreach(
        path->commands,
        bb_geda_path_render_lambda,
        renderer
        );
}


static void
bb_geda_path_render_lambda(gpointer command, gpointer renderer)
{
    bb_geda_path_render(
        BB_PATH_COMMAND(command),
        BB_ITEM_RENDERER(renderer)
        );
}


static void
bb_geda_path_set_cap_type(BbGraphicPath *path, int type)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->cap_type != type)
    {
        path->line_style->cap_type = type;

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_CAP_TYPE]);
    }
}


static void
bb_geda_path_set_dash_length(BbGraphicPath *path, int length)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->dash_length != length)
    {
        path->line_style->dash_length = length;

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_DASH_LENGTH]);
    }
}


static void
bb_geda_path_set_dash_space(BbGraphicPath *path, int space)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->dash_space != space)
    {
        path->line_style->dash_space = space;

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_DASH_SPACE]);
    }
}


static void
bb_geda_path_set_dash_type(BbGraphicPath *path, int type)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->dash_type != type)
    {
        path->line_style->dash_type = type;

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_DASH_TYPE]);
    }
}


static void
bb_geda_path_set_item_color(BbGraphicPath *path, int color)
{
    g_return_if_fail(path != NULL);

    if (path->color != color)
    {
        path->color = color;

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_geda_path_set_line_width(BbGraphicPath *path, int width)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->line_width != width)
    {
        path->line_style->line_width = width;

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_LINE_WIDTH]);
    }
}


static void
bb_geda_path_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAP_TYPE:
            bb_geda_path_set_cap_type(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_geda_path_set_item_color(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_DASH_LENGTH:
            bb_geda_path_set_dash_length(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_DASH_SPACE:
            bb_geda_path_set_dash_space(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_DASH_TYPE:
            bb_geda_path_set_dash_type(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_LINE_WIDTH:
            bb_geda_path_set_line_width(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_path_translate(BbGedaItem *item, int dx, int dy)
{
    BbGraphicPath *path = BB_GEDA_PATH(item);
    g_return_if_fail(path != NULL);

    TranslateCapture capture;

    capture.dx = dx;
    capture.dy = dy;

    g_slist_foreach(
        path->commands,
        bb_geda_path_translate_lambda,
        &capture
        );
}


static void
bb_geda_path_translate_lambda(BbPathCommand *command, TranslateCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_translate(command, capture->dx, capture->dy);
}


static void
bb_geda_path_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGraphicPath *path = BB_GEDA_PATH(item);

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
bb_geda_path_write_finish(
    BbGedaItem *item,
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