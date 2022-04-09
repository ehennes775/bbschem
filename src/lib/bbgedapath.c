/*
 * bbschem
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
#include "bbcolor.h"
#include "bberror.h"
#include "bbcolors.h"
#include "bbadjustableitemcolor.h"
#include "bbadjustablefillstyle.h"
#include "bbadjustablelinestyle.h"


/**
 * The positions of parameters in the file format
 */
enum
{
    PARAM_TOKEN,
    PARAM_COLOR,
    PARAM_LINE_WIDTH,
    PARAM_CAP_TYPE,
    PARAM_DASH_TYPE,
    PARAM_DASH_LENGTH,
    PARAM_DASH_SPACE,
    PARAM_FILL_TYPE,
    PARAM_FILL_WIDTH,
    PARAM_FILL_ANGLE_1,
    PARAM_FILL_PITCH_1,
    PARAM_FILL_ANGLE_2,
    PARAM_FILL_PITCH_2,
    PARAM_LINE_COUNT,
    N_PARAMETERS
};


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

    PROP_COMMANDS,

    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbGedaPath
{
    BbGedaItem parent;

    BbItemParams *params;

    GSList *commands;

    int color;

    BbFillStyle *fill_style;
    BbLineStyle *line_style;
};


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


static void
bb_geda_path_closed_shape_drawer_init(BbClosedShapeDrawerInterface *iface);

static void
bb_geda_path_draw_outline_lambda(gpointer command, gpointer renderer);

static void
bb_geda_path_adjustable_fill_style_init(BbAdjustableFillStyleInterface *iface);


static void
bb_geda_path_adjustable_item_color_init(BbAdjustableItemColorInterface *iface);


static void
bb_geda_path_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface);

static BbBounds*
bb_geda_path_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator);

static void
bb_geda_path_dispose(GObject *object);

static void
bb_geda_path_finalize(GObject *object);

static int
bb_geda_path_get_cap_type(BbGedaPath *path);

static int
bb_geda_path_get_dash_length(BbGedaPath *path);

static int
bb_geda_path_get_dash_space(BbGedaPath *path);

static int
bb_geda_path_get_dash_type(BbGedaPath *path);

static int
bb_geda_path_get_item_color(BbGedaPath *path);

static int
bb_geda_path_get_line_width(BbGedaPath *path);

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
bb_geda_path_rotate(BbGedaItem *item, int cx, int cy, int angle);

static void
bb_geda_path_rotate_lambda(BbPathCommand *command, RotateCapture *capture);

static void
bb_geda_path_set_cap_type(BbGedaPath *path, int type);

static void
bb_geda_path_set_dash_length(BbGedaPath *path, int length);

static void
bb_geda_path_set_dash_space(BbGedaPath *path, int space);

static void
bb_geda_path_set_dash_type(BbGedaPath *path, int type);

static void
bb_geda_path_set_item_color(BbGedaPath *path, int color);

static void
bb_geda_path_set_line_width(BbGedaPath *path, int width);

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
static guint signals[N_SIGNALS];

G_DEFINE_TYPE_WITH_CODE(
    BbGedaPath,
    bb_geda_path,
    BB_TYPE_GEDA_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_FILL_STYLE, bb_geda_path_adjustable_fill_style_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_geda_path_adjustable_item_color_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_LINE_STYLE, bb_geda_path_adjustable_line_style_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_CLOSED_SHAPE_DRAWER, bb_geda_path_closed_shape_drawer_init)
    )

// region From AdjustableFillStyle Interface

static int
bb_geda_path_get_angle_1(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->fill_style != NULL, 0);

    return path->fill_style->angle[0];
}


void
bb_geda_path_set_angle_1(BbGedaPath *path, int angle)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->fill_style != NULL);

    if (path->fill_style->angle[0] != angle)
    {
        path->fill_style->angle[0] = angle;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_ANGLE_1]);
    }
}


static int
bb_geda_path_get_angle_2(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->fill_style != NULL, 0);

    return path->fill_style->angle[1];
}


void
bb_geda_path_set_angle_2(BbGedaPath *path, int angle)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->fill_style != NULL);

    if (path->fill_style->angle[1] != angle)
    {
        path->fill_style->angle[1] = angle;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_ANGLE_2]);
    }
}


static int
bb_geda_path_get_fill_type(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, BB_FILL_TYPE_DEFAULT);
    g_return_val_if_fail(path->fill_style != NULL, BB_FILL_TYPE_DEFAULT);

    return path->fill_style->type;
}


void
bb_geda_path_set_fill_type(BbGedaPath *path, int fill_type)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->fill_style != NULL);
    g_return_if_fail(bb_fill_type_is_valid(fill_type));

    if (path->fill_style->type != fill_type)
    {
        path->fill_style->type = fill_type;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_FILL_TYPE]);
    }
}


