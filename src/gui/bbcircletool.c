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
#include "bbcircletool.h"
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


struct _BbCircleTool
{
    GObject parent;

    BbGraphicCircle *item;

    int state;

    BbToolSubject *subject;
};


static gboolean
bb_circle_tool_button_pressed(BbDrawingTool *tool, double x, double y);

static gboolean
bb_circle_tool_button_released(BbDrawingTool *tool, double x, double y);

static void
bb_circle_tool_dispose(GObject *object);

static void
bb_circle_tool_draw(BbDrawingTool *tool, BbGraphics *graphics);

static void
bb_circle_tool_drawing_tool_init(BbDrawingToolInterface *iface);

static void
bb_circle_tool_finalize(GObject *object);

static void
bb_circle_tool_finish(BbCircleTool *circle_tool);

static BbGraphicCircle*
bb_circle_tool_get_item(BbCircleTool *tool);

static void
bb_circle_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static BbToolSubject*
bb_circle_tool_get_subject(BbCircleTool *tool);

static void
bb_circle_tool_invalidate_item_cb(BbSchematicItem *item, BbCircleTool *circle_tool);

static void
bb_circle_tool_key_pressed(BbDrawingTool *tool);

static void
bb_circle_tool_key_released(BbDrawingTool *tool);

static gboolean
bb_circle_tool_motion_notify(BbDrawingTool *tool, double x, double y);

static void
bb_circle_tool_reset(BbCircleTool *circle_tool);

static void
bb_circle_tool_reset_with_point(BbCircleTool *tool, double x, double y);

static void
bb_circle_tool_set_item(BbCircleTool *tool, BbGraphicCircle *item);

static void
bb_circle_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_circle_tool_set_subject(BbCircleTool *tool, BbToolSubject *subject);

static void
bb_circle_tool_update_with_point(BbCircleTool *circle_tool, double x, double y);


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbCircleTool,
    bb_circle_tool,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(BB_TYPE_DRAWING_TOOL, bb_circle_tool_drawing_tool_init)
    )


static gboolean
bb_circle_tool_button_pressed(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbCircleTool *circle_tool = BB_CIRCLE_TOOL(tool);
    g_return_val_if_fail(circle_tool != NULL, FALSE);

    switch (circle_tool->state)
    {
        case STATE_S0:
            bb_circle_tool_reset_with_point(circle_tool, x, y);
            break;

        case STATE_S1:
            bb_circle_tool_update_with_point(circle_tool, x, y);
            bb_circle_tool_finish(circle_tool);
            break;

        default:
            g_return_val_if_reached(FALSE);
    }

    return TRUE;
}


static gboolean
bb_circle_tool_button_released(BbDrawingTool *tool, double x, double y)
{
}


