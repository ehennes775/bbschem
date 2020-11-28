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
#include "bbselecttool.h"
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


struct _BbSelectTool
{
    GObject parent;

    int state;

    BbToolSubject *subject;

    int x[2];
    int y[2];
};


static gboolean
bb_select_tool_button_pressed(BbDrawingTool *tool, double x, double y);

static gboolean
bb_select_tool_button_released(BbDrawingTool *tool, double x, double y);

static void
bb_select_tool_dispose(GObject *object);

static void
bb_select_tool_draw(BbDrawingTool *tool, BbGraphics *graphics);

static void
bb_select_tool_drawing_tool_init(BbDrawingToolInterface *iface);

static void
bb_select_tool_finalize(GObject *object);

static void
bb_select_tool_finish(BbSelectTool *select_tool);

static void
bb_select_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static BbToolSubject*
bb_select_tool_get_subject(BbSelectTool *tool);

static void
bb_select_tool_key_pressed(BbDrawingTool *tool);

static void
bb_select_tool_key_released(BbDrawingTool *tool);

static gboolean
bb_select_tool_motion_notify(BbDrawingTool *tool, double x, double y);

static void
bb_select_tool_reset(BbSelectTool *select_tool);

static void
bb_select_tool_reset_with_point(BbSelectTool *tool, double x, double y);

static void
bb_select_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_select_tool_set_subject(BbSelectTool *tool, BbToolSubject *subject);

static void
bb_select_tool_update_with_point(BbSelectTool *select_tool, double x, double y);


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbSelectTool,
    bb_select_tool,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(BB_TYPE_DRAWING_TOOL, bb_select_tool_drawing_tool_init)
    )


static gboolean
bb_select_tool_button_pressed(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbSelectTool *select_tool = BB_SELECT_TOOL(tool);
    g_return_val_if_fail(select_tool != NULL, FALSE);

    switch (select_tool->state)
    {
        case STATE_S0:
            bb_select_tool_reset_with_point(select_tool, x, y);
            break;

        case STATE_S1:
            bb_select_tool_update_with_point(select_tool, x, y);
            bb_select_tool_finish(select_tool);
            break;

        default:
            g_return_val_if_reached(FALSE);
    }

    return TRUE;
}

static gboolean
bb_select_tool_button_released(BbDrawingTool *tool, double x, double y)
{
}


static void
bb_select_tool_class_init(BbSelectToolClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_select_tool_dispose;
    object_class->finalize = bb_select_tool_finalize;
    object_class->get_property = bb_select_tool_get_property;
    object_class->set_property = bb_select_tool_set_property;

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
bb_select_tool_dispose(GObject *object)
{
    BbSelectTool *select_tool = BB_SELECT_TOOL(object);
    g_return_if_fail(select_tool != NULL);

    bb_select_tool_set_subject(select_tool, NULL);
}


static void
bb_select_tool_draw(BbDrawingTool *tool, BbGraphics *graphics)
{
    BbSelectTool *select_tool = BB_SELECT_TOOL(tool);
    g_return_if_fail(select_tool != NULL);

    if (select_tool->state != STATE_S0)
    {
        bb_graphics_draw_select_box(
            graphics,
            select_tool->x[0],
            select_tool->y[0],
            select_tool->x[1],
            select_tool->y[1]
            );
    }
}


static void
bb_select_tool_drawing_tool_init(BbDrawingToolInterface *iface)
{
    g_return_if_fail(iface != NULL);
    
    iface->button_pressed = bb_select_tool_button_pressed;
    iface->button_released = bb_select_tool_button_released;
    iface->draw = bb_select_tool_draw;
    iface->key_pressed = bb_select_tool_key_pressed;
    iface->key_released = bb_select_tool_key_released;
    iface->motion_notify = bb_select_tool_motion_notify;
}


static void
bb_select_tool_finalize(GObject *object)
{
}


static void
bb_select_tool_finish(BbSelectTool *select_tool)
{
    g_return_if_fail(select_tool != NULL);

    bb_select_tool_reset(select_tool);

    bb_tool_subject_invalidate_rect_dev(
        select_tool->subject,
        select_tool->x[0],
        select_tool->y[0],
        select_tool->x[1],
        select_tool->y[1]
        );
}


static void
bb_select_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_SUBJECT:
            g_value_set_object(value, bb_select_tool_get_subject(BB_SELECT_TOOL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static BbToolSubject*
bb_select_tool_get_subject(BbSelectTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->subject;
}


static void
bb_select_tool_init(BbSelectTool *select_tool)
{
    bb_select_tool_reset(select_tool);
}


static void
bb_select_tool_key_pressed(BbDrawingTool *tool)
{
    g_message("bb_select_tool_key_pressed");
}


static void
bb_select_tool_key_released(BbDrawingTool *tool)
{
    g_message("bb_select_tool_key_released");
}


BbSelectTool*
bb_select_tool_new(BbToolSubject *subject)
{
    return BB_SELECT_TOOL(g_object_new(
        BB_TYPE_SELECT_TOOL,
        "subject", subject,
        NULL
        ));
}


static void
bb_select_tool_reset(BbSelectTool *select_tool)
{
    g_return_if_fail(select_tool != NULL);

    select_tool->state = STATE_S0;
}


static void
bb_select_tool_reset_with_point(BbSelectTool *select_tool, gdouble x, gdouble y)
{
    g_return_if_fail(select_tool != NULL);

    select_tool->x[0] = x;
    select_tool->y[0] = y;

    select_tool->x[1] = x;
    select_tool->y[1] = y;

    select_tool->state = STATE_S1;
}


static gboolean
bb_select_tool_motion_notify(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbSelectTool *select_tool = BB_SELECT_TOOL(tool);
    g_return_val_if_fail(select_tool != NULL, FALSE);

    bb_select_tool_update_with_point(select_tool, x, y);

    return TRUE;
}


static void
bb_select_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_SUBJECT:
            bb_select_tool_set_subject(BB_SELECT_TOOL(object), BB_TOOL_SUBJECT(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_select_tool_set_subject(BbSelectTool *tool, BbToolSubject *subject)
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
bb_select_tool_update_with_point(BbSelectTool *select_tool, gdouble x, gdouble y)
{
    g_return_if_fail(select_tool != NULL);

    if (select_tool->state == STATE_S1)
    {
        bb_tool_subject_invalidate_rect_dev(
            select_tool->subject,
            select_tool->x[0],
            select_tool->y[0],
            select_tool->x[1],
            select_tool->y[1]
            );

        select_tool->x[1] = x;
        select_tool->y[1] = y;

        bb_tool_subject_invalidate_rect_dev(
            select_tool->subject,
            select_tool->x[0],
            select_tool->y[0],
            select_tool->x[1],
            select_tool->y[1]
            );
    }
}