static int
bb_geda_path_get_fill_width(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->fill_style != NULL, 0);

    return path->fill_style->width;
}


void
bb_geda_path_set_fill_width(BbGedaPath *path, int width)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->fill_style != NULL);

    if (path->fill_style->width != width)
    {
        path->fill_style->width = width;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_FILL_WIDTH]);
    }
}


static int
bb_geda_path_get_pitch_1(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->fill_style != NULL, 0);

    return path->fill_style->pitch[0];
}


void
bb_geda_path_set_pitch_1(BbGedaPath *path, int pitch)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->fill_style != NULL);

    if (path->fill_style->pitch[0] != pitch)
    {
        path->fill_style->pitch[0] = pitch;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_PITCH_1]);
    }
}


static int
bb_geda_path_get_pitch_2(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->fill_style != NULL, 0);

    return path->fill_style->pitch[1];
}


void
bb_geda_path_set_pitch_2(BbGedaPath *path, int pitch)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->fill_style != NULL);

    if (path->fill_style->pitch[1] != pitch)
    {
        path->fill_style->pitch[1] = pitch;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_PITCH_2]);
    }
}


static void
bb_geda_path_adjustable_fill_style_init(BbAdjustableFillStyleInterface *iface)
{
    g_return_if_fail(iface != NULL);
}

// endregion


// region From BbClosedShapeDrawer Interface

static void
bb_geda_path_draw_hatch(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer)
{
    BbGedaPath *path = BB_GEDA_PATH(drawer);
    g_return_if_fail(BB_IS_GEDA_PATH(path));
    g_return_if_fail(BB_IS_ITEM_RENDERER(renderer));
}


static void
bb_geda_path_draw_outline(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer)
{
    BbGedaPath *path = BB_GEDA_PATH(drawer);
    g_return_if_fail(BB_IS_GEDA_PATH(path));
    g_return_if_fail(BB_IS_ITEM_RENDERER(renderer));

    g_slist_foreach(
        path->commands,
        bb_geda_path_draw_outline_lambda,
        renderer
        );
}


static void
bb_geda_path_draw_outline_lambda(gpointer command, gpointer renderer)
{
    bb_path_command_render(
        BB_PATH_COMMAND(command),
        BB_ITEM_RENDERER(renderer)
        );
}


static void
bb_geda_path_closed_shape_drawer_init(BbClosedShapeDrawerInterface *iface)
{
    iface->draw_hatch = bb_geda_path_draw_hatch;
    iface->draw_outline = bb_geda_path_draw_outline;
}

// endregion

static void
bb_geda_path_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
}


static void
bb_geda_path_adjustable_line_style_init(BbAdjustableLineStyleInterface *iface)
{
}


static BbBounds*
bb_geda_path_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaPath *path = BB_GEDA_PATH(item);

    g_return_val_if_fail(path != NULL, NULL);

    return bb_bounds_new();
}


static void
bb_geda_path_class_init(BbGedaPathClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

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

    /* From BbAdjustableItemColor */

    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        object_class,
        PROP_ITEM_COLOR,
        "item-color"
        );

    /* From BbAdjustableFillStyle */

    properties[PROP_ANGLE_1] = bb_object_class_override_property(
        object_class,
        PROP_ANGLE_1,
        "angle-1"
        );

    properties[PROP_ANGLE_2] = bb_object_class_override_property(
        object_class,
        PROP_ANGLE_2,
        "angle-2"
        );

    properties[PROP_PITCH_1] = bb_object_class_override_property(
        object_class,
        PROP_PITCH_1,
        "pitch-1"
        );

    properties[PROP_PITCH_2] = bb_object_class_override_property(
        object_class,
        PROP_PITCH_2,
        "pitch-2"
        );

    properties[PROP_FILL_TYPE] = bb_object_class_override_property(
        object_class,
        PROP_FILL_TYPE,
        "fill-type"
        );

    properties[PROP_FILL_WIDTH] = bb_object_class_override_property(
        object_class,
        PROP_FILL_WIDTH,
        "fill-width"
        );


    /* From BbAdjustableLineStyle */

    properties[PROP_CAP_TYPE] = bb_object_class_override_property(
        object_class,
        PROP_CAP_TYPE,
        "cap-type"
        );

    properties[PROP_DASH_LENGTH] = bb_object_class_override_property(
        object_class,
        PROP_DASH_LENGTH,
        "dash-length"
        );

    properties[PROP_DASH_SPACE] = bb_object_class_override_property(
        object_class,
        PROP_DASH_SPACE,
        "dash-space"
        );

    properties[PROP_DASH_TYPE] = bb_object_class_override_property(
        object_class,
        PROP_DASH_TYPE,
        "dash-type"
        );

    properties[PROP_LINE_WIDTH] = bb_object_class_override_property(
        object_class,
        PROP_LINE_WIDTH,
        "line-width"
        );

    /* From BbGedaPath */

    properties[PROP_COMMANDS] = bb_object_class_install_property(
        object_class,
        PROP_COMMANDS,
        g_param_spec_pointer(
            "commands",
            "",
            "",
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_GEDA_ITEM);
}


