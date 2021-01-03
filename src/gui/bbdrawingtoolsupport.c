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
#include "bbdrawingtoolsupport.h"


G_DEFINE_INTERFACE(BbDrawingToolSupport, bb_drawing_tool_support, G_TYPE_OBJECT)


BbDrawingTool*
bb_drawing_tool_support_get_drawing_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->get_drawing_tool != NULL, NULL);

    return iface->get_drawing_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_get_drawing_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_zoom_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_arc_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_arc_tool != NULL, NULL);

    return iface->select_arc_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_arc_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_arc_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_attribute_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_attribute_tool != NULL, NULL);

    return iface->select_attribute_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_attribute_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_attribute_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_block_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_block_tool != NULL, NULL);

    return iface->select_block_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_block_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_block_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_box_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_box_tool != NULL, NULL);

    return iface->select_box_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_box_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_box_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_bus_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_bus_tool != NULL, NULL);

    return iface->select_bus_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_bus_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_bus_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_circle_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_circle_tool != NULL, NULL);

    return iface->select_circle_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_circle_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_circle_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_line_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_line_tool != NULL, NULL);

    return iface->select_line_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_line_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_line_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_net_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_net_tool != NULL, NULL);

    return iface->select_net_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_net_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_net_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_path_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_path_tool != NULL, NULL);

    return iface->select_path_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_path_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_path_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_pin_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_pin_tool != NULL, NULL);

    return iface->select_pin_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_pin_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_pin_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_select_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_select_tool != NULL, NULL);

    return iface->select_select_tool(support);
};

static BbDrawingTool*
bb_drawing_tool_support_select_select_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_select_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_text_tool(BbDrawingToolSupport *support, BbTextControl *text_control)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_text_tool != NULL, NULL);

    return iface->select_text_tool(support, text_control);
};


static BbDrawingTool*
bb_drawing_tool_support_select_text_tool_missing(BbDrawingToolSupport *support, BbTextControl *text_control)
{
    g_error("bb_drawing_tool_support_select_text_tool() not overridden");
}


BbDrawingTool*
bb_drawing_tool_support_select_zoom_tool(BbDrawingToolSupport *support)
{
    g_return_val_if_fail(BB_IS_DRAWING_TOOL_SUPPORT(support), NULL);

    BbDrawingToolSupportInterface *iface = BB_DRAWING_TOOL_SUPPORT_GET_IFACE(support);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_zoom_tool != NULL, NULL);

    return iface->select_zoom_tool(support);
};


static BbDrawingTool*
bb_drawing_tool_support_select_zoom_tool_missing(BbDrawingToolSupport *support)
{
    g_error("bb_drawing_tool_support_select_zoom_tool() not overridden");
}


static void
bb_drawing_tool_support_default_init(BbDrawingToolSupportInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_drawing_tool = bb_drawing_tool_support_get_drawing_tool_missing;
    iface->select_arc_tool = bb_drawing_tool_support_select_arc_tool_missing;
    iface->select_attribute_tool = bb_drawing_tool_support_select_attribute_tool_missing;
    iface->select_block_tool = bb_drawing_tool_support_select_block_tool_missing;
    iface->select_box_tool = bb_drawing_tool_support_select_box_tool_missing;
    iface->select_bus_tool = bb_drawing_tool_support_select_bus_tool_missing;
    iface->select_circle_tool = bb_drawing_tool_support_select_circle_tool_missing;
    iface->select_line_tool = bb_drawing_tool_support_select_line_tool_missing;
    iface->select_path_tool = bb_drawing_tool_support_select_path_tool_missing;
    iface->select_pin_tool = bb_drawing_tool_support_select_pin_tool_missing;
    iface->select_select_tool = bb_drawing_tool_support_select_select_tool_missing;
    iface->select_text_tool = bb_drawing_tool_support_select_text_tool_missing;
    iface->select_zoom_tool = bb_drawing_tool_support_select_zoom_tool_missing;
}
