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
#include <bbcoord.h>
#include <bbangle.h>
#include "bbschematicitem.h"
#include "bbdrawingtool.h"
#include "bbarctool.h"
#include "bbgraphicarc.h"
#include "bbextensions.h"

enum
{
    PROP_0,
    PROP_ITEM,
    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE_ITEM,
    N_SIGNALS
};


enum
{
    STATE_S0,
    STATE_S1,
    STATE_S2,
    STATE_S3
};


struct _BbArcTool
{
    GObject parent;

    BbGraphicArc *item;

    int state;
};


static gboolean
bb_arc_tool_button_pressed(BbDrawingTool *tool, double x, double y);

static void
bb_arc_tool_dispose(GObject *object);

static void
bb_arc_tool_draw(BbDrawingTool *tool);

static void
bb_arc_tool_drawing_tool_init(BbDrawingToolInterface *iface);

static void
bb_arc_tool_finalize(GObject *object);

static void
bb_arc_tool_finish(BbArcTool *arc_tool);

static BbGraphicArc*
bb_arc_tool_get_item(BbArcTool *tool);

static void
bb_arc_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_arc_tool_invalidate_item_cb(BbSchematicItem *item, BbArcTool *arc_tool);

static void
bb_arc_tool_key_pressed(BbDrawingTool *tool);

static void
bb_arc_tool_key_released(BbDrawingTool *tool);

static gboolean
bb_arc_tool_motion_notify(BbDrawingTool *tool, double x, double y);

static void
bb_arc_tool_reset(BbArcTool *arc_tool);

static void
bb_arc_tool_reset_with_point(BbArcTool *tool, double x, double y);

static void
bb_arc_tool_set_item(BbArcTool *tool, BbGraphicArc *item);

static void
bb_arc_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_arc_tool_update_with_point(BbArcTool *arc_tool, double x, double y);


static GParamSpec *properties[N_PROPERTIES];

static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbArcTool,
    bb_arc_tool,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(BB_TYPE_DRAWING_TOOL, bb_arc_tool_drawing_tool_init)
    )


static gboolean
bb_arc_tool_button_pressed(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbArcTool *arc_tool = BB_ARC_TOOL(tool);
    g_return_val_if_fail(arc_tool != NULL, FALSE);

    switch (arc_tool->state)
    {
        case STATE_S0:
            bb_arc_tool_reset_with_point(arc_tool, x, y);
            break;

        case STATE_S1:
            bb_arc_tool_update_with_point(arc_tool, x, y);
            arc_tool->state = STATE_S2;
            break;

        case STATE_S2:
            bb_arc_tool_update_with_point(arc_tool, x, y);
            arc_tool->state = STATE_S3;
            break;

        case STATE_S3:
            bb_arc_tool_update_with_point(arc_tool, x, y);
            bb_arc_tool_finish(arc_tool);
            break;

        default:
            g_return_val_if_reached(FALSE);
    }

    return TRUE;
}


static void
bb_arc_tool_class_init(BbArcToolClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_arc_tool_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_arc_tool_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_arc_tool_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_arc_tool_set_property;

    bb_object_class_install_property(
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

    signals[SIG_INVALIDATE_ITEM] = g_signal_lookup("invalidate-item", BB_TYPE_DRAWING_TOOL);
}


static void
bb_arc_tool_dispose(GObject *object)
{
    BbArcTool *arc_tool = BB_ARC_TOOL(object);
    g_return_if_fail(arc_tool != NULL);

    bb_arc_tool_set_item(arc_tool, NULL);
}


static void
bb_arc_tool_draw(BbDrawingTool *tool)
{
    g_message("bb_arc_tool_draw");
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
bb_arc_tool_finish(BbArcTool *arc_tool)
{
    // TODO Add the new arc to the schematic

    bb_arc_tool_reset(arc_tool);
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
bb_arc_tool_invalidate_item_cb(BbSchematicItem *item, BbArcTool *arc_tool)
{
    g_return_if_fail(arc_tool != NULL);
    g_return_if_fail(arc_tool->item != NULL);
    g_return_if_fail(arc_tool->item == BB_GRAPHIC_ARC(item));

    g_signal_emit(arc_tool, signals[SIG_INVALIDATE_ITEM], 0, item);
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


static void
bb_arc_tool_reset(BbArcTool *arc_tool)
{
    g_return_if_fail(arc_tool != NULL);

    arc_tool->state = STATE_S0;
}


static void
bb_arc_tool_reset_with_point(BbArcTool *arc_tool, gdouble x, gdouble y)
{
    g_return_if_fail(arc_tool != NULL);

    bb_graphic_arc_set_center_x(arc_tool->item, x);
    bb_graphic_arc_set_center_y(arc_tool->item, y);

    bb_graphic_arc_set_radius(arc_tool->item, 100);

    bb_graphic_arc_set_start_angle(arc_tool->item, 0);
    bb_graphic_arc_set_sweep_angle(arc_tool->item, 270);

    arc_tool->state = STATE_S1;
}


static void
bb_arc_tool_update_with_point(BbArcTool *arc_tool, gdouble x, gdouble y)
{
    g_return_if_fail(arc_tool != NULL);
    g_return_if_fail(arc_tool->item != NULL);

    if (arc_tool->state == STATE_S1)
    {
        double distance = bb_coord_distance(
            bb_graphic_arc_get_center_x(arc_tool->item),
            bb_graphic_arc_get_center_y(arc_tool->item),
            x,
            y
            );

        bb_graphic_arc_set_radius(arc_tool->item, distance);
    }
    else if (arc_tool->state == STATE_S2)
    {
        double radians = bb_coord_radians(
            bb_graphic_arc_get_center_x(arc_tool->item),
            bb_graphic_arc_get_center_y(arc_tool->item),
            x,
            y
            );

        bb_graphic_arc_set_start_angle(arc_tool->item, bb_angle_from_radians(radians));
    }
    else if (arc_tool->state == STATE_S3)
    {
        double radians = bb_coord_radians(
            bb_graphic_arc_get_center_x(arc_tool->item),
            bb_graphic_arc_get_center_y(arc_tool->item),
            x,
            y
            );

        int sweep = bb_angle_calculate_sweep(
            bb_graphic_arc_get_start_angle(arc_tool->item),
            bb_angle_from_radians(radians)
            );

        bb_graphic_arc_set_sweep_angle(arc_tool->item, sweep);
    }
}


static gboolean
bb_arc_tool_motion_notify(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbArcTool *arc_tool = BB_ARC_TOOL(tool);
    g_return_val_if_fail(arc_tool != NULL, FALSE);

    bb_arc_tool_update_with_point(arc_tool, x, y);

    return TRUE;
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
