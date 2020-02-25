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
#include "bbmainwindow.h"
#include "bbtextpropertyeditor.h"
#include "bbpropertycombobox.h"
#include "bbschematicwindow.h"


enum
{
    PROP_0,
    PROP_MAIN_WINDOW
};


struct _BbTextPropertyEditor
{
    GtkExpander parent;

    BbMainWindow *main_window;
};

G_DEFINE_TYPE(BbTextPropertyEditor, bb_text_property_editor, GTK_TYPE_EXPANDER);


static void
bb_text_property_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec);

static void
bb_text_property_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec);

static void
bb_text_property_editor_update(BbTextPropertyEditor *editor);

static void
bb_text_property_editor_apply(BbPropertyComboBox *combo, BbTextPropertyEditor *editor)
{
    BbSchematicWindow *window = BB_SCHEMATIC_WINDOW(
        bb_main_window_get_current_document_window(editor->main_window)
    );

    if (window != NULL)
    {
        bb_schematic_window_apply_property(window, "text-properties");
    }
}


static void
bb_text_property_editor_class_init(BbTextPropertyEditorClass *class)
{
    G_OBJECT_CLASS(class)->get_property = bb_text_property_editor_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_text_property_editor_set_property;

    g_object_class_install_property(
        G_OBJECT_CLASS(class),
        PROP_MAIN_WINDOW,
        g_param_spec_object(
            "main-window",
            "",
            "",
            BB_MAIN_WINDOW_TYPE,
            G_PARAM_READWRITE
            )
        );

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbtextpropertyeditor.ui"
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_text_property_editor_apply
        );
}


BbMainWindow*
bb_text_property_editor_get_main_window(BbTextPropertyEditor *editor)
{
    g_return_val_if_fail(editor != NULL, NULL);

    return editor->main_window;
}


static void
bb_text_property_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec)
{
    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            g_value_set_object(value, bb_text_property_editor_get_main_window(BB_TEXT_PROPERTY_EDITOR(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}


static void
bb_text_property_editor_init(BbTextPropertyEditor *editor)
{
    gtk_widget_init_template(GTK_WIDGET(editor));
}


__attribute__((constructor)) void
bb_text_property_editor_register()
{
    bb_text_property_editor_get_type();
}


void
bb_text_property_editor_set_main_window(BbTextPropertyEditor *editor, BbMainWindow *window)
{
    g_return_if_fail(editor != NULL);

    if (editor->main_window != NULL)
    {
        g_signal_handlers_disconnect_by_func(
            editor->main_window,
            G_CALLBACK(bb_text_property_editor_update),
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
            G_CALLBACK(bb_text_property_editor_update),
            editor
            );
    }
}


static void
bb_text_property_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec)
{
    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            bb_text_property_editor_set_main_window(BB_TEXT_PROPERTY_EDITOR(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}


static void
bb_text_property_editor_update(BbTextPropertyEditor *editor)
{
    g_message("Update text properties");
}

