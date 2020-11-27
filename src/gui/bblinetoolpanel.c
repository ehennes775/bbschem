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
#include "bblinetoolpanel.h"
#include "bbtoolfactory.h"
#include "bblinetool.h"


enum
{
    PROP_0,
    PROP_CAIRO,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


struct _BbLineToolPanel
{
    GtkLabel parent;
};


static BbDrawingTool*
bb_line_tool_panel_create_tool(BbToolFactory *factory, BbToolSubject *subject);

static void
bb_line_tool_panel_dispose(GObject *object);

static void
bb_line_tool_panel_finalize(GObject *object);

static void
bb_line_tool_panel_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_line_tool_panel_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_line_tool_panel_tool_factory_init(BbToolFactoryInterface *iface);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbLineToolPanel,
    bb_line_tool_panel,
    GTK_TYPE_LABEL,
    G_IMPLEMENT_INTERFACE(BB_TYPE_TOOL_FACTORY, bb_line_tool_panel_tool_factory_init)
    )


static void
bb_line_tool_panel_class_init(BbLineToolPanelClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_line_tool_panel_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_line_tool_panel_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_line_tool_panel_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_line_tool_panel_set_property;

    //gtk_widget_class_set_template_from_resource(
    //    GTK_WIDGET_CLASS(klasse),
    //    "/com/github/ehennes775/bbsch/gui/bblinetoolpanel.ui"
    //);
}


static BbDrawingTool*
bb_line_tool_panel_create_tool(BbToolFactory *factory, BbToolSubject *subject)
{
    return BB_DRAWING_TOOL(bb_line_tool_new(subject));
}


static void
bb_line_tool_panel_dispose(GObject *object)
{
    // BbLineToolPanel* privat = BBLINE_TOOL_PANEL_GET_PRIVATE(object);
}


static void
bb_line_tool_panel_finalize(GObject *object)
{
    // BbLineToolPanel* privat = BBLINE_TOOL_PANEL_GET_PRIVATE(object);
}


static void
bb_line_tool_panel_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_line_tool_panel_init(BbLineToolPanel *window)
{
    //gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_line_tool_panel_register()
{
    bb_line_tool_panel_get_type();
}


static void
bb_line_tool_panel_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static void
bb_line_tool_panel_tool_factory_init(BbToolFactoryInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->create_tool = bb_line_tool_panel_create_tool;
}
