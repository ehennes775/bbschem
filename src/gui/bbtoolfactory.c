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
#include "bbtoolfactory.h"


BbDrawingTool*
bb_tool_factory_select_tool_missing(BbToolFactory *factory, BbDrawingToolSupport *support);


G_DEFINE_INTERFACE(BbToolFactory, bb_tool_factory, G_TYPE_OBJECT)


static void
bb_tool_factory_default_init(BbToolFactoryInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->select_tool = bb_tool_factory_select_tool_missing;
}


BbDrawingTool*
bb_tool_factory_select_tool(BbToolFactory *factory, BbDrawingToolSupport *support)
{
    g_return_val_if_fail(factory != NULL, NULL);

    BbToolFactoryInterface *iface = BB_TOOL_FACTORY_GET_IFACE(factory);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->select_tool != NULL, NULL);

    return iface->select_tool(factory, support);
}


BbDrawingTool*
bb_tool_factory_select_tool_missing(BbToolFactory *factory, BbDrawingToolSupport *support)
{
    g_error("bb_tool_factory_select_tool");
}