static void
bb_geda_path_dispose(GObject *object)
{
    BbGedaPath *path = BB_GEDA_PATH(object);

    g_return_if_fail(path != NULL);

    g_slist_free_full(
        g_steal_pointer(&path->commands),
        g_object_unref
        );
}


static void
bb_geda_path_finalize(GObject *object)
{
    BbGedaPath *path = BB_GEDA_PATH(object);

    g_return_if_fail(path != NULL);

    bb_fill_style_free(path->fill_style);
    bb_line_style_free(path->line_style);
}


static int
bb_geda_path_get_cap_type(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->line_style != NULL, 0);

    return path->line_style->cap_type;
}


static int
bb_geda_path_get_dash_length(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->line_style != NULL, 0);

    return path->line_style->dash_length;
}


static int
bb_geda_path_get_dash_space(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->line_style != NULL, 0);

    return path->line_style->dash_space;
}


static int
bb_geda_path_get_dash_type(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);
    g_return_val_if_fail(path->line_style != NULL, 0);

    return path->line_style->dash_type;
}


static int
bb_geda_path_get_item_color(BbGedaPath *path)
{
    g_return_val_if_fail(path != NULL, 0);

    return path->color;
}


int
bb_geda_path_get_line_count(BbParams *params, GError **error)
{
    g_return_val_if_fail(params != NULL, 0);

    GError *local_error = NULL;

    int line_count = bb_params_get_int(params, PARAM_LINE_COUNT, &local_error);

    if (local_error == NULL)
    {
        if (line_count <= 0)
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_VALUE_OUT_OF_RANGE,
                "Line count of %d is out of range",
                line_count
            );
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        line_count = 0;
    }

    return line_count;
}


static int
bb_geda_path_get_line_width(BbGedaPath *path)
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
bb_geda_path_init(BbGedaPath *path)
{
    g_return_if_fail(path != NULL);

    path->fill_style = bb_fill_style_new();
    path->line_style = bb_line_style_new();
}


static void
bb_geda_path_mirror_x(BbGedaItem *item, int cx)
{
    BbGedaPath *path = BB_GEDA_PATH(item);
    g_return_if_fail(path != NULL);

    MirrorXCapture capture;

    capture.cx = cx;

    g_slist_foreach(
        path->commands,
        (GFunc) bb_geda_path_mirror_x_lambda,
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
    BbGedaPath *path = BB_GEDA_PATH(item);
    g_return_if_fail(path != NULL);

    MirrorYCapture capture;

    capture.cy = cy;

    g_slist_foreach(
        path->commands,
        (GFunc) bb_geda_path_mirror_y_lambda,
        &capture
        );
}


static void
bb_geda_path_mirror_y_lambda(BbPathCommand *command, MirrorYCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_mirror_y(command, capture->cy);
}


BbGedaPath*
bb_geda_path_new_with_params(BbParams *params, GSList *commands, GError **error)
{
    GError *local_error = NULL;

    g_return_val_if_fail(bb_params_token_matches(params, BB_GEDA_PATH_TOKEN), NULL);

    BbGedaPath *path = NULL;
    int color;
    BbFillStyle fill_style;
    BbLineStyle line_style;

    color = bb_text_color_from_params(params, PARAM_COLOR, BB_COLOR_GRAPHIC, &local_error);

    if (local_error == NULL)
    {
        bb_fill_style_from_params(params, PARAM_FILL_TYPE, &fill_style, &local_error);
    }

    if (local_error == NULL)
    {
        bb_line_style_from_params(params, PARAM_LINE_WIDTH, &line_style, &local_error);
    }

    if (local_error == NULL)
    {
        path = BB_GEDA_PATH(g_object_new(
            BB_TYPE_GEDA_PATH,

            /* From AdjustableColor */
            "item-color", color,

            /* From AdjustableFillStyle */
            "fill-type", fill_style.type,
            "fill-width", fill_style.width,
            "angle-1", fill_style.angle[0],
            "pitch-1", fill_style.pitch[0],
            "angle-2", fill_style.angle[1],
            "pitch-2", fill_style.pitch[1],

            /* From AdjustableLineStyle */
            "line-width", line_style.line_width,
            "cap-type", line_style.cap_type,
            "dash_type", line_style.dash_type,
            "dash-length", line_style.dash_length,
            "dash-space", line_style.dash_space,

            "commands", commands,

            NULL
        ));
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        g_clear_object(&path);
    }

    return path;
}


static void
bb_geda_path_rotate(BbGedaItem *item, int cx, int cy, int angle)
{
    BbGedaPath *path = BB_GEDA_PATH(item);
    g_return_if_fail(path != NULL);

    RotateCapture capture;

    capture.cx = cx;
    capture.cy = cy;
    capture.angle = angle;

    g_slist_foreach(
        path->commands,
        (GFunc) bb_geda_path_rotate_lambda,
        &capture
        );
}


static void
bb_geda_path_rotate_lambda(BbPathCommand *command, RotateCapture *capture)
{
    g_return_if_fail(capture != NULL);

    bb_path_command_rotate(command, capture->cx, capture->cy, capture->angle);
}


static void
bb_geda_path_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaPath *path = BB_GEDA_PATH(item);
    g_return_if_fail(BB_IS_GEDA_PATH(path));

    bb_item_renderer_draw_closed_shape(
        renderer,
        path->color,
        path->fill_style,
        path->line_style,
        BB_CLOSED_SHAPE_DRAWER(item)
        );
}


