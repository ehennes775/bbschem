/*
 * bbsch
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
#include <src/lib/bbadjustablelinestyle.h>
#include "bbmainwindow.h"
#include "bblinestyleeditor.h"
#include "bbpropertycombobox.h"
#include "bbschematicwindow.h"
#include "bbint32combobox.h"


enum
{
    PROP_0,
    PROP_MAIN_WINDOW
};


struct _BbLineStyleEditor
{
    GtkExpander parent;

    BbMainWindow *main_window;

    BbPropertyComboBox *cap_type_combo;
    BbInt32ComboBox *dash_length_combo;
    BbInt32ComboBox *dash_space_combo;
    BbPropertyComboBox *dash_type_combo;
    BbInt32ComboBox *line_width_combo;
};


G_DEFINE_TYPE(BbLineStyleEditor, bb_line_style_editor, GTK_TYPE_EXPANDER)


static void
bb_line_style_editor_apply_cap_type(BbPropertyComboBox *combo, BbLineStyleEditor *editor);

static void
bb_line_style_editor_apply_cap_type_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_line_style_editor_apply_dash_length(BbInt32ComboBox *combo, BbLineStyleEditor *editor);

static void
bb_line_style_editor_apply_dash_length_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_line_style_editor_apply_dash_space(BbInt32ComboBox *combo, BbLineStyleEditor *editor);

static void
bb_line_style_editor_apply_dash_space_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_line_style_editor_apply_dash_type(BbPropertyComboBox *combo, BbLineStyleEditor *editor);

static void
bb_line_style_editor_apply_dash_type_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_line_style_editor_apply_line_width(BbInt32ComboBox *combo, BbLineStyleEditor *editor);

static void
bb_line_style_editor_apply_line_width_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_line_style_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec);

static void
bb_line_style_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec);

static void
bb_line_style_editor_update(BbLineStyleEditor *editor);


/**
 * Apply a cap type to the selection
 *
 * @param combo The cap type combo box widget
 * @param editor A fill style editor
 */
static void
bb_line_style_editor_apply_cap_type(BbPropertyComboBox *combo, BbLineStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->cap_type_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_line_style_editor_apply_cap_type_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))  // FIX ME
        );
}


/**
 * Apply a new cap type to an individual item
 *
 * @param item A schematic item
 * @param user_data The cap type
 */
static void
bb_line_style_editor_apply_cap_type_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_LINE_STYLE(item))
    {
        bb_adjustable_line_style_set_cap_type(
            BB_ADJUSTABLE_LINE_STYLE(item),
            GPOINTER_TO_INT(user_data)
            );
    }
}


/**
 * Apply a dash length to the selection
 *
 * @param combo The dash length combo box widget
 * @param editor A fill style editor
 */
static void
bb_line_style_editor_apply_dash_length(BbInt32ComboBox *combo, BbLineStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->dash_length_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_line_style_editor_apply_dash_length_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))
        );
}


/**
 * Apply a new dash length to an individual item
 *
 * @param item A schematic item
 * @param user_data The dash length
 */
static void
bb_line_style_editor_apply_dash_length_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_LINE_STYLE(item))
    {
        bb_adjustable_line_style_set_dash_length(
            BB_ADJUSTABLE_LINE_STYLE(item),
            GPOINTER_TO_INT(user_data)
            );
    }
}
/**
 * Apply a dash space to the selection
 *
 * @param combo The dash space combo box widget
 * @param editor A fill style editor
 */
static void
bb_line_style_editor_apply_dash_space(BbInt32ComboBox *combo, BbLineStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->dash_space_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_line_style_editor_apply_dash_space_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))
        );
}


/**
 * Apply a new dash space to an individual item
 *
 * @param item A schematic item
 * @param user_data The dash space
 */
