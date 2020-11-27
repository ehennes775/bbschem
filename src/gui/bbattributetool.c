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
#include "bbattributetool.h"
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


struct _BbAttributeTool
{
    GObject parent;

    BbUniversalText *item;

    BbToolSubject *subject;
};


static gboolean
bb_attribute_tool_button_pressed(BbDrawingTool *tool, double x, double y);

static void
bb_attribute_tool_dispose(GObject *object);

static void
bb_attribute_tool_draw(BbDrawingTool *tool, BbGraphics *graphics);

static void
bb_attribute_tool_drawing_tool_init(BbDrawingToolInterface *iface);

static void
bb_attribute_tool_finalize(GObject *object);

static void
bb_attribute_tool_finish(BbAttributeTool *attribute_tool);

static BbUniversalText*
bb_attribute_tool_get_item(BbAttributeTool *tool);

static void
bb_attribute_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static BbToolSubject*
bb_attribute_tool_get_subject(BbAttributeTool *tool);

static void
bb_attribute_tool_invalidate_item_cb(BbSchematicItem *item, BbAttributeTool *attribute_tool);

static void
bb_attribute_tool_key_pressed(BbDrawingTool *tool);

static void
bb_attribute_tool_key_released(BbDrawingTool *tool);

static gboolean
bb_attribute_tool_motion_notify(BbDrawingTool *tool, double x, double y);

static void
bb_attribute_tool_reset(BbAttributeTool *attribute_tool);

static void
bb_attribute_tool_reset_with_point(BbAttributeTool *tool, double x, double y);

static void
bb_attribute_tool_set_item(BbAttributeTool *tool, BbUniversalText *item);

static void
bb_attribute_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_attribute_tool_set_subject(BbAttributeTool *tool, BbToolSubject *subject);

static void
bb_attribute_tool_update_with_point(BbAttributeTool *attribute_tool, double x, double y);


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbAttributeTool,
    bb_attribute_tool,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(BB_TYPE_DRAWING_TOOL, bb_attribute_tool_drawing_tool_init)
    )


static gboolean
bb_attribute_tool_button_pressed(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbAttributeTool *attribute_tool = BB_ATTRIBUTE_TOOL(tool);
    g_return_val_if_fail(attribute_tool != NULL, FALSE);

    bb_attribute_tool_update_with_point(attribute_tool, x, y);
    bb_attribute_tool_finish(attribute_tool);

    return TRUE;
}


