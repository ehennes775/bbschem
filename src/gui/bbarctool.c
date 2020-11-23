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
#include "bbarctool.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


struct _BbArcTool
{
    GObject parent;
};


static void
bb_arc_tool_button_pressed(BbDrawingTool *tool);

static void
bb_arc_tool_dispose(GObject *object);

static void
bb_arc_tool_draw(BbDrawingTool *tool);

static void
bb_arc_tool_drawing_tool_init(BbDrawingToolInterface *iface);

static void
bb_arc_tool_finalize(GObject *object);

static void
bb_arc_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_arc_tool_key_pressed(BbDrawingTool *tool);

static void
bb_arc_tool_key_released(BbDrawingTool *tool);

static void
bb_arc_tool_motion_notify(BbDrawingTool *tool);

static void
bb_arc_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbArcTool,
    bb_arc_tool,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(BB_TYPE_DRAWING_TOOL, bb_arc_tool_drawing_tool_init)
    )


static void
bb_arc_tool_button_pressed(BbDrawingTool *tool)
{
    g_message("bb_arc_tool_button_pressed");
}


static void
bb_arc_tool_class_init(BbArcToolClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_arc_tool_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_arc_tool_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_arc_tool_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_arc_tool_set_property;
}


static void
bb_arc_tool_dispose(GObject *object)
{
    g_return_if_fail(object != NULL);
}


static void
bb_arc_tool_draw(BbDrawingTool *tool)
{
    g_message("bb_arc_tool_button_pressed");
}


static void
bb_arc_tool_drawing_tool_init(BbDrawingToolInterface *iface)
{
    g_return_if_fail(iface != NULL);
    
    iface->button_pressed = bb_arc_tool_button_pressed;
    iface->draw = bb_arc_tool_draw;
    iface->key_pressed = bb_arc_tool_key_pressed;
    iface->key_released = bb_arc_tool_key_released;
    iface->motion_notify = bb_arc_tool_motion_notify;
}


static void
bb_arc_tool_finalize(GObject *object)
{
}


static void
bb_arc_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
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
bb_arc_tool_init(BbArcTool *window)
{
}

BbArcTool*
bb_arc_tool_new()
{
    return BB_ARC_TOOL(g_object_new(
        BB_TYPE_ARC_TOOL,
        NULL
        ));
}


__attribute__((constructor)) void
bb_arc_tool_register()
{
    bb_arc_tool_get_type();
}


static void
bb_arc_tool_key_pressed(BbDrawingTool *tool)
{
    g_message("bb_arc_tool_key_pressed");
}


static void
bb_arc_tool_key_released(BbDrawingTool *tool)
{
    g_message("bb_arc_tool_key_released");
}


static void
bb_arc_tool_motion_notify(BbDrawingTool *tool)
{
    g_message("bb_arc_tool_motion_notify");
}


static void
bb_arc_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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