static void
bb_line_style_editor_apply_dash_space_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_LINE_STYLE(item))
    {
        bb_adjustable_line_style_set_dash_space(
            BB_ADJUSTABLE_LINE_STYLE(item),
            GPOINTER_TO_INT(user_data)
            );
    }
}


/**
 * Apply a dash type to the selection
 *
 * @param combo The dash type combo box widget
 * @param editor A fill style editor
 */
static void
bb_line_style_editor_apply_dash_type(BbPropertyComboBox *combo, BbLineStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->dash_type_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_line_style_editor_apply_dash_type_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))  // FIXME
        );
}


/**
 * Apply a new dash type to an individual item
 *
 * @param item A schematic item
 * @param user_data The dash type
 */
static void
bb_line_style_editor_apply_dash_type_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_LINE_STYLE(item))
    {
        bb_adjustable_line_style_set_dash_type(
            BB_ADJUSTABLE_LINE_STYLE(item),
            GPOINTER_TO_INT(user_data)
            );
    }
}


/**
 * Apply a line width to the selection
 *
 * @param combo The line width combo box widget
 * @param editor A fill style editor
 */
static void
bb_line_style_editor_apply_line_width(BbInt32ComboBox *combo, BbLineStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->line_width_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_line_style_editor_apply_line_width_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))
        );
}


/**
 * Apply a new line width to an individual item
 *
 * @param item A schematic item
 * @param user_data The line width
 */
static void
bb_line_style_editor_apply_line_width_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_LINE_STYLE(item))
    {
        bb_adjustable_line_style_set_line_width(
            BB_ADJUSTABLE_LINE_STYLE(item),
            GPOINTER_TO_INT(user_data)
            );
    }
}


static void
bb_line_style_editor_class_init(BbLineStyleEditorClass *class)
{
    G_OBJECT_CLASS(class)->get_property = bb_line_style_editor_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_line_style_editor_set_property;

    g_object_class_install_property(
        G_OBJECT_CLASS(class),
        PROP_MAIN_WINDOW,
        g_param_spec_object(
            "main-window",
            "",
            "",
            BB_TYPE_MAIN_WINDOW,
            G_PARAM_READWRITE
            )
        );

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bblinestyleeditor.ui"
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_line_style_editor_apply_cap_type
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_line_style_editor_apply_dash_length
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_line_style_editor_apply_dash_space
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_line_style_editor_apply_dash_type
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_line_style_editor_apply_line_width
        );
}


BbMainWindow*
bb_line_style_editor_get_main_window(BbLineStyleEditor *editor)
{
    g_return_val_if_fail(editor != NULL, NULL);

    return editor->main_window;
}


static void
bb_line_style_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec)
{
    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            g_value_set_object(value, bb_line_style_editor_get_main_window(BB_LINE_STYLE_EDITOR(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}


static void
bb_line_style_editor_init(BbLineStyleEditor *editor)
{
    gtk_widget_init_template(GTK_WIDGET(editor));
}


__attribute__((constructor)) void
bb_line_style_editor_register()
{
    bb_line_style_editor_get_type();
}


void
bb_line_style_editor_set_main_window(BbLineStyleEditor *editor, BbMainWindow *window)
{
    g_return_if_fail(editor != NULL);

    if (editor->main_window != NULL)
    {
        g_signal_handlers_disconnect_by_func(
            editor->main_window,
            G_CALLBACK(bb_line_style_editor_update),
            editor
            );

        g_object_unref(editor->main_window);
    }

    editor->main_window = window;

    if (editor->main_window != NULL)
    {
        g_object_ref(editor->main_window);

        g_signal_connect(
            editor->main_window,
            "update",
            G_CALLBACK(bb_line_style_editor_update),
            editor
          );
    }
}


static void
bb_line_style_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec)
{
    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            bb_line_style_editor_set_main_window(BB_LINE_STYLE_EDITOR(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}


static void
bb_line_style_editor_update(BbLineStyleEditor *editor)
{
    g_message("Update line style");
}
