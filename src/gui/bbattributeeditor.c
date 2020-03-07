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
#include "bbattributeeditor.h"


enum
{
    PROP_0,
    PROP_DOCUMENT_WINDOW
};


struct _BbAttributeEditor
{
    GtkBox parent;

    BbDocumentWindow *document_window;
};


G_DEFINE_TYPE(BbAttributeEditor, bb_attribute_editor, GTK_TYPE_BOX);


static void
bb_attribute_editor_dispose(GObject *object);

static void
bb_attribute_editor_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_attribute_editor_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static void
bb_attribute_editor_class_init(BbAttributeEditorClass *class)
{

    G_OBJECT_CLASS(class)->dispose = bb_attribute_editor_dispose;
    G_OBJECT_CLASS(class)->get_property = bb_attribute_editor_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_attribute_editor_set_property;

    g_object_class_install_property(
        G_OBJECT_CLASS(class),
        PROP_DOCUMENT_WINDOW,
        g_param_spec_object(
            "document-window",
            "",
            "",
            BB_TYPE_DOCUMENT_WINDOW,
            G_PARAM_READWRITE
            )
        );

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbattributeeditor.ui"
        );
}


static void
bb_attribute_editor_dispose(GObject *object)
{
    BbAttributeEditor *editor = BB_ATTRIBUTE_EDITOR(object);
    g_return_if_fail(editor != NULL);

    g_clear_object(&editor->document_window);
}


BbDocumentWindow*
bb_attribute_editor_get_document_window(BbAttributeEditor *editor)
{
    g_return_val_if_fail(editor != NULL, NULL);

    return editor->document_window;
}


static void
bb_attribute_editor_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_DOCUMENT_WINDOW:
            g_value_set_object(value, bb_attribute_editor_get_document_window(BB_ATTRIBUTE_EDITOR(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_attribute_editor_init(BbAttributeEditor *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_attribute_editor_register()
{
    bb_attribute_editor_get_type();
}


void
bb_attribute_editor_set_document_window(BbAttributeEditor *editor, BbDocumentWindow *window)
{
    g_return_if_fail(editor != NULL);

    g_set_object(&editor->document_window, window);
}


static void
bb_attribute_editor_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_DOCUMENT_WINDOW:
            bb_attribute_editor_set_document_window(BB_ATTRIBUTE_EDITOR(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
