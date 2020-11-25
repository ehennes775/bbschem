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
#include <bbschematicitem.h>
#include "bbdrawingtool.h"


static gboolean
bb_drawing_tool_button_pressed_missing(BbDrawingTool *tool, gdouble x, gdouble y);

static void
bb_drawing_tool_draw_missing(BbDrawingTool *tool);

static void
bb_drawing_tool_key_pressed_missing(BbDrawingTool *tool);

static void
bb_drawing_tool_key_released_missing(BbDrawingTool *tool);

static gboolean
bb_drawing_tool_motion_notify_missing(BbDrawingTool *tool, gdouble x, gdouble y);


G_DEFINE_INTERFACE(
    BbDrawingTool,
    bb_drawing_tool,
    G_TYPE_OBJECT
    )


static void
bb_drawing_tool_default_init(BbDrawingToolInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->button_pressed = bb_drawing_tool_button_pressed_missing;
    iface->draw = bb_drawing_tool_draw_missing;
    iface->key_pressed = bb_drawing_tool_key_pressed_missing;
    iface->key_released = bb_drawing_tool_key_released_missing;
    iface->motion_notify = bb_drawing_tool_motion_notify_missing;

    g_signal_new(
        "invalidate-item",
        BB_TYPE_DRAWING_TOOL,
        0,
        0,
        NULL,
        NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE,
        1,
        BB_TYPE_SCHEMATIC_ITEM
        );
}


gboolean
bb_drawing_tool_button_pressed(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbDrawingToolInterface *iface = BB_DRAWING_TOOL_GET_IFACE(tool);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->button_pressed != NULL, FALSE);

    return iface->button_pressed(tool, x, y);
}


static gboolean
bb_drawing_tool_button_pressed_missing(BbDrawingTool *tool, gdouble x, gdouble y)
{
    g_error("bb_drawing_tool_button_pressed() not overridden");

    return FALSE;
}


void
bb_drawing_tool_draw(BbDrawingTool *tool)
{
    BbDrawingToolInterface *iface = BB_DRAWING_TOOL_GET_IFACE(tool);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->draw != NULL);

    return iface->draw(tool);
}


static void
bb_drawing_tool_draw_missing(BbDrawingTool *tool)
{
    g_error("bb_drawing_tool_draw() not overridden");
}


void
bb_drawing_tool_key_pressed(BbDrawingTool *tool)
{
    BbDrawingToolInterface *iface = BB_DRAWING_TOOL_GET_IFACE(tool);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->key_pressed != NULL);

    return iface->key_pressed(tool);
}


static void
bb_drawing_tool_key_pressed_missing(BbDrawingTool *tool)
{
    g_error("bb_drawing_tool_key_pressed() not overridden");
}


void
bb_drawing_tool_key_released(BbDrawingTool *tool)
{
    BbDrawingToolInterface *iface = BB_DRAWING_TOOL_GET_IFACE(tool);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->key_released != NULL);

    return iface->key_released(tool);
}


static void
bb_drawing_tool_key_released_missing(BbDrawingTool *tool)
{
    g_error("bb_drawing_tool_key_released() not overridden");
}


gboolean
bb_drawing_tool_motion_notify(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbDrawingToolInterface *iface = BB_DRAWING_TOOL_GET_IFACE(tool);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->motion_notify != NULL, FALSE);

    return iface->motion_notify(tool, x, y);
}


static gboolean
bb_drawing_tool_motion_notify_missing(BbDrawingTool *tool, gdouble x, gdouble y)
{
    g_error("bb_drawing_tool_motion_notify() not overridden");

    return FALSE;
}
