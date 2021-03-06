#ifndef __BBDRAWINGTOOL__
#define __BBDRAWINGTOOL__
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
#include "bbgraphics.h"

#define BB_TYPE_DRAWING_TOOL bb_drawing_tool_get_type()
G_DECLARE_INTERFACE(BbDrawingTool, bb_drawing_tool, BB, DRAWING_TOOL, GObject)

struct _BbDrawingToolInterface
{
    GTypeInterface g_iface;

    gboolean (*button_pressed)(BbDrawingTool *tool, gdouble x, gdouble y);
    gboolean (*button_released)(BbDrawingTool *tool, gdouble x, gdouble y);
    void (*draw)(BbDrawingTool *tool, BbGraphics *graphics);
    void (*key_pressed)(BbDrawingTool *tool);
    void (*key_released)(BbDrawingTool *tool);
    gboolean (*motion_notify)(BbDrawingTool *tool, gdouble x, gdouble y);
};


gboolean
bb_drawing_tool_button_pressed(BbDrawingTool *tool, gdouble x, gdouble y);


gboolean
bb_drawing_tool_button_released(BbDrawingTool *tool, gdouble x, gdouble y);


void
bb_drawing_tool_draw(BbDrawingTool *tool, BbGraphics *graphics);


void
bb_drawing_tool_key_pressed(BbDrawingTool *tool);


void
bb_drawing_tool_key_released(BbDrawingTool *tool);


gboolean
bb_drawing_tool_motion_notify(BbDrawingTool *tool, gdouble x, gdouble y);


#endif