static void
bb_attribute_tool_class_init(BbAttributeToolClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_attribute_tool_dispose;
    object_class->finalize = bb_attribute_tool_finalize;
    object_class->get_property = bb_attribute_tool_get_property;
    object_class->set_property = bb_attribute_tool_set_property;

    properties[PROP_ITEM] = bb_object_class_install_property(
        object_class,
        PROP_ITEM,
        g_param_spec_object(
            "item",
            "",
            "",
            BB_TYPE_UNIVERSAL_TEXT,
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
bb_attribute_tool_dispose(GObject *object)
{
    BbAttributeTool *attribute_tool = BB_ATTRIBUTE_TOOL(object);
    g_return_if_fail(attribute_tool != NULL);

    bb_attribute_tool_set_item(attribute_tool, NULL);
    bb_attribute_tool_set_subject(attribute_tool, NULL);
}


static void
bb_attribute_tool_draw(BbDrawingTool *tool, BbGraphics *graphics)
{
    g_message("bb_attribute_tool_draw");
}


static void
bb_attribute_tool_drawing_tool_init(BbDrawingToolInterface *iface)
{
    g_return_if_fail(iface != NULL);
    
    iface->button_pressed = bb_attribute_tool_button_pressed;
    iface->draw = bb_attribute_tool_draw;
    iface->key_pressed = bb_attribute_tool_key_pressed;
    iface->key_released = bb_attribute_tool_key_released;
    iface->motion_notify = bb_attribute_tool_motion_notify;
}


static void
bb_attribute_tool_finalize(GObject *object)
{
}


static void
bb_attribute_tool_finish(BbAttributeTool *attribute_tool)
{
    g_return_if_fail(attribute_tool != NULL);

    BbSchematicItem *item = bb_schematic_item_clone(BB_SCHEMATIC_ITEM(attribute_tool->item));

    bb_tool_subject_add_item(attribute_tool->subject, item);

    g_clear_object(&item);

    bb_attribute_tool_reset(attribute_tool);
}


static BbUniversalText*
bb_attribute_tool_get_item(BbAttributeTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->item;
}


static void
bb_attribute_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            g_value_set_object(value, bb_attribute_tool_get_item(BB_ATTRIBUTE_TOOL(object)));
            break;

        case PROP_SUBJECT:
            g_value_set_object(value, bb_attribute_tool_get_subject(BB_ATTRIBUTE_TOOL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static BbToolSubject*
bb_attribute_tool_get_subject(BbAttributeTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->subject;
}


static void
bb_attribute_tool_init(BbAttributeTool *attribute_tool)
{
    bb_attribute_tool_reset(attribute_tool);
}


static void
bb_attribute_tool_invalidate_item_cb(BbSchematicItem *item, BbAttributeTool *attribute_tool)
{
    g_return_if_fail(attribute_tool != NULL);
    g_return_if_fail(attribute_tool->item != NULL);
    g_return_if_fail(attribute_tool->item == BB_UNIVERSAL_TEXT(item));

    g_signal_emit(attribute_tool, signals[SIG_INVALIDATE_ITEM], 0, item);
}


static void
bb_attribute_tool_key_pressed(BbDrawingTool *tool)
{
    g_message("bb_attribute_tool_key_pressed");
}


static void
bb_attribute_tool_key_released(BbDrawingTool *tool)
{
    g_message("bb_attribute_tool_key_released");
}


BbAttributeTool*
bb_attribute_tool_new(BbToolSubject *subject)
{
    return BB_ATTRIBUTE_TOOL(g_object_new(
        BB_TYPE_ATTRIBUTE_TOOL,
        "item", g_object_new(BB_TYPE_UNIVERSAL_TEXT, NULL),
        "subject", subject,
        NULL
        ));
}


static void
bb_attribute_tool_reset(BbAttributeTool *attribute_tool)
{
    g_return_if_fail(attribute_tool != NULL);
}


static void
bb_attribute_tool_reset_with_point(BbAttributeTool *attribute_tool, gdouble x, gdouble y)
{
    g_return_if_fail(attribute_tool != NULL);
    g_return_if_fail(attribute_tool->item != NULL);

    bb_universal_text_set_insert_x(attribute_tool->item, x);
    bb_universal_text_set_insert_y(attribute_tool->item, y);
}


static gboolean
bb_attribute_tool_motion_notify(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbAttributeTool *attribute_tool = BB_ATTRIBUTE_TOOL(tool);
    g_return_val_if_fail(attribute_tool != NULL, FALSE);

    bb_attribute_tool_update_with_point(attribute_tool, x, y);

    return TRUE;
}


static void
bb_attribute_tool_set_item(BbAttributeTool *tool, BbUniversalText *item)
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
                G_CALLBACK(bb_attribute_tool_invalidate_item_cb),
                tool,
                0
                );
        }

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_ITEM]);
    }
}


static void
bb_attribute_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            bb_attribute_tool_set_item(BB_ATTRIBUTE_TOOL(object), BB_UNIVERSAL_TEXT(g_value_get_object(value)));
            break;

        case PROP_SUBJECT:
            bb_attribute_tool_set_subject(BB_ATTRIBUTE_TOOL(object), BB_TOOL_SUBJECT(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_attribute_tool_set_subject(BbAttributeTool *tool, BbToolSubject *subject)
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
bb_attribute_tool_update_with_point(BbAttributeTool *attribute_tool, gdouble x, gdouble y)
{
    g_return_if_fail(attribute_tool != NULL);
    g_return_if_fail(attribute_tool->item != NULL);

    bb_universal_text_set_insert_x(attribute_tool->item, x);
    bb_universal_text_set_insert_y(attribute_tool->item, y);
}