static void
bb_circle_tool_class_init(BbCircleToolClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_circle_tool_dispose;
    object_class->finalize = bb_circle_tool_finalize;
    object_class->get_property = bb_circle_tool_get_property;
    object_class->set_property = bb_circle_tool_set_property;

    properties[PROP_ITEM] = bb_object_class_install_property(
        object_class,
        PROP_ITEM,
        g_param_spec_object(
            "item",
            "",
            "",
            BB_TYPE_GRAPHIC_CIRCLE,
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
bb_circle_tool_dispose(GObject *object)
{
    BbCircleTool *circle_tool = BB_CIRCLE_TOOL(object);
    g_return_if_fail(circle_tool != NULL);

    bb_circle_tool_set_item(circle_tool, NULL);
    bb_circle_tool_set_subject(circle_tool, NULL);
}


static void
bb_circle_tool_draw(BbDrawingTool *tool, BbGraphics *graphics)
{
    BbCircleTool *circle_tool = BB_CIRCLE_TOOL(tool);
    g_return_if_fail(circle_tool != NULL);

    if (circle_tool->state != STATE_S0)
    {
        bb_schematic_item_render(
            BB_SCHEMATIC_ITEM(circle_tool->item),
            BB_ITEM_RENDERER(graphics)
         );
    }
}


static void
bb_circle_tool_drawing_tool_init(BbDrawingToolInterface *iface)
{
    g_return_if_fail(iface != NULL);
    
    iface->button_pressed = bb_circle_tool_button_pressed;
    iface->button_released = bb_circle_tool_button_released;
    iface->draw = bb_circle_tool_draw;
    iface->key_pressed = bb_circle_tool_key_pressed;
    iface->key_released = bb_circle_tool_key_released;
    iface->motion_notify = bb_circle_tool_motion_notify;
}


static void
bb_circle_tool_finalize(GObject *object)
{
}


static void
bb_circle_tool_finish(BbCircleTool *circle_tool)
{
    g_return_if_fail(circle_tool != NULL);

    BbSchematicItem *item = bb_schematic_item_clone(BB_SCHEMATIC_ITEM(circle_tool->item));

    bb_tool_subject_add_item(circle_tool->subject, item);

    g_clear_object(&item);

    bb_circle_tool_reset(circle_tool);
}


static BbGraphicCircle*
bb_circle_tool_get_item(BbCircleTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->item;
}


static void
bb_circle_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            g_value_set_object(value, bb_circle_tool_get_item(BB_CIRCLE_TOOL(object)));
            break;

        case PROP_SUBJECT:
            g_value_set_object(value, bb_circle_tool_get_subject(BB_CIRCLE_TOOL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static BbToolSubject*
bb_circle_tool_get_subject(BbCircleTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->subject;
}


static void
bb_circle_tool_init(BbCircleTool *circle_tool)
{
    bb_circle_tool_reset(circle_tool);
}


static void
bb_circle_tool_invalidate_item_cb(BbSchematicItem *item, BbCircleTool *circle_tool)
{
    g_return_if_fail(circle_tool != NULL);
    g_return_if_fail(circle_tool->item != NULL);
    g_return_if_fail(circle_tool->item == BB_GRAPHIC_CIRCLE(item));

    g_signal_emit(circle_tool, signals[SIG_INVALIDATE_ITEM], 0, item);
}


static void
bb_circle_tool_key_pressed(BbDrawingTool *tool)
{
    g_message("bb_circle_tool_key_pressed");
}


static void
bb_circle_tool_key_released(BbDrawingTool *tool)
{
    g_message("bb_circle_tool_key_released");
}


BbCircleTool*
bb_circle_tool_new(BbToolSubject *subject)
{
    return BB_CIRCLE_TOOL(g_object_new(
        BB_TYPE_CIRCLE_TOOL,
        "item", g_object_new(BB_TYPE_GRAPHIC_CIRCLE, NULL),
        "subject", subject,
        NULL
        ));
}


static void
bb_circle_tool_reset(BbCircleTool *circle_tool)
{
    g_return_if_fail(circle_tool != NULL);

    circle_tool->state = STATE_S0;
}


static void
bb_circle_tool_reset_with_point(BbCircleTool *circle_tool, gdouble x, gdouble y)
{
    g_return_if_fail(circle_tool != NULL);

    double ux;
    double uy;

    gboolean success = bb_tool_subject_widget_to_user(circle_tool->subject, x, y, &ux, &uy);
    g_return_if_fail(success);

    int sx;
    int sy;

    bb_tool_subject_snap_coordinate(circle_tool->subject, bb_coord_round(ux), bb_coord_round(uy), &sx, &sy);

    bb_graphic_circle_set_center_x(circle_tool->item, sx);
    bb_graphic_circle_set_center_y(circle_tool->item, sy);

    bb_graphic_circle_set_radius(circle_tool->item, 0);

    circle_tool->state = STATE_S1;
}


static gboolean
bb_circle_tool_motion_notify(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbCircleTool *circle_tool = BB_CIRCLE_TOOL(tool);
    g_return_val_if_fail(circle_tool != NULL, FALSE);

    bb_circle_tool_update_with_point(circle_tool, x, y);

    return TRUE;
}


static void
bb_circle_tool_set_item(BbCircleTool *tool, BbGraphicCircle *item)
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
                G_CALLBACK(bb_circle_tool_invalidate_item_cb),
                tool,
                0
                );
        }

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_ITEM]);
    }
}


static void
bb_circle_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            bb_circle_tool_set_item(BB_CIRCLE_TOOL(object), BB_GRAPHIC_CIRCLE(g_value_get_object(value)));
            break;

        case PROP_SUBJECT:
            bb_circle_tool_set_subject(BB_CIRCLE_TOOL(object), BB_TOOL_SUBJECT(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_circle_tool_set_subject(BbCircleTool *tool, BbToolSubject *subject)
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
bb_circle_tool_update_with_point(BbCircleTool *circle_tool, gdouble x, gdouble y)
{
    g_return_if_fail(circle_tool != NULL);
    g_return_if_fail(circle_tool->item != NULL);

    if (circle_tool->state == STATE_S1)
    {
        double ux;
        double uy;

        gboolean success = bb_tool_subject_widget_to_user(circle_tool->subject, x, y, &ux, &uy);
        g_return_if_fail(success);

        double distance = bb_coord_distance(
            bb_graphic_circle_get_center_x(circle_tool->item),
            bb_graphic_circle_get_center_y(circle_tool->item),
            ux,
            uy
            );

        bb_graphic_circle_set_radius(circle_tool->item, distance);
    }
}
