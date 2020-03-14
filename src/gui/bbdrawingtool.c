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
#include "bbdrawingtool.h"


enum
{
    PROP_0,
    PROP_WINDOW,
    N_PROPERTIES
};

typedef struct _BbDrawingToolPrivate BbDrawingToolPrivate;

struct _BbDrawingToolPrivate
{
    BbSchematicWindow *window;
};


G_DEFINE_TYPE_WITH_PRIVATE(BbDrawingTool, bb_drawing_tool, G_TYPE_OBJECT)


static void
bb_drawing_tool_dispose(GObject *object);

static void
bb_drawing_tool_finalize(GObject *object);

static void
bb_drawing_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_drawing_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


static void
bb_drawing_tool_class_init(BbDrawingToolClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_drawing_tool_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_drawing_tool_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_drawing_tool_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_drawing_tool_set_property;

    properties[PROP_WINDOW] = g_param_spec_object(
        "window",
        "",
        "",
        BB_TYPE_DOCUMENT_WINDOW,
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
bb_drawing_tool_dispose(GObject *object)
{
    BbDrawingToolPrivate* privat = bb_drawing_tool_get_instance_private(object);
    g_return_if_fail(privat != NULL);

    g_clear_object(&privat->window);
}


static void
bb_drawing_tool_finalize(GObject *object)
{
    // BbItemBuildTool* privat = BBDRAWING_TOOL_GET_PRIVATE(object);
}


static void
bb_drawing_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WINDOW:
            g_value_set_object(value, bb_drawing_tool_get_window(BB_DRAWING_TOOL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


BbSchematicWindow*
bb_drawing_tool_get_window(BbDrawingTool *tool)
{
    BbDrawingToolPrivate* privat = bb_drawing_tool_get_instance_private(tool);
    g_return_val_if_fail(privat != NULL, NULL);

    return privat->window;
}


static void
bb_drawing_tool_init(BbDrawingTool *tool)
{
}


__attribute__((constructor)) void
bb_drawing_tool_register()
{
    bb_drawing_tool_get_type();
}


static void
bb_drawing_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_WINDOW:
            bb_drawing_tool_set_window(BB_DRAWING_TOOL(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_drawing_tool_set_window(BbDrawingTool *tool, BbSchematicWindow *window)
{
    BbDrawingToolPrivate* privat = bb_drawing_tool_get_instance_private(tool);
    g_return_if_fail(privat != NULL);

    if (privat->window != window)
    {
        g_set_object(&privat->window, window);

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_WINDOW]);
    }
}