static void
bb_geda_path_set_cap_type(BbGedaPath *path, int type)
{
    g_return_if_fail(BB_IS_GEDA_PATH(path));
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->cap_type != type)
    {
        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        path->line_style->cap_type = type;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_CAP_TYPE]);
    }
}


static void
bb_geda_path_set_commands(BbGedaPath *path, GSList *commands)
{
    g_return_if_fail(BB_IS_GEDA_PATH(path));

    g_signal_emit(path, signals[SIG_INVALIDATE], 0);

    g_clear_slist(&path->commands, g_object_unref);

    path->commands = g_slist_copy_deep(commands, (GCopyFunc) g_object_ref, NULL);

    g_signal_emit(path, signals[SIG_INVALIDATE], 0);

    g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_COMMANDS]);
}



static void
bb_geda_path_set_dash_length(BbGedaPath *path, int length)
{
    g_return_if_fail(BB_IS_GEDA_PATH(path));
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->dash_length != length)
    {
        path->line_style->dash_length = length;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_DASH_LENGTH]);
    }
}


static void
bb_geda_path_set_dash_space(BbGedaPath *path, int space)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->dash_space != space)
    {
        path->line_style->dash_space = space;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_DASH_SPACE]);
    }
}


static void
bb_geda_path_set_dash_type(BbGedaPath *path, int type)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->dash_type != type)
    {
        path->line_style->dash_type = type;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_DASH_TYPE]);
    }
}


static void
bb_geda_path_set_item_color(BbGedaPath *path, int color)
{
    g_return_if_fail(BB_IS_GEDA_PATH(path));
    g_return_if_fail(bb_color_is_valid(color));

    if (path->color != color)
    {
        path->color = color;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(path), properties[PROP_ITEM_COLOR]);
    }
}


static void
bb_geda_path_set_line_width(BbGedaPath *path, int width)
{
    g_return_if_fail(path != NULL);
    g_return_if_fail(path->line_style != NULL);

    if (path->line_style->line_width != width)
    {
        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

        path->line_style->line_width = width;

        g_signal_emit(path, signals[SIG_INVALIDATE], 0);

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

        case PROP_COMMANDS:
            bb_geda_path_set_commands(BB_GEDA_PATH(object), g_value_get_pointer(value));
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

        case PROP_ANGLE_1:
            bb_geda_path_set_angle_1(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_ANGLE_2:
            bb_geda_path_set_angle_2(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_FILL_TYPE:
            bb_geda_path_set_fill_type(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_FILL_WIDTH:
            bb_geda_path_set_fill_width(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_PITCH_1:
            bb_geda_path_set_pitch_1(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        case PROP_PITCH_2:
            bb_geda_path_set_pitch_2(BB_GEDA_PATH(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_path_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaPath *path = BB_GEDA_PATH(item);
    g_return_if_fail(path != NULL);

    TranslateCapture capture;

    capture.dx = dx;
    capture.dy = dy;

    g_signal_emit(path, signals[SIG_INVALIDATE], 0);

    g_slist_foreach(
        path->commands,
        (GFunc) bb_geda_path_translate_lambda,
        &capture
        );

    g_signal_emit(path, signals[SIG_INVALIDATE], 0);
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
    BbGedaPath *path = BB_GEDA_PATH(item);

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
