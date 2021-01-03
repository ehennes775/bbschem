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
#include <bbcolor.h>
#include "bbtexttoolpanel.h"
#include "bbtoolfactory.h"
#include "bbtextcontrol.h"
#include "bbint32combobox.h"
#include "bbalignmentcombobox.h"
#include "bbcolorcombobox.h"


enum
{
    PROP_0,

    /* From BbTextControl Interface */
    
    PROP_ALIGNMENT,
    PROP_COLOR,
    PROP_PRESENTATION,
    PROP_ROTATION,
    PROP_SIZE,
    PROP_TEXT,
    PROP_VISIBILITY,

    N_PROPERTIES
};


struct _BbTextToolPanel
{
    GtkBox parent;

    /**
     * @brief A cached dynamically allocated string returned from the GtkTextBuffer
     */
    gchar *allocated_text;

    /**
     * @brief The provided text rotation when the contents of the GUI widget are invalid
     */
    int last_valid_text_rotation;

    /**
     * @brief The provided text size when the contents of the GUI widget are invalid
     */
    int last_valid_text_size;

    BbAlignmentComboBox *text_alignment_combo;

    BbColorComboBox *text_color_combo;

    BbInt32ComboBox *text_rotation_combo;

    BbInt32ComboBox *text_size_combo;

    GtkTextView *text_view;
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

static void
bb_text_tool_panel_text_alignment_changed(BbAlignmentComboBox *combo, BbTextToolPanel *panel);

static void
bb_text_tool_panel_text_color_changed(BbColorComboBox *combo, BbTextToolPanel *panel);

static void
bb_text_tool_panel_text_rotation_changed(BbInt32ComboBox *combo, BbTextToolPanel *panel);

static void
bb_text_tool_panel_text_size_changed(BbInt32ComboBox *combo, BbTextToolPanel *panel);

static void
bb_text_tool_panel_text_view_changed(GtkTextBuffer *buffer, BbTextToolPanel *panel);


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
    BbTextToolPanel *panel = BB_TEXT_TOOL_PANEL(text_control);
    g_return_val_if_fail(BB_IS_TEXT_TOOL_PANEL(panel), BB_TEXT_ALIGNMENT_DEFAULT);

    BbTextAlignment alignment = bb_alignment_combo_box_get_alignment(panel->text_alignment_combo);

    g_return_val_if_fail(bb_text_alignment_is_valid(alignment), BB_TEXT_ALIGNMENT_DEFAULT);

    return alignment;
}


static int
bb_text_tool_panel_get_color(BbTextControl *text_control)
{
    BbTextToolPanel *panel = BB_TEXT_TOOL_PANEL(text_control);
    g_return_val_if_fail(BB_IS_TEXT_TOOL_PANEL(panel), BB_COLOR_TEXT);

    int color = bb_color_combo_box_get_color(panel->text_color_combo);

    g_return_val_if_fail(bb_color_is_valid(color), BB_COLOR_TEXT);

    return color;
}


static BbTextPresentation
bb_text_tool_panel_get_presentation(BbTextControl *text_control)
{
    BbTextToolPanel *panel = BB_TEXT_TOOL_PANEL(text_control);
    g_return_val_if_fail(BB_IS_TEXT_TOOL_PANEL(panel), BB_TEXT_SIZE_DEFAULT);

    return BB_TEXT_PRESENTATION_DEFAULT;
}


static int
bb_text_tool_panel_get_rotation(BbTextControl *text_control)
{
    BbTextToolPanel *panel = BB_TEXT_TOOL_PANEL(text_control);
    g_return_val_if_fail(BB_IS_TEXT_TOOL_PANEL(panel), BB_TEXT_SIZE_DEFAULT);
    g_return_val_if_fail(BB_IS_INT32_COMBO_BOX(panel->text_rotation_combo), BB_TEXT_SIZE_DEFAULT);

    int text_rotation = bb_int32_combo_box_get_value(panel->text_rotation_combo);

    //if (bb_text_rotation_is_valid(text_rotation)) TODO if something is invalid in the widget
    {
        panel->last_valid_text_rotation = text_rotation;
    }

    return panel->last_valid_text_rotation;
}


static int
bb_text_tool_panel_get_size(BbTextControl *text_control)
{
    BbTextToolPanel *panel = BB_TEXT_TOOL_PANEL(text_control);
    g_return_val_if_fail(BB_IS_TEXT_TOOL_PANEL(panel), BB_TEXT_SIZE_DEFAULT);
    g_return_val_if_fail(BB_IS_INT32_COMBO_BOX(panel->text_size_combo), BB_TEXT_SIZE_DEFAULT);

    int text_size = bb_int32_combo_box_get_value(panel->text_size_combo);

    if (bb_text_size_is_valid(text_size))
    {
        panel->last_valid_text_size = text_size;
    }

    return panel->last_valid_text_size;
}


static const gchar*
bb_text_tool_panel_get_text(BbTextControl *text_control)
{
    BbTextToolPanel *panel = BB_TEXT_TOOL_PANEL(text_control);
    g_return_val_if_fail(BB_IS_TEXT_TOOL_PANEL(panel), "Error");
    g_return_val_if_fail(GTK_IS_TEXT_VIEW(panel->text_view), "Error");

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(panel->text_view);
    g_return_val_if_fail(GTK_IS_TEXT_BUFFER(buffer), "Error");

    GtkTextIter iter0;
    GtkTextIter iter1;

    gtk_text_buffer_get_bounds(buffer, &iter0, &iter1);

    gchar* text = gtk_text_iter_get_text(&iter0, &iter1);

    if (text != NULL)
    {
        g_free(panel->allocated_text);

        panel->allocated_text = text;
    }

    return panel->allocated_text;
}


