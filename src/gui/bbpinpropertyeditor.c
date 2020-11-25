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
#include <bbextensions.h>
#include <src/lib/bbvaluecount.h>
#include "bbmainwindow.h"
#include "bbpinpropertyeditor.h"
#include "bbpropertycombobox.h"
#include "bbschematicwindow.h"


enum
{
    PROP_0,
    PROP_MAIN_WINDOW
};


struct _BbPinPropertyEditor
{
    GtkExpander parent;

    BbMainWindow *main_window;

    BbPropertyComboBox *pin_type_combo;
};


G_DEFINE_TYPE(BbPinPropertyEditor, bb_pin_property_editor, GTK_TYPE_EXPANDER)


static void
bb_pin_property_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec);

static void
bb_pin_property_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec);

static void
bb_pin_property_editor_update(BbMainWindow *main_window, BbPinPropertyEditor *editor);

static gboolean
bb_pin_property_editor_update_lambda(BbSchematicItem *item, gpointer user_data);

static void
bb_pin_property_editor_apply(BbPropertyComboBox *combo, BbPinPropertyEditor *editor)
{
    g_return_if_fail(editor != NULL);

    if (editor->main_window != NULL)
    {
        GVariant *color = g_variant_new_int32(10);

        g_action_group_activate_action(
            G_ACTION_GROUP(editor->main_window),
            "apply-pin-type",
            color
            );
    }
}


static void
bb_pin_property_editor_class_init(BbPinPropertyEditorClass *class)
{
    G_OBJECT_CLASS(class)->get_property = bb_pin_property_editor_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_pin_property_editor_set_property;

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
        "/com/github/ehennes775/bbsch/gui/bbpinpropertyeditor.ui"
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_pin_property_editor_apply
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbPinPropertyEditor,
        pin_type_combo
        );
}


BbMainWindow*
bb_pin_property_editor_get_main_window(BbPinPropertyEditor *editor)
{
    g_return_val_if_fail(editor != NULL, NULL);

    return editor->main_window;
}


static void
bb_pin_property_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec)
{
    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            g_value_set_object(value, bb_pin_property_editor_get_main_window(BB_PIN_PROPERTY_EDITOR(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}


static void
bb_pin_property_editor_init(BbPinPropertyEditor *editor)
{
    gtk_widget_init_template(GTK_WIDGET(editor));
}


__attribute__((constructor)) void
bb_pin_property_editor_register()
{
    bb_pin_property_editor_get_type();
}


void
bb_pin_property_editor_set_main_window(BbPinPropertyEditor *editor, BbMainWindow *window)
{
    g_return_if_fail(editor != NULL);

    if (editor->main_window != NULL)
    {
        g_signal_handlers_disconnect_by_func(
            editor->main_window,
            G_CALLBACK(bb_pin_property_editor_update),
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
            G_CALLBACK(bb_pin_property_editor_update),
            editor
            );
    }
}


static void
bb_pin_property_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec)
{
    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            bb_pin_property_editor_set_main_window(BB_PIN_PROPERTY_EDITOR(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, param_id, pspec);
    }
}


static void
bb_pin_property_editor_update(BbMainWindow *main_window, BbPinPropertyEditor *editor)
{
    BbValueCount count;
    GHashTable *table;
    GtkWidget *window;

    g_message("Update pin type property");

    g_return_if_fail(editor != NULL);
    g_return_if_fail(main_window != NULL);
    g_return_if_fail(editor->main_window == main_window);

    table = g_hash_table_new(NULL, NULL);
    window = bb_main_window_get_current_document_window(main_window);

    if (BB_IS_SCHEMATIC_WINDOW(window))
    {
        bb_schematic_window_query_selection(
            BB_SCHEMATIC_WINDOW(window),
            bb_pin_property_editor_update_lambda,
            table
            );
    }

    count = bb_value_count_from_count(g_hash_table_size(table));

    if (bb_value_count_inconsistent(count))
    {
        // bb_pin_type_combo_set_pin_type(editor->pin_type_combo, -1);
    }
    else
    {
        // bb_pin_type_combo_set_pin_type(
        //     editor->pin_type_combo,
        //     GPOINTER_TO_INT(g_hash_table_find(table, always, NULL))
        //     );
    }

    gtk_widget_set_sensitive(
        GTK_WIDGET(editor->pin_type_combo),
        bb_value_count_sensitive(count)
        );
}


static gboolean
bb_pin_property_editor_update_lambda(BbSchematicItem *item, gpointer user_data)
{
    GHashTable *table = (GHashTable*) user_data;

    g_return_val_if_fail(table != NULL, FALSE);

    //if (BB_IS_PIN(item))
    //{
    //    g_hash_table_add(
    //        table,
    //        GINT_TO_POINTER(bb_pin_get_pin_type(BB_PIN(item)))
    //    );
    //}

    return g_hash_table_size(table) < BB_VALUE_COUNT_MANY;
}
