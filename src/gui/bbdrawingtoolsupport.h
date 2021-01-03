#ifndef __BBDRAWINGTOOLSUPPORT__
#define __BBDRAWINGTOOLSUPPORT__
/*
 * bbschem
 * Copyright (C) 2021 Edward C. Hennessy
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
#include "bbtextcontrol.h"

#define BB_TYPE_DRAWING_TOOL_SUPPORT bb_drawing_tool_support_get_type()
G_DECLARE_INTERFACE(BbDrawingToolSupport, bb_drawing_tool_support, BB, DRAWING_TOOL_SUPPORT, GObject)

struct _BbDrawingToolSupportInterface
{
    GTypeInterface g_iface;

    BbDrawingTool* (*get_drawing_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_arc_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_attribute_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_block_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_box_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_bus_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_circle_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_line_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_net_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_path_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_pin_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_select_tool)(BbDrawingToolSupport *support);
    BbDrawingTool* (*select_text_tool)(BbDrawingToolSupport *support, BbTextControl *text_control);
    BbDrawingTool* (*select_zoom_tool)(BbDrawingToolSupport *support);
};


BbDrawingTool*
bb_drawing_tool_support_get_drawing_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_arc_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_attribute_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_block_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_box_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_bus_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_circle_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_line_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_net_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_path_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_pin_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_select_tool(BbDrawingToolSupport *support);

BbDrawingTool*
bb_drawing_tool_support_select_text_tool(BbDrawingToolSupport *support, BbTextControl *text_control);

BbDrawingTool*
bb_drawing_tool_support_select_zoom_tool(BbDrawingToolSupport *support);

#endif
