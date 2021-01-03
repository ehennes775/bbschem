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
#include <bblibrary.h>
#include <bbgedabus.h>
#include "bbbustool.h"
#include "bbdrawingtool.h"


enum
{
    PROP_0,
    PROP_ITEM,
    PROP_SUBJECT,
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
    STATE_S1
};


struct _BbBusTool
{
    GObject parent;

    BbGedaBus *item;

    int state;

    BbToolSubject *subject;
};


static gboolean
bb_bus_tool_button_pressed(BbDrawingTool *tool, double x, double y);

static gboolean
bb_bus_tool_button_released(BbDrawingTool *tool, double x, double y);

static void
bb_bus_tool_dispose(GObject *object);

static void
bb_bus_tool_draw(BbDrawingTool *tool, BbGraphics *graphics);

static void
bb_bus_tool_drawing_tool_init(BbDrawingToolInterface *iface);

static void
bb_bus_tool_finalize(GObject *object);

static void
bb_bus_tool_finish(BbBusTool *bus_tool);

static BbGedaBus*
bb_bus_tool_get_item(BbBusTool *tool);

static void
bb_bus_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static BbToolSubject*
bb_bus_tool_get_subject(BbBusTool *tool);

static void
bb_bus_tool_invalidate_item_cb(BbGedaItem *item, BbBusTool *bus_tool);

static void
bb_bus_tool_key_pressed(BbDrawingTool *tool);

static void
bb_bus_tool_key_released(BbDrawingTool *tool);

static gboolean
bb_bus_tool_motion_notify(BbDrawingTool *tool, double x, double y);

static void
bb_bus_tool_reset(BbBusTool *bus_tool);

static void
bb_bus_tool_reset_with_point(BbBusTool *tool, double x, double y);

static void
bb_bus_tool_set_item(BbBusTool *tool, BbGedaBus *item);

static void
bb_bus_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_bus_tool_set_subject(BbBusTool *tool, BbToolSubject *subject);

static void
bb_bus_tool_update_with_point(BbBusTool *bus_tool, double x, double y);


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_DYNAMIC_TYPE_EXTENDED(
    BbBusTool,
    bb_bus_tool,
    G_TYPE_OBJECT,
    0,
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_DRAWING_TOOL, bb_bus_tool_drawing_tool_init)
    )


static gboolean
bb_bus_tool_button_pressed(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbBusTool *bus_tool = BB_BUS_TOOL(tool);
    g_return_val_if_fail(bus_tool != NULL, FALSE);

    switch (bus_tool->state)
    {
        case STATE_S0:
            bb_bus_tool_reset_with_point(bus_tool, x, y);
            break;

        case STATE_S1:
            bb_bus_tool_update_with_point(bus_tool, x, y);
            bb_bus_tool_finish(bus_tool);
            break;

        default:
            g_return_val_if_reached(FALSE);
    }

    return TRUE;
}


static gboolean
bb_bus_tool_button_released(BbDrawingTool *tool, double x, double y)
{
}


