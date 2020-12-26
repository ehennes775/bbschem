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
#include "bbclosepath.h"


enum
{
    PROP_0,
    N_PROPERTIES
};


struct _BbClosePath
{
    BbPathCommand parent;
};


static BbPathCommand*
bb_close_path_clone(BbPathCommand *command);

static void
bb_close_path_dispose(GObject *object);

static void
bb_close_path_finalize(GObject *object);

static void
bb_close_path_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_close_path_mirror_x(BbPathCommand *command, int cx);

static void
bb_close_path_mirror_y(BbPathCommand *command, int cy);

static void
bb_close_path_render(BbPathCommand *command, BbItemRenderer *renderer);

static void
bb_close_path_rotate(BbPathCommand *command, int cx, int cy, int angle);

static void
bb_close_path_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_close_path_translate(BbPathCommand *command, int dx, int dy);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE(BbClosePath, bb_close_path, BB_TYPE_PATH_COMMAND)


static void
bb_close_path_class_init(BbClosePathClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(G_IS_OBJECT_CLASS(object_class));

    object_class->dispose = bb_close_path_dispose;
    object_class->finalize = bb_close_path_finalize;
    object_class->get_property = bb_close_path_get_property;
    object_class->set_property = bb_close_path_set_property;

    BbPathCommandClass *command_class = BB_PATH_COMMAND_CLASS(klasse);
    g_return_if_fail(BB_IS_PATH_COMMAND_CLASS(command_class));

    command_class->clone = bb_close_path_clone;
    command_class->mirror_x = bb_close_path_mirror_x;
    command_class->mirror_y = bb_close_path_mirror_y;
    command_class->render = bb_close_path_render;
    command_class->rotate = bb_close_path_rotate;
    command_class->translate = bb_close_path_translate;
}


static BbPathCommand*
bb_close_path_clone(BbPathCommand *command)
{
    g_warn_if_fail(BB_IS_CLOSE_PATH(command));

    return BB_PATH_COMMAND(bb_close_path_new());
}


static void
bb_close_path_dispose(GObject *object)
{
}


static void
bb_close_path_finalize(GObject *object)
{
}


static void
bb_close_path_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_close_path_init(BbClosePath *command)
{
    g_return_if_fail(BB_IS_CLOSE_PATH(command));
}


static void
bb_close_path_mirror_x(BbPathCommand *command, int cx)
{
    g_return_if_fail(BB_IS_CLOSE_PATH(command));
}


static void
bb_close_path_mirror_y(BbPathCommand *command, int cy)
{
    g_return_if_fail(BB_IS_CLOSE_PATH(command));
}


BbClosePath*
bb_close_path_new()
{
    return BB_CLOSE_PATH(g_object_new(
        BB_TYPE_CLOSE_PATH,
        NULL
        ));
}


static void
bb_close_path_render(BbPathCommand *command, BbItemRenderer *renderer)
{
    g_return_if_fail(BB_IS_CLOSE_PATH(command));

    bb_item_renderer_close_path(renderer);
}


static void
bb_close_path_rotate(BbPathCommand *command, int cx, int cy, int angle)
{
    g_return_if_fail(BB_IS_CLOSE_PATH(command));
}


static void
bb_close_path_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_close_path_translate(BbPathCommand *command, int dx, int dy)
{
    g_return_if_fail(BB_IS_CLOSE_PATH(command));
}
