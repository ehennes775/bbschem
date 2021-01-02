#ifndef __BBTOOLCHANGER__
#define __BBTOOLCHANGER__
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
#include "bbdrawingtool.h"
#include "bbdrawingtoolsupport.h"


#define BB_TYPE_TOOL_CHANGER bb_tool_changer_get_type()
G_DECLARE_INTERFACE(BbToolChanger, bb_tool_changer, BB, TOOL_CHANGER, GObject)

struct _BbToolChangerInterface
{
    GTypeInterface g_iface;

    BbDrawingTool* (*select_tool)(BbToolChanger *changer, BbDrawingToolSupport *support);
};


BbDrawingTool*
bb_tool_changer_select_tool(BbToolChanger *changer, BbDrawingToolSupport *support);


#endif
