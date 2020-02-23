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
#include "bbdocumentwindowtab.h"


struct _BbDocumentWindowTab
{
    GtkWidget parent;

    GtkLabel *modified_label;
    GtkLabel *changed_label;
    GtkLabel *name_label;
};


G_DEFINE_TYPE(BbDocumentWindowTab, bb_document_window_tab, GTK_TYPE_BOX);


static void
bb_document_window_tab_class_init(BbDocumentWindowTabClass *klasse)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/bbdocumentwindowtab.ui"
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbDocumentWindowTab,
        modified_label
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbDocumentWindowTab,
        changed_label
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbDocumentWindowTab,
        name_label
        );
}


static void
bb_document_window_tab_init(BbDocumentWindowTab *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


BbDocumentWindowTab*
bb_document_window_tab_new(BbDocumentWindow *window)
{
    return g_object_new(
        BB_TYPE_DOCUMENT_WINDOW_TAB,
        NULL
        );
}


__attribute__((constructor)) void
bb_document_window_tab_register()
{
    bb_document_window_tab_get_type();
}
