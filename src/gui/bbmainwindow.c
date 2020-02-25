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
#include "bbapplication.h"
#include "bbdocumentwindowtab.h"
#include "bbschematicwindow.h"

struct _BbMainWindow
{
    GtkApplicationWindow parent;

    BbDocumentWindow *current_page;
    GtkNotebook *document_notebook;
};

G_DEFINE_TYPE(BbMainWindow, bb_main_window, GTK_TYPE_APPLICATION_WINDOW);


static void
bb_main_window_dispose(GObject *object);

static void
bb_main_window_notify_page_num(BbMainWindow *window, GParamSpec *pspec, GtkNotebook *notebook);

static void
bb_main_window_page_added(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook);

static void
bb_main_window_page_removed(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook);

static void
bb_main_window_update(GtkWidget *child, BbMainWindow *window);


void
bb_main_window_add_page(BbMainWindow *window, BbDocumentWindow *page)
{
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->document_notebook != NULL);
    g_return_if_fail(page != NULL);

    gtk_notebook_append_page(
        window->document_notebook,
        GTK_WIDGET(page),
        gtk_label_new("Hello") //GTK_WIDGET(bb_document_window_tab_new(page))
        );
}


static void
bb_main_window_class_init(BbMainWindowClass *class)
{
    G_OBJECT_CLASS(class)->dispose = bb_main_window_dispose;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbmainwindow.ui"
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbMainWindow,
        document_notebook
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_main_window_notify_page_num
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_main_window_page_added
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_main_window_page_removed
        );

    g_signal_new(
        "update",
        G_OBJECT_CLASS_TYPE(class),
        (GSignalFlags) 0,
        0,
        NULL,
        NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE,
        0
        );
}


static void
bb_main_window_dispose(GObject *object)
{
    BbMainWindow *window = BB_MAIN_WINDOW(object);

    g_return_if_fail(window != NULL);

    g_set_object(&window->current_page, NULL);
}


GtkWidget*
bb_main_window_get_current_document_window(BbMainWindow* window)
{
    g_return_val_if_fail(window != NULL, NULL);
    g_return_val_if_fail(window->document_notebook != NULL, NULL);

    gint page_num = gtk_notebook_get_current_page(window->document_notebook);
    gboolean empty = (page_num < 0);

    return empty ? NULL : gtk_notebook_get_nth_page(window->document_notebook, page_num);
}


static void
bb_main_window_init(BbMainWindow *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));

    bb_main_window_add_page(window, g_object_new(BB_TYPE_SCHEMATIC_WINDOW, NULL));
}


BbMainWindow*
bb_main_window_new(BbApplication *application)
{
    return g_object_new(
        BB_MAIN_WINDOW_TYPE,
        "application", application,
        NULL
        );
}


static void
bb_main_window_notify_page_num(BbMainWindow *window, GParamSpec *pspec, GtkNotebook *notebook)
{
    g_return_if_fail(window != NULL);

    GtkWidget *temp_page = bb_main_window_get_current_document_window(window);
    BbDocumentWindow *next_page = BB_IS_DOCUMENT_WINDOW(temp_page) ? BB_DOCUMENT_WINDOW(temp_page) : NULL;

    if (window->current_page != next_page)
    {
        if (window->current_page != NULL)
        {
            bb_document_window_detach_actions(window->current_page, G_ACTION_MAP(window));
        }

        g_set_object(&window->current_page, next_page);

        if (window->current_page != NULL)
        {
            bb_document_window_attach_actions(window->current_page, G_ACTION_MAP(window));
        }

        g_signal_emit_by_name(window, "update");
    }
}


static void
bb_main_window_page_added(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook)
{
    g_return_if_fail(child != NULL);
    g_return_if_fail(notebook != NULL);
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->document_notebook == notebook);

    g_signal_connect(
        child,
        "update",
        G_CALLBACK(bb_main_window_update),
        window
        );
}


static void
bb_main_window_page_removed(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook)
{
    g_return_if_fail(child != NULL);
    g_return_if_fail(notebook != NULL);
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->document_notebook == notebook);

    g_signal_handlers_disconnect_by_func(
        child,
        G_CALLBACK(bb_main_window_update),
        window
        );
}


static void
bb_main_window_update(GtkWidget* child, BbMainWindow *window)
{
    g_return_if_fail(window != NULL);

    g_signal_emit_by_name(window, "update");
}
