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
#include <src/lib/bbadjustablefillstyle.h>
#include "bbmainwindow.h"
#include "bbfillstyleeditor.h"
#include "bbint32combobox.h"
#include "bbpropertycombobox.h"
#include "bbschematicwindow.h"
#include "bbextensions.h"


enum
{
    PROP_0,
    PROP_MAIN_WINDOW
};


struct _BbFillStyleEditor
{
    GtkExpander parent;

    BbMainWindow *main_window;

    BbInt32ComboBox *fill_angle1_combo;
    BbInt32ComboBox *fill_pitch1_combo;
    BbPropertyComboBox *fill_type_combo;
    BbInt32ComboBox *fill_width_combo;
    BbInt32ComboBox *fill_angle2_combo;
    BbInt32ComboBox *fill_pitch2_combo;
};


static void
bb_fill_style_editor_apply_angle_1(BbInt32ComboBox *combo, BbFillStyleEditor *editor);

static void
bb_fill_style_editor_apply_angle_1_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_fill_style_editor_apply_angle_2(BbInt32ComboBox *combo, BbFillStyleEditor *editor);

static void
bb_fill_style_editor_apply_angle_2_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_fill_style_editor_apply_fill_type(BbPropertyComboBox *combo, BbFillStyleEditor *editor);

static void
bb_fill_style_editor_apply_fill_type_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_fill_style_editor_apply_fill_width(BbInt32ComboBox *combo, BbFillStyleEditor *editor);

static void
bb_fill_style_editor_apply_fill_width_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_fill_style_editor_apply_pitch_1(BbInt32ComboBox *combo, BbFillStyleEditor *editor);

static void
bb_fill_style_editor_apply_pitch_1_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_fill_style_editor_apply_pitch_2(BbInt32ComboBox *combo, BbFillStyleEditor *editor);

static void
bb_fill_style_editor_apply_pitch_2_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_fill_style_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec);

static void
bb_fill_style_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec);

static void
bb_fill_style_editor_update(BbFillStyleEditor *editor);


G_DEFINE_TYPE(BbFillStyleEditor, bb_fill_style_editor, GTK_TYPE_EXPANDER)


/**
 * Apply a fill angle to the selection
 *
 * @param combo The angle combo box widget
 * @param editor A fill style editor
 */
static void
bb_fill_style_editor_apply_angle_1(BbInt32ComboBox *combo, BbFillStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->fill_angle1_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_fill_style_editor_apply_angle_1_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))
        );
}


/**
 * Apply a new fill angle to an individual item
 *
 * @param item A schematic item
 * @param user_data The angle
 */
static void
bb_fill_style_editor_apply_angle_1_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_FILL_STYLE(item))
    {
        bb_adjustable_fill_style_set_fill_angle_1(
            BB_ADJUSTABLE_FILL_STYLE(item),
            GPOINTER_TO_INT(user_data)
            );
    }
}


/**
 * Apply a fill angle to the selection
 *
 * @param combo The angle combo box widget
 * @param editor A fill style editor
 */
static void
bb_fill_style_editor_apply_angle_2(BbInt32ComboBox *combo, BbFillStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->fill_angle2_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_fill_style_editor_apply_angle_2_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))
        );
}


/**
 * Apply a new fill angle to an individual item
 *
 * @param item A schematic item
 * @param user_data The angle
 */
static void
bb_fill_style_editor_apply_angle_2_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_FILL_STYLE(item))
    {
        bb_adjustable_fill_style_set_fill_angle_2(
            BB_ADJUSTABLE_FILL_STYLE(item),
            GPOINTER_TO_INT(user_data)
            );
    }
}


/**
 * Apply a fill type to the selection
 *
 * @param combo The fill type combo box widget
 * @param editor A fill style editor
 */
static void
bb_fill_style_editor_apply_fill_type(BbPropertyComboBox *combo, BbFillStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->fill_type_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_fill_style_editor_apply_fill_type_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))
        );
}


/**
 * Apply a new fill pitch to an individual item
 *
 * @param item A schematic item
 * @param user_data The pitch
 */
static void
bb_fill_style_editor_apply_fill_type_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_FILL_STYLE(item))
    {
        bb_adjustable_fill_style_set_fill_type(
            BB_ADJUSTABLE_FILL_STYLE(item),
            GPOINTER_TO_INT(user_data)
            );
    }
}


/**
 * Apply a fill width to the selection
 *
 * @param combo The width combo box widget
 * @param editor A fill style editor
 */
static void
bb_fill_style_editor_apply_fill_width(BbInt32ComboBox *combo, BbFillStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->fill_width_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_fill_style_editor_apply_fill_width_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))
    );
}


/**
 * Apply a new fill pitch to an individual item
 *
 * @param item A schematic item
 * @param user_data The pitch
 */
static void
bb_fill_style_editor_apply_fill_width_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_FILL_STYLE(item))
    {
        bb_adjustable_fill_style_set_fill_width(
            BB_ADJUSTABLE_FILL_STYLE(item),
            GPOINTER_TO_INT(user_data)
            );
    }
}

/**
 * Apply a fill pitch to the selection
 *
 * @param combo The pitch combo box widget
 * @param editor A fill style editor
 */
