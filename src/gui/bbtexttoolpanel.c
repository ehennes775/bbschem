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
#include <bbcolors.h>
#include <bbtextsize.h>
#include <bbextensions.h>
#include "bbtexttoolpanel.h"
#include "bbtoolfactory.h"
#include "gedaplugin/bbtexttool.h"
#include "bbtextcontrol.h"


enum
{
    PROP_0,

    PROP_ALIGNMENT,
    PROP_COLOR,
    PROP_PRESENTATION,
    PROP_SIZE,
    PROP_ROTATION,
    PROP_TEXT,
    PROP_VISIBILITY,

    N_PROPERTIES
};


struct _BbTextToolPanel
{
    GtkBox parent;
};


static void
bb_text_tool_panel_dispose(GObject *object);

static void
bb_text_tool_panel_finalize(GObject *object);

static void
bb_text_tool_panel_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static BbDrawingTool*
bb_text_tool_panel_select_tool(BbToolFactory *factory, BbDrawingToolSupport *support);

static void
bb_text_tool_panel_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_text_tool_panel_text_control_init(BbTextControlInterface *iface);

static void
bb_text_tool_panel_tool_factory_init(BbToolFactoryInterface *iface);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbTextToolPanel,
    bb_text_tool_panel,
    GTK_TYPE_BOX,
    G_IMPLEMENT_INTERFACE(BB_TYPE_TEXT_CONTROL, bb_text_tool_panel_text_control_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_TOOL_FACTORY, bb_text_tool_panel_tool_factory_init)
    )

    
// region From BbTextControl Interface

static BbTextAlignment
bb_text_tool_panel_get_alignment(BbTextControl *text_control)
{
    return BB_TEXT_ALIGNMENT_DEFAULT;
}


static int
bb_text_tool_panel_get_color(BbTextControl *text_control)
{
    return BB_COLOR_GRAPHIC;
}


static BbTextPresentation
bb_text_tool_panel_get_presentation(BbTextControl *text_control)
{
    return BB_TEXT_PRESENTATION_DEFAULT;
}


static int
bb_text_tool_panel_get_rotation(BbTextControl *text_control)
{
    return 0;
}


static int
bb_text_tool_panel_get_size(BbTextControl *text_control)
{
    return BB_TEXT_SIZE_DEFAULT;
}


static const gchar*
bb_text_tool_panel_get_text(BbTextControl *text_control)
{
    return "hello";
}


static BbTextVisibility
bb_text_tool_panel_get_visibility(BbTextControl *text_control)
{
    return BB_TEXT_VISIBILITY_DEFAULT;
}


static void
bb_text_tool_panel_text_control_init(BbTextControlInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_alignment = bb_text_tool_panel_get_alignment;
    iface->get_color = bb_text_tool_panel_get_color;
    iface->get_presentation = bb_text_tool_panel_get_presentation;
    iface->get_rotation = bb_text_tool_panel_get_rotation;
    iface->get_size = bb_text_tool_panel_get_size;
    iface->get_text = bb_text_tool_panel_get_text;
    iface->get_visibility = bb_text_tool_panel_get_visibility;
}

// endregion

// region From BbToolFactory Interface

static BbDrawingTool*
bb_text_tool_panel_select_tool(BbToolFactory *factory, BbDrawingToolSupport *support)
{
    return bb_drawing_tool_support_select_text_tool(support, BB_TEXT_CONTROL(factory));
}


static void
bb_text_tool_panel_tool_factory_init(BbToolFactoryInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->select_tool = bb_text_tool_panel_select_tool;
}

// endregion

static void
bb_text_tool_panel_class_init(BbTextToolPanelClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(G_IS_OBJECT_CLASS(object_class));

    object_class->dispose = bb_text_tool_panel_dispose;
    object_class->finalize = bb_text_tool_panel_finalize;
    object_class->get_property = bb_text_tool_panel_get_property;
    object_class->set_property = bb_text_tool_panel_set_property;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/bbtexttoolpanel.ui"
        );

    properties[PROP_ALIGNMENT] = bb_object_class_override_property(
        object_class,
        PROP_ALIGNMENT,
        "alignment"
        );

    properties[PROP_COLOR] = bb_object_class_override_property(
        object_class,
        PROP_COLOR,
        "color"
        );

    properties[PROP_PRESENTATION] = bb_object_class_override_property(
        object_class,
        PROP_PRESENTATION,
        "presentation"
        );

    properties[PROP_ROTATION] = bb_object_class_override_property(
        object_class,
        PROP_ROTATION,
        "rotation"
        );

    properties[PROP_SIZE] = bb_object_class_override_property(
        object_class,
        PROP_SIZE,
        "size"
        );

    properties[PROP_TEXT] = bb_object_class_override_property(
        object_class,
        PROP_TEXT,
        "text"
        );

    properties[PROP_VISIBILITY] = bb_object_class_override_property(
        object_class,
        PROP_VISIBILITY,
        "visibility"
        );
}


static void
bb_text_tool_panel_dispose(GObject *object)
{
}


static void
bb_text_tool_panel_finalize(GObject *object)
{
}


static void
bb_text_tool_panel_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_text_tool_panel_init(BbTextToolPanel *panel)
{
    gtk_widget_init_template(GTK_WIDGET(panel));
}


__attribute__((constructor)) void
bb_text_tool_panel_register()
{
    bb_text_tool_panel_get_type();
}


static void
bb_text_tool_panel_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
