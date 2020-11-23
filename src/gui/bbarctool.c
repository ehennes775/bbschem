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
#include "bbschematicitem.h"
#include "bbdrawingtool.h"
#include "bbarctool.h"
#include "bbgraphicarc.h"

enum
{
    PROP_0,
    PROP_ITEM,
    N_PROPERTIES
};


struct _BbArcTool
{
    GObject parent;

    BbGraphicArc *item;
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

static BbGraphicArc*
bb_arc_tool_get_item(BbArcTool *tool);

static void
bb_arc_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_arc_tool_invalidate_item_cb(BbSchematicItem *item, BbArcTool *tool);

static void
bb_arc_tool_key_pressed(BbDrawingTool *tool);

static void
bb_arc_tool_key_released(BbDrawingTool *tool);

static void
bb_arc_tool_motion_notify(BbDrawingTool *tool);

static void
bb_arc_tool_set_item(BbArcTool *tool, BbGraphicArc *item);

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

    BbArcTool *arc_tool = BB_ARC_TOOL(tool);
    g_return_if_fail(arc_tool != NULL);

    bb_graphic_arc_set_center_x(
        arc_tool->item,
        bb_graphic_arc_get_center_x(arc_tool->item) + 1
        );

    bb_graphic_arc_set_center_x(
        arc_tool->item,
        bb_graphic_arc_get_center_x(arc_tool->item) + 1
        );
}


static void
bb_arc_tool_class_init(BbArcToolClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_arc_tool_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_arc_tool_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_arc_tool_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_arc_tool_set_property;

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_ITEM,
        properties[PROP_ITEM] = g_param_spec_object(
            "item",
            "",
            "",
            BB_TYPE_GRAPHIC_ARC,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );
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


static BbGraphicArc*
bb_arc_tool_get_item(BbArcTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->item;
}


static void
bb_arc_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            g_value_set_object(value, bb_arc_tool_get_item(BB_ARC_TOOL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_arc_tool_init(BbArcTool *tool)
{
    bb_arc_tool_set_item(
        tool,
        g_object_new(BB_TYPE_GRAPHIC_ARC, NULL)
        );
}


static void
bb_arc_tool_invalidate_item_cb(BbSchematicItem *item, BbArcTool *tool)
{
    g_return_if_fail(tool != NULL);

    g_signal_emit(
        tool,
        g_signal_lookup("invalidate-item", BB_TYPE_DRAWING_TOOL),
        0,
        item
        );
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
bb_arc_tool_motion_notify(BbDrawingTool *tool)
{
    //g_message("bb_arc_tool_motion_notify");
}


static void
bb_arc_tool_set_item(BbArcTool *tool, BbGraphicArc *item)
{
    g_return_if_fail(tool != NULL);

    if (tool->item != item)
    {
        if (tool->item != NULL)
        {
            g_signal_handlers_disconnect_by_data(item, tool);

            g_object_unref(tool->item);
        }

        tool->item = item;

        if (tool->item != NULL)
        {
            g_object_ref(tool->item);

            g_signal_connect_object(
                item,
                "invalidate-item",
                G_CALLBACK(bb_arc_tool_invalidate_item_cb),
                tool,
                0
                );
        }

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_ITEM]);
    }
}


static void
bb_arc_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            bb_arc_tool_set_item(BB_ARC_TOOL(object), BB_GRAPHIC_ARC(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