static void
bb_fill_style_editor_apply_pitch_1(BbInt32ComboBox *combo, BbFillStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->fill_pitch1_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_fill_style_editor_apply_pitch_1_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))
    );
}


/**
 * Apply a new fill pitch to an individual item
 *
 * @param item A schematic item
 * @param user_data The pitch
 */
static void
bb_fill_style_editor_apply_pitch_1_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_FILL_STYLE(item))
    {
        bb_adjustable_fill_style_set_fill_angle_1(
            BB_ADJUSTABLE_FILL_STYLE(item),
            GPOINTER_TO_INT(user_data)
        );
    }
}


/**
 * Apply a fill pitch to the selection
 *
 * @param combo The pitch combo box widget
 * @param editor A fill style editor
 */
static void
bb_fill_style_editor_apply_pitch_2(BbInt32ComboBox *combo, BbFillStyleEditor *editor)
{
    GtkWidget *window;

    g_return_if_fail(combo != NULL);
    g_return_if_fail(editor != NULL);
    g_return_if_fail(combo == editor->fill_pitch2_combo);

    window = bb_main_window_get_current_document_window(editor->main_window);

    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(window));

    bb_schematic_window_apply_selection(
        BB_SCHEMATIC_WINDOW(window),
        bb_fill_style_editor_apply_pitch_2_lambda,
        GINT_TO_POINTER(bb_int32_combo_box_get_value(combo))
    );
}


/**
 * Apply a new fill pitch to an individual item
 *
 * @param item A schematic item
 * @param user_data The pitch
 */
static void
bb_fill_style_editor_apply_pitch_2_lambda(BbSchematicItem *item, gpointer user_data)
{
    if (BB_IS_ADJUSTABLE_FILL_STYLE(item))
    {
        bb_adjustable_fill_style_set_fill_pitch_2(
            BB_ADJUSTABLE_FILL_STYLE(item),
            GPOINTER_TO_INT(user_data)
        );
    }
}


static void
bb_fill_style_editor_class_init(BbFillStyleEditorClass *class)
{
    G_OBJECT_CLASS(class)->get_property = bb_fill_style_editor_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_fill_style_editor_set_property;

    bb_object_class_install_property(
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
        "/com/github/ehennes775/bbsch/gui/bbfillstyleeditor.ui"
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_fill_style_editor_apply_angle_1
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_fill_style_editor_apply_angle_2
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_fill_style_editor_apply_fill_type
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_fill_style_editor_apply_fill_width
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_fill_style_editor_apply_pitch_1
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_fill_style_editor_apply_pitch_2
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbFillStyleEditor,
        fill_angle1_combo
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbFillStyleEditor,
        fill_angle2_combo
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbFillStyleEditor,
        fill_type_combo
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbFillStyleEditor,
        fill_width_combo
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbFillStyleEditor,
        fill_pitch1_combo
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbFillStyleEditor,
        fill_pitch2_combo
        );
}


BbMainWindow*
bb_fill_style_editor_get_main_window(BbFillStyleEditor *editor)
{
    g_return_val_if_fail(editor != NULL, NULL);

    return editor->main_window;
}


static void
bb_fill_style_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec)
{
    BbFillStyleEditor *editor = BB_FILL_STYLE_EDITOR(object);
    g_return_if_fail(editor != NULL);

    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            g_value_set_object(value, bb_fill_style_editor_get_main_window(BB_FILL_STYLE_EDITOR(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}


static void
bb_fill_style_editor_init(BbFillStyleEditor *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_fill_style_editor_register()
{
    bb_fill_style_editor_get_type();
}


void
bb_fill_style_editor_set_main_window(BbFillStyleEditor *editor, BbMainWindow *window)
{
    g_return_if_fail(editor != NULL);

    if (editor->main_window != NULL)
    {
        g_signal_handlers_disconnect_by_func(
            editor->main_window,
            G_CALLBACK(bb_fill_style_editor_update),
            editor
            );

        g_object_unref(editor->main_window);
    }

    editor->main_window = window;

#if 0
    bb_property_combo_box_set_action_group(
        BB_PROPERTY_COMBO_BOX(editor->fill_angle1_combo),
        G_ACTION_GROUP(editor->main_window)
        );

    bb_property_combo_box_set_action_group(
        BB_PROPERTY_COMBO_BOX(editor->fill_angle2_combo),
        G_ACTION_GROUP(editor->main_window)
        );

    bb_property_combo_box_set_action_group(
        BB_PROPERTY_COMBO_BOX(editor->fill_pitch1_combo),
        G_ACTION_GROUP(editor->main_window)
        );

    bb_property_combo_box_set_action_group(
        BB_PROPERTY_COMBO_BOX(editor->fill_pitch2_combo),
        G_ACTION_GROUP(editor->main_window)
        );
#endif

    if (editor->main_window != NULL)
    {
        g_object_ref(editor->main_window);

        g_signal_connect(
            editor->main_window,
            "update",
            G_CALLBACK(bb_fill_style_editor_update),
            editor
            );
    }
}


static void
bb_fill_style_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec)
{
    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            bb_fill_style_editor_set_main_window(BB_FILL_STYLE_EDITOR(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}


static void
bb_fill_style_editor_update(BbFillStyleEditor *editor)
{
    g_message("Update fill style");
}
