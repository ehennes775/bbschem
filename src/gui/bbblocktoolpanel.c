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
#include "bbblocktoolpanel.h"
#include "bbtoolfactory.h"
#include "bbblocktool.h"


struct _BbBlockToolPanel
{
    GtkBox parent;
};


static BbDrawingTool*
bb_block_tool_panel_select_tool(BbToolFactory *factory, BbDrawingToolSupport *support);

static void
bb_block_tool_panel_tool_factory_init(BbToolFactoryInterface *iface);


G_DEFINE_TYPE_WITH_CODE(
    BbBlockToolPanel,
    bb_block_tool_panel,
    GTK_TYPE_BOX,
    G_IMPLEMENT_INTERFACE(BB_TYPE_TOOL_FACTORY, bb_block_tool_panel_tool_factory_init)
    )


static void
bb_block_tool_panel_class_init(BbBlockToolPanelClass *class)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbblocktoolpanel.ui"
        );
}


static BbDrawingTool*
bb_block_tool_panel_select_tool(BbToolFactory *factory, BbDrawingToolSupport *support)
{
    return bb_drawing_tool_support_select_block_tool(support);
}


static void
bb_block_tool_panel_init(BbBlockToolPanel *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_block_tool_panel_register()
{
    bb_block_tool_panel_get_type();
}


static void
bb_block_tool_panel_tool_factory_init(BbToolFactoryInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->select_tool = bb_block_tool_panel_select_tool;
}
