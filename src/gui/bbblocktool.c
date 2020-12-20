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
#include "bbblocktool.h"
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


struct _BbBlockTool
{
    GObject parent;

    BbGedaBlock *item;

    BbToolSubject *subject;
};


static gboolean
bb_block_tool_button_pressed(BbDrawingTool *tool, double x, double y);

static gboolean
bb_block_tool_button_released(BbDrawingTool *tool, double x, double y);

static void
bb_block_tool_dispose(GObject *object);

static void
bb_block_tool_draw(BbDrawingTool *tool, BbGraphics *graphics);

static void
bb_block_tool_drawing_tool_init(BbDrawingToolInterface *iface);

static void
bb_block_tool_finalize(GObject *object);

static void
bb_block_tool_finish(BbBlockTool *block_tool);

static BbGedaBlock*
bb_block_tool_get_item(BbBlockTool *tool);

static void
bb_block_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static BbToolSubject*
bb_block_tool_get_subject(BbBlockTool *tool);

static void
bb_block_tool_invalidate_item_cb(BbGedaItem *item, BbBlockTool *block_tool);

static void
bb_block_tool_key_pressed(BbDrawingTool *tool);

static void
bb_block_tool_key_released(BbDrawingTool *tool);

static gboolean
bb_block_tool_motion_notify(BbDrawingTool *tool, double x, double y);

static void
bb_block_tool_reset(BbBlockTool *block_tool);

static void
bb_block_tool_reset_with_point(BbBlockTool *tool, double x, double y);

static void
bb_block_tool_set_item(BbBlockTool *tool, BbGedaBlock *item);

static void
bb_block_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_block_tool_set_subject(BbBlockTool *tool, BbToolSubject *subject);

static void
bb_block_tool_update_with_point(BbBlockTool *block_tool, double x, double y);


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbBlockTool,
    bb_block_tool,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(BB_TYPE_DRAWING_TOOL, bb_block_tool_drawing_tool_init)
    )


static gboolean
bb_block_tool_button_pressed(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbBlockTool *block_tool = BB_BLOCK_TOOL(tool);
    g_return_val_if_fail(block_tool != NULL, FALSE);

    bb_block_tool_update_with_point(block_tool, x, y);
    bb_block_tool_finish(block_tool);

    return TRUE;
}


static gboolean
bb_block_tool_button_released(BbDrawingTool *tool, double x, double y)
{
}


static void
bb_block_tool_class_init(BbBlockToolClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_block_tool_dispose;
    object_class->finalize = bb_block_tool_finalize;
    object_class->get_property = bb_block_tool_get_property;
    object_class->set_property = bb_block_tool_set_property;

    properties[PROP_ITEM] = bb_object_class_install_property(
        object_class,
        PROP_ITEM,
        g_param_spec_object(
            "item",
            "",
            "",
            BB_TYPE_GEDA_BLOCK,
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
bb_block_tool_dispose(GObject *object)
{
    BbBlockTool *block_tool = BB_BLOCK_TOOL(object);
    g_return_if_fail(block_tool != NULL);

    bb_block_tool_set_item(block_tool, NULL);
    bb_block_tool_set_subject(block_tool, NULL);
}


static void
bb_block_tool_draw(BbDrawingTool *tool, BbGraphics *graphics)
{
    g_message("bb_block_tool_draw");
}


static void
bb_block_tool_drawing_tool_init(BbDrawingToolInterface *iface)
{
    g_return_if_fail(iface != NULL);
    
    iface->button_pressed = bb_block_tool_button_pressed;
    iface->button_released = bb_block_tool_button_released;
    iface->draw = bb_block_tool_draw;
    iface->key_pressed = bb_block_tool_key_pressed;
    iface->key_released = bb_block_tool_key_released;
    iface->motion_notify = bb_block_tool_motion_notify;
}


static void
bb_block_tool_finalize(GObject *object)
{
}


static void
bb_block_tool_finish(BbBlockTool *block_tool)
{
    g_return_if_fail(block_tool != NULL);

    BbGedaItem *item = bb_geda_item_clone(BB_GEDA_ITEM(block_tool->item));

    bb_tool_subject_add_item(block_tool->subject, item);

    g_clear_object(&item);

    bb_block_tool_reset(block_tool);
}


static BbGedaBlock*
bb_block_tool_get_item(BbBlockTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->item;
}


static void
bb_block_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            g_value_set_object(value, bb_block_tool_get_item(BB_BLOCK_TOOL(object)));
            break;

        case PROP_SUBJECT:
            g_value_set_object(value, bb_block_tool_get_subject(BB_BLOCK_TOOL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static BbToolSubject*
bb_block_tool_get_subject(BbBlockTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->subject;
}


static void
bb_block_tool_init(BbBlockTool *block_tool)
{
    bb_block_tool_reset(block_tool);
}


static void
bb_block_tool_invalidate_item_cb(BbGedaItem *item, BbBlockTool *block_tool)
{
    g_return_if_fail(block_tool != NULL);
    g_return_if_fail(block_tool->item != NULL);
    g_return_if_fail(block_tool->item == BB_GEDA_BLOCK(item));

    g_signal_emit(block_tool, signals[SIG_INVALIDATE_ITEM], 0, item);
}


static void
bb_block_tool_key_pressed(BbDrawingTool *tool)
{
    g_message("bb_block_tool_key_pressed");
}


static void
bb_block_tool_key_released(BbDrawingTool *tool)
{
    g_message("bb_block_tool_key_released");
}


BbBlockTool*
bb_block_tool_new(BbToolSubject *subject)
{
    return BB_BLOCK_TOOL(g_object_new(
        BB_TYPE_BLOCK_TOOL,
        "item", g_object_new(BB_TYPE_GEDA_BLOCK, NULL),
        "subject", subject,
        NULL
        ));
}


static void
bb_block_tool_reset(BbBlockTool *block_tool)
{
    g_return_if_fail(block_tool != NULL);
}


static void
bb_block_tool_reset_with_point(BbBlockTool *block_tool, gdouble x, gdouble y)
{
    g_return_if_fail(block_tool != NULL);
    g_return_if_fail(block_tool->item != NULL);

    bb_geda_block_set_insert_x(block_tool->item, x);
    bb_geda_block_set_insert_y(block_tool->item, y);
}


static gboolean
bb_block_tool_motion_notify(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbBlockTool *block_tool = BB_BLOCK_TOOL(tool);
    g_return_val_if_fail(block_tool != NULL, FALSE);

    bb_block_tool_update_with_point(block_tool, x, y);

    return TRUE;
}


static void
bb_block_tool_set_item(BbBlockTool *tool, BbGedaBlock *item)
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
                G_CALLBACK(bb_block_tool_invalidate_item_cb),
                tool,
                0
                );
        }

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_ITEM]);
    }
}


static void
bb_block_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ITEM:
            bb_block_tool_set_item(BB_BLOCK_TOOL(object), BB_GEDA_BLOCK(g_value_get_object(value)));
            break;

        case PROP_SUBJECT:
            bb_block_tool_set_subject(BB_BLOCK_TOOL(object), BB_TOOL_SUBJECT(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_block_tool_set_subject(BbBlockTool *tool, BbToolSubject *subject)
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
bb_block_tool_update_with_point(BbBlockTool *block_tool, gdouble x, gdouble y)
{
    g_return_if_fail(block_tool != NULL);
    g_return_if_fail(block_tool->item != NULL);

    bb_geda_block_set_insert_x(block_tool->item, x);
    bb_geda_block_set_insert_y(block_tool->item, y);
}
