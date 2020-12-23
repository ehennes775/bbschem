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
#include <bbextensions.h>
#include "bbdocumentwindowtab.h"


enum
{
    PROP_0,
    PROP_DOCUMENT_WINDOW
};


struct _BbDocumentWindowTab
{
    GtkBox parent;

    BbDocumentWindow *document_window;

    GtkLabel *modified_label;
    GtkLabel *changed_label;
    GtkLabel *name_label;
};

G_DEFINE_TYPE(BbDocumentWindowTab, bb_document_window_tab, GTK_TYPE_BOX);


static void
bb_document_window_tab_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_document_window_tab_notify(GObject *unused, GParamSpec *pspec, BbDocumentWindowTab *tab);

static void
bb_document_window_tab_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static void
bb_document_window_tab_class_init(BbDocumentWindowTabClass *klasse)
{
    G_OBJECT_CLASS(klasse)->get_property = bb_document_window_tab_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_document_window_tab_set_property;

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
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
bb_document_window_tab_init(BbDocumentWindowTab *tab)
{
    gtk_widget_init_template(GTK_WIDGET(tab));
}


BbDocumentWindow*
bb_document_window_tab_get_document_window(BbDocumentWindowTab *tab)
{
    g_return_val_if_fail(tab != NULL, NULL);

    return tab->document_window;
}


static void
bb_document_window_tab_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    BbDocumentWindowTab *tab = BB_DOCUMENT_WINDOW_TAB(object);
    g_return_if_fail(tab != NULL);

    switch (property_id)
    {
        case PROP_DOCUMENT_WINDOW:
            g_value_set_object(value, tab->document_window);
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


BbDocumentWindowTab*
bb_document_window_tab_new(BbDocumentWindow *window)
{
    BbDocumentWindowTab *tab = g_object_new(
        BB_TYPE_DOCUMENT_WINDOW_TAB,
        NULL
        );

    g_signal_connect(
        tab,
        "notify::document-window",
        G_CALLBACK(bb_document_window_tab_notify),
        tab
        );

    bb_document_window_tab_set_document_window(tab, window);

    return tab;
}


static void
bb_document_window_tab_notify(GObject *unused, GParamSpec *pspec, BbDocumentWindowTab *tab)
{
    g_return_if_fail(tab != NULL);
    g_return_if_fail(tab->document_window != NULL);
    g_return_if_fail(tab->name_label != NULL);

    gtk_label_set_text(tab->name_label, bb_document_window_get_tab(tab->document_window));
}


__attribute__((constructor)) void
bb_document_window_tab_register()
{
    bb_document_window_tab_get_type();
}


void
bb_document_window_tab_set_document_window(BbDocumentWindowTab *tab, BbDocumentWindow *window)
{
    g_return_if_fail(tab != NULL);

    if (tab->document_window != window)
    {
        if (tab->document_window != NULL)
        {
        }

        g_set_object(&tab->document_window, window);

        if (tab->document_window != NULL)
        {
        }

        g_object_notify(G_OBJECT(tab), "document-window");
    }
}


static void
bb_document_window_tab_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    BbDocumentWindowTab *tab = BB_DOCUMENT_WINDOW_TAB(object);
    g_return_if_fail(tab != NULL);

    switch (property_id)
    {
        case PROP_DOCUMENT_WINDOW:
            bb_document_window_tab_set_document_window(tab, g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
