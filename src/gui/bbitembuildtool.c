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

#include <math.h>
#include <gtk/gtk.h>
#include <bbextensions.h>
#include "bbitembuilder.h"
#include "bbitembuildtool.h"

enum
{
    PROP_0,
    PROP_BUILDER,
    N_PROPERTIES
};

enum State
{
    STATE_S0,
    STATE_S1
};

struct _BbItemBuildTool
{
    GObject parent;

    BbItemBuilder *builder;

    double x;
    double y;

    enum State state;
};

G_DEFINE_TYPE(BbItemBuildTool, bb_item_build_tool, BB_TYPE_DRAWING_TOOL);


static int
bb_item_build_tool_button_pressed(BbItemBuildTool *tool, GdkEventButton event);

static void
bb_item_build_tool_dispose(GObject *object);

static void
bb_item_build_tool_finalize(GObject *object);

static void
bb_item_build_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_item_build_tool_motion_notify(BbItemBuildTool *tool, GdkEventMotion event);

static void
bb_item_build_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static GParamSpec *properties[N_PROPERTIES];


static int
bb_item_build_tool_button_pressed(BbItemBuildTool *tool, GdkEventButton event)
{
    if (tool->state == STATE_S0)
    {
        tool->x = event.x;
        tool->y = event.y;

        double x = tool->x;
        double y = tool->y;

//        bb_schematic_window_device_to_user(
//            bb_drawing_tool_get_window(BB_DRAWING_TOOL(tool)),
//            &x,
//            &y
//            );

        int ix = (int) round(x);
        int iy = (int) round(y);

//        bb_schematic_window_snap_point(
//            bb_drawing_tool_get_window(BB_DRAWING_TOOL(tool)),
//            &ix,
//            &iy
//            );

        bb_item_builder_put_point(tool->builder, 0, ix, iy);
        bb_item_builder_put_point(tool->builder, 1, ix, iy);

        tool->state = STATE_S1;
    }
    else if (tool->state == STATE_S1)
    {
        tool->x = event.x;
        tool->y = event.y;

        double x = tool->x;
        double y = tool->y;

//        bb_schematic_window_device_to_user(
//            bb_drawing_tool_get_window(BB_DRAWING_TOOL(tool)),
//            &x,
//            &y
//            );

        int ix = (int) round(x);
        int iy = (int) round(y);

//        bb_schematic_window_snap_point(
//            bb_drawing_tool_get_window(BB_DRAWING_TOOL(tool)),
//            &ix,
//            &iy
//            );

        bb_item_builder_put_point(tool->builder, 1, ix, iy);

//        bb_schematic_window_add_items(
//            bb_drawing_tool_get_window(BB_DRAWING_TOOL(tool)),
//            bb_item_builder_create_items(tool->builder)
//            );

        // reset();
    }
    else
    {
        g_return_val_if_reached(FALSE);
    }

    return TRUE;
}

static void
bb_item_build_tool_class_init(BbItemBuildToolClass *class)
{
    G_OBJECT_CLASS(class)->dispose = bb_item_build_tool_dispose;
    G_OBJECT_CLASS(class)->finalize = bb_item_build_tool_finalize;
    G_OBJECT_CLASS(class)->get_property = bb_item_build_tool_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_item_build_tool_set_property;

    properties[PROP_BUILDER] = g_param_spec_object(
        "builder",
        "",
        "",
        BB_TYPE_ITEM_BUILDER,
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
bb_item_build_tool_dispose(GObject *object)
{
    BbItemBuildTool *tool = BB_ITEM_BUILD_TOOL(object);
    g_return_if_fail(tool != NULL);

    g_clear_object(&tool->builder);
}


static void
bb_item_build_tool_finalize(GObject *object)
{
}


BbItemBuilder*
bb_item_build_tool_get_builder(BbItemBuildTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->builder;
}


static void
bb_item_build_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_BUILDER:
            g_value_set_object(value, bb_item_build_tool_get_builder(BB_ITEM_BUILD_TOOL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_item_build_tool_init(BbItemBuildTool *tool)
{
}


static void
bb_item_build_tool_motion_notify(BbItemBuildTool *tool, GdkEventMotion event)
{
    if (tool->state == STATE_S1)
    {
        tool->x = event.x;
        tool->y = event.y;

        double x = tool->x;
        double y = tool->y;

//        bb_schematic_window_device_to_user(
//            bb_drawing_tool_get_window(BB_DRAWING_TOOL(tool)),
//            &x,
//            &y
//            );

        int ix = (int) round(x);
        int iy = (int) round(y);

//        bb_schematic_window_snap_point(
//            bb_drawing_tool_get_window(BB_DRAWING_TOOL(tool)),
//            &ix,
//            &iy
//            );

        bb_item_builder_put_point(tool->builder, 1, ix, iy);
    }
}


__attribute__((constructor)) void
bb_item_build_tool_register()
{
    bb_item_build_tool_get_type();
}


void
bb_item_build_tool_set_builder(BbItemBuildTool *tool, BbItemBuilder *builder)
{
    g_return_if_fail(tool != NULL);

    if (tool->builder != builder)
    {
        tool->builder = builder;

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_BUILDER]);
    }
}


static void
bb_item_build_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_BUILDER:
            bb_item_build_tool_set_builder(BB_ITEM_BUILD_TOOL(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