static void
bb_bus_tool_class_init(BbBusToolClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_bus_tool_dispose;
    object_class->finalize = bb_bus_tool_finalize;
    object_class->get_property = bb_bus_tool_get_property;
    object_class->set_property = bb_bus_tool_set_property;

    properties[PROP_ITEM] = bb_object_class_install_property(
        object_class,
        PROP_ITEM,
        g_param_spec_object(
            "item",
            "",
            "",
            BB_TYPE_GEDA_BUS,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_SUBJECT] = bb_object_class_install_property(
        object_class,
        PROP_SUBJECT,
        g_param_spec_object(
            "subject",
            "",
            "",
            BB_TYPE_TOOL_SUBJECT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );

    signals[SIG_INVALIDATE_ITEM] = g_signal_lookup(
        "invalidate-item",
        BB_TYPE_DRAWING_TOOL
        );
}


static void
bb_bus_tool_class_finalize(BbBusToolClass *klasse)
{
}


static void
bb_bus_tool_dispose(GObject *object)
{
    BbBusTool *bus_tool = BB_BUS_TOOL(object);
    g_return_if_fail(bus_tool != NULL);

    bb_bus_tool_set_item(bus_tool, NULL);
    bb_bus_tool_set_subject(bus_tool, NULL);
}


static void
bb_bus_tool_draw(BbDrawingTool *tool, BbGraphics *graphics)
{
    BbBusTool *bus_tool = BB_BUS_TOOL(tool);
    g_return_if_fail(bus_tool != NULL);

    if (bus_tool->state != STATE_S0)
    {
        bb_geda_item_render(
            BB_GEDA_ITEM(bus_tool->item),
            BB_ITEM_RENDERER(graphics)
        );
    }
}


static void
bb_bus_tool_drawing_tool_init(BbDrawingToolInterface *iface)
{
    g_return_if_fail(iface != NULL);
    
    iface->button_pressed = bb_bus_tool_button_pressed;
    iface->button_released = bb_bus_tool_button_released;
    iface->draw = bb_bus_tool_draw;
    iface->key_pressed = bb_bus_tool_key_pressed;
    iface->key_released = bb_bus_tool_key_released;
    iface->motion_notify = bb_bus_tool_motion_notify;
}


static void
bb_bus_tool_finalize(GObject *object)
{
}


static void
bb_bus_tool_finish(BbBusTool *bus_tool)
{
    g_return_if_fail(bus_tool != NULL);

    BbGedaItem *item = bb_geda_item_clone(BB_GEDA_ITEM(bus_tool->item));

    bb_tool_subject_add_item(bus_tool->subject, item);

    g_clear_object(&item);

    bb_bus_tool_reset(bus_tool);
}


static BbGedaBus*
bb_bus_tool_get_item(BbBusTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->item;
}


static void
bb_bus_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            g_value_set_object(value, bb_bus_tool_get_item(BB_BUS_TOOL(object)));
            break;

        case PROP_SUBJECT:
            g_value_set_object(value, bb_bus_tool_get_subject(BB_BUS_TOOL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static BbToolSubject*
bb_bus_tool_get_subject(BbBusTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->subject;
}


static void
bb_bus_tool_init(BbBusTool *bus_tool)
{
    bb_bus_tool_reset(bus_tool);
}


static void
bb_bus_tool_invalidate_item_cb(BbGedaItem *item, BbBusTool *bus_tool)
{
    g_return_if_fail(bus_tool != NULL);
    g_return_if_fail(bus_tool->item != NULL);
    g_return_if_fail(bus_tool->item == BB_GEDA_BUS(item));

    g_signal_emit(bus_tool, signals[SIG_INVALIDATE_ITEM], 0, item);
}


static void
bb_bus_tool_key_pressed(BbDrawingTool *tool)
{
    g_message("bb_bus_tool_key_pressed");
}


static void
bb_bus_tool_key_released(BbDrawingTool *tool)
{
    g_message("bb_bus_tool_key_released");
}


BbBusTool*
bb_bus_tool_new(BbToolSubject *subject)
{
    return BB_BUS_TOOL(g_object_new(
        BB_TYPE_BUS_TOOL,
        "item", g_object_new(BB_TYPE_GEDA_BUS, NULL),
        "subject", subject,
        NULL
        ));
}


void
bb_bus_tool_register(GTypeModule *module)
{
    bb_bus_tool_register_type(module);
}


static void
bb_bus_tool_reset(BbBusTool *bus_tool)
{
    g_return_if_fail(bus_tool != NULL);

    bus_tool->state = STATE_S0;
}


static void
bb_bus_tool_reset_with_point(BbBusTool *bus_tool, gdouble x, gdouble y)
{
    g_return_if_fail(bus_tool != NULL);

    double ux;
    double uy;

    gboolean success = bb_tool_subject_widget_to_user(bus_tool->subject, x, y, &ux, &uy);
    g_return_if_fail(success);

    int sx;
    int sy;

    bb_tool_subject_snap_coordinate(bus_tool->subject, bb_coord_round(ux), bb_coord_round(uy), &sx, &sy);

    bb_geda_bus_set_x0(bus_tool->item, sx);
    bb_geda_bus_set_y0(bus_tool->item, sy);

    bb_geda_bus_set_x1(bus_tool->item, sx);
    bb_geda_bus_set_y1(bus_tool->item, sy);

    bus_tool->state = STATE_S1;
}


static gboolean
bb_bus_tool_motion_notify(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbBusTool *bus_tool = BB_BUS_TOOL(tool);
    g_return_val_if_fail(bus_tool != NULL, FALSE);

    bb_bus_tool_update_with_point(bus_tool, x, y);

    return TRUE;
}


static void
bb_bus_tool_set_item(BbBusTool *tool, BbGedaBus *item)
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
                G_CALLBACK(bb_bus_tool_invalidate_item_cb),
                tool,
                0
                );
        }

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_ITEM]);
    }
}


static void
bb_bus_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            bb_bus_tool_set_item(BB_BUS_TOOL(object), BB_GEDA_BUS(g_value_get_object(value)));
            break;

        case PROP_SUBJECT:
            bb_bus_tool_set_subject(BB_BUS_TOOL(object), BB_TOOL_SUBJECT(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_bus_tool_set_subject(BbBusTool *tool, BbToolSubject *subject)
{
    g_return_if_fail(tool != NULL);

    if (tool->subject != subject)
    {
        if (tool->subject != NULL)
        {
            g_object_unref(tool->subject);
        }

        tool->subject = subject;

        if (tool->subject != NULL)
        {
            g_object_ref(tool->subject);
        }

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_SUBJECT]);
    }
}


static void
bb_bus_tool_update_with_point(BbBusTool *bus_tool, gdouble x, gdouble y)
{
    g_return_if_fail(bus_tool != NULL);
    g_return_if_fail(bus_tool->item != NULL);

    if (bus_tool->state == STATE_S1)
    {
        double ux;
        double uy;

        gboolean success = bb_tool_subject_widget_to_user(bus_tool->subject, x, y, &ux, &uy);
        g_return_if_fail(success);

        int sx;
        int sy;

        bb_tool_subject_snap_coordinate(bus_tool->subject, bb_coord_round(ux), bb_coord_round(uy), &sx, &sy);

        bb_geda_bus_set_x1(bus_tool->item, sx);
        bb_geda_bus_set_y1(bus_tool->item, sy);
    }
}
