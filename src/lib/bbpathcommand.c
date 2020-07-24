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
#include "bbpathcommand.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


G_DEFINE_TYPE(BbPathCommand, bb_path_command, G_TYPE_OBJECT)


static BbPathCommand*
bb_path_command_clone_missing(const BbPathCommand *command);

static void
bb_path_command_dispose(GObject *object);

static void
bb_path_command_finalize(GObject *object);

static void
bb_path_command_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_path_command_mirror_x_missing(BbPathCommand *command, int cx);

static void
bb_path_command_mirror_y_missing(BbPathCommand *command, int cy);

static GSList*
bb_path_command_output_missing(BbPathCommand *command, GSList *collector);

static void
bb_path_command_rotate_missing(BbPathCommand *command, int cx, int cy, int angle);

static void
bb_path_command_render_missing(BbPathCommand *command, BbItemRenderer *renderer);

static void
bb_path_command_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_path_command_translate_missing(BbPathCommand *command, int dx, int dy);


GParamSpec *properties[N_PROPERTIES];


static void
bb_path_command_class_init(BbPathCommandClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_path_command_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_path_command_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_path_command_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_path_command_set_property;

    klasse->clone = bb_path_command_clone_missing;
    klasse->mirror_x = bb_path_command_mirror_x_missing;
    klasse->mirror_y = bb_path_command_mirror_y_missing;
    klasse->output = bb_path_command_output_missing;
    klasse->render = bb_path_command_render_missing;
    klasse->rotate = bb_path_command_rotate_missing;
    klasse->translate = bb_path_command_translate_missing;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/bbpathcommand.ui"
        );
}


BbPathCommand*
bb_path_command_clone(const BbPathCommand *command)
{
    BbPathCommandClass *class = BB_PATH_COMMAND_GET_CLASS(command);

    g_return_val_if_fail(class != NULL, NULL);
    g_return_val_if_fail(class->clone != NULL, NULL);

    return class->clone(command);
}


static BbPathCommand*
bb_path_command_clone_missing(const BbPathCommand *command)
{
    g_error("bb_path_command_clone() not overridden");
}


static void
bb_path_command_dispose(GObject *object)
{
}


static void
bb_path_command_finalize(GObject *object)
{
}


static void
bb_path_command_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
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
bb_path_command_init(BbPathCommand *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


void
bb_path_command_mirror_x(BbPathCommand *command, int cx)
{
    BbPathCommandClass *class = BB_PATH_COMMAND_GET_CLASS(command);

    g_return_if_fail(class != NULL);
    g_return_if_fail(class->mirror_x != NULL);

    class->mirror_x(command, cx);
}


static void
bb_path_command_mirror_x_missing(BbPathCommand *command, int cx)
{
    g_error("bb_path_command_mirror_x() not overridden");
}


void
bb_path_command_mirror_y(BbPathCommand *command, int cy)
{
    BbPathCommandClass *class = BB_PATH_COMMAND_GET_CLASS(command);

    g_return_if_fail(class != NULL);
    g_return_if_fail(class->mirror_y != NULL);

    class->mirror_y(command, cy);
}


void
bb_path_command_mirror_y_missing(BbPathCommand *command, int cy)
{
    g_error("bb_path_command_mirror_y() not overridden");
}


GSList*
bb_path_command_output(BbPathCommand *command, GSList *collector)
{
    BbPathCommandClass *class = BB_PATH_COMMAND_GET_CLASS(command);

    g_return_val_if_fail(class != NULL, collector);
    g_return_val_if_fail(class->mirror_y != NULL, collector);

    return class->output(command, collector);
}


static GSList*
bb_path_command_output_missing(BbPathCommand *command, GSList *collector)
{
    g_error("bb_path_command_output() not overridden");
}


__attribute__((constructor)) void
bb_path_command_register()
{
    bb_path_command_get_type();
}


void
bb_path_command_render(BbPathCommand *command, BbItemRenderer *renderer)
{
    BbPathCommandClass *class = BB_PATH_COMMAND_GET_CLASS(command);

    g_return_if_fail(class != NULL);
    g_return_if_fail(class->render != NULL);

    class->render(command, renderer);
}


static void
bb_path_command_render_missing(BbPathCommand *command, BbItemRenderer *renderer)
{
    g_error("bb_path_command_render() not overridden");
}


void
bb_path_command_rotate(BbPathCommand *command, int cx, int cy, int angle)
{
    BbPathCommandClass *class = BB_PATH_COMMAND_GET_CLASS(command);

    g_return_if_fail(class != NULL);
    g_return_if_fail(class->rotate != NULL);

    class->rotate(command, cx, cy, angle);
}


static void
bb_path_command_rotate_missing(BbPathCommand *command, int cx, int cy, int angle)
{
    g_error("bb_path_command_rotate() not overridden");
}


static void
bb_path_command_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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


void
bb_path_command_translate(BbPathCommand *command, int dx, int dy)
{
    BbPathCommandClass *class = BB_PATH_COMMAND_GET_CLASS(command);

    g_return_if_fail(class != NULL);
    g_return_if_fail(class->translate != NULL);

    class->translate(command, dx, dy);
}


static void
bb_path_command_translate_missing(BbPathCommand *command, int dx, int dy)
{
    g_error("bb_path_command_translate() not overridden");
}
