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
#include <bblibrary.h>
#include "bbtoolchanger.h"


G_DEFINE_INTERFACE(BbToolChanger, bb_tool_changer, G_TYPE_OBJECT)


BbDrawingTool*
bb_tool_changer_select_tool(BbToolChanger *changer, BbDrawingToolSupport *support)
{
    g_return_val_if_fail(changer != NULL, NULL);

    BbToolChangerInterface *iface = BB_TOOL_CHANGER_GET_IFACE(changer);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_tool != NULL, NULL);

    return iface->select_tool(changer, support);
}


BbDrawingTool*
bb_tool_changer_select_tool_missing(BbToolChanger *changer, BbDrawingToolSupport *support)
{
    g_error("bb_tool_changer_select_tool() not overridden");
}


static void
bb_tool_changer_default_init(BbToolChangerInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->select_tool = bb_tool_changer_select_tool_missing;

    g_signal_new(
        "tool-changed",
        BB_TYPE_TOOL_CHANGER,
        0,
        0,
        NULL,
        NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE,
        0
        );
}