static BbTextVisibility
bb_text_tool_panel_get_visibility(BbTextControl *text_control)
{
    g_warn_if_fail(BB_IS_TEXT_TOOL_PANEL(text_control));

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

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbTextToolPanel,
        text_alignment_combo
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbTextToolPanel,
        text_color_combo
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbTextToolPanel,
        text_rotation_combo
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbTextToolPanel,
        text_size_combo
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbTextToolPanel,
        text_view
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
    BbTextToolPanel *panel = BB_TEXT_TOOL_PANEL(object);
    g_return_if_fail(BB_IS_TEXT_TOOL_PANEL(panel));

    g_free(g_steal_pointer(&panel->allocated_text));
}


static void
bb_text_tool_panel_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ALIGNMENT:
            g_value_set_int(value, bb_text_tool_panel_get_alignment(BB_TEXT_CONTROL(object)));
            break;

        case PROP_COLOR:
            g_value_set_int(value, bb_text_tool_panel_get_color(BB_TEXT_CONTROL(object)));
            break;

        case PROP_PRESENTATION:
            g_value_set_int(value, bb_text_tool_panel_get_presentation(BB_TEXT_CONTROL(object)));
            break;

        case PROP_ROTATION:
            g_value_set_int(value, bb_text_tool_panel_get_rotation(BB_TEXT_CONTROL(object)));
            break;

        case PROP_SIZE:
            g_value_set_int(value, bb_text_tool_panel_get_size(BB_TEXT_CONTROL(object)));
            break;

        case PROP_TEXT:
            g_value_set_string(value, bb_text_tool_panel_get_text(BB_TEXT_CONTROL(object)));
            break;

        case PROP_VISIBILITY:
            g_value_set_int(value, bb_text_tool_panel_get_visibility(BB_TEXT_CONTROL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_text_tool_panel_init(BbTextToolPanel *panel)
{
    gtk_widget_init_template(GTK_WIDGET(panel));

    panel->allocated_text = g_strdup("Empty");
    panel->last_valid_text_rotation = 0;
    panel->last_valid_text_size = BB_TEXT_SIZE_DEFAULT;

    g_signal_connect(
        panel->text_alignment_combo,
        "changed",
        G_CALLBACK(bb_text_tool_panel_text_alignment_changed),
        panel
        );

    g_signal_connect(
        panel->text_color_combo,
        "changed",
        G_CALLBACK(bb_text_tool_panel_text_color_changed),
        panel
        );

    g_signal_connect(
        panel->text_rotation_combo,
        "changed",
        G_CALLBACK(bb_text_tool_panel_text_rotation_changed),
        panel
        );

    g_signal_connect(
        panel->text_size_combo,
        "changed",
        G_CALLBACK(bb_text_tool_panel_text_size_changed),
        panel
        );

    g_signal_connect(
        gtk_text_view_get_buffer(panel->text_view),
        "changed",
        G_CALLBACK(bb_text_tool_panel_text_view_changed),
        panel
        );
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



static void
bb_text_tool_panel_text_alignment_changed(BbAlignmentComboBox *combo, BbTextToolPanel *panel)
{
    g_return_if_fail(BB_IS_ALIGNMENT_COMBO_BOX(combo));
    g_return_if_fail(BB_IS_TEXT_TOOL_PANEL(panel));

    g_object_notify_by_pspec(G_OBJECT(panel), properties[PROP_ALIGNMENT]);
}


static void
bb_text_tool_panel_text_color_changed(BbColorComboBox *combo, BbTextToolPanel *panel)
{
    g_return_if_fail(BB_IS_COLOR_COMBO_BOX(combo));
    g_return_if_fail(BB_IS_TEXT_TOOL_PANEL(panel));

    g_object_notify_by_pspec(G_OBJECT(panel), properties[PROP_COLOR]);
}


static void
bb_text_tool_panel_text_rotation_changed(BbInt32ComboBox *combo, BbTextToolPanel *panel)
{
    g_return_if_fail(BB_IS_INT32_COMBO_BOX(combo));
    g_return_if_fail(BB_IS_TEXT_TOOL_PANEL(panel));

    g_object_notify_by_pspec(G_OBJECT(panel), properties[PROP_ROTATION]);
}


static void
bb_text_tool_panel_text_size_changed(BbInt32ComboBox *combo, BbTextToolPanel *panel)
{
    g_return_if_fail(BB_IS_INT32_COMBO_BOX(combo));
    g_return_if_fail(BB_IS_TEXT_TOOL_PANEL(panel));

    g_object_notify_by_pspec(G_OBJECT(panel), properties[PROP_SIZE]);
}


static void
bb_text_tool_panel_text_view_changed(GtkTextBuffer *buffer, BbTextToolPanel *panel)
{
    g_return_if_fail(GTK_IS_TEXT_BUFFER(buffer));
    g_return_if_fail(BB_IS_TEXT_TOOL_PANEL(panel));

    g_object_notify_by_pspec(G_OBJECT(panel), properties[PROP_TEXT]);
}

