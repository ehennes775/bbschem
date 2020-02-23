#include <gtk/gtk.h>
#include "bbmainwindow.h"
#include "bbapplication.h"
//#include "bbdocumentwindowtab.h"

struct _BbMainWindow
{
    GtkApplicationWindow parent;

    GtkNotebook *document_notebook;
};


G_DEFINE_TYPE(BbMainWindow, bb_main_window, GTK_TYPE_APPLICATION_WINDOW);


void
bb_main_window_page_added(GtkNotebook *notebook, GtkWidget *child, guint page_num, BbMainWindow *window);

void
bb_main_window_page_removed(GtkNotebook *notebook, GtkWidget *child, guint page_num, BbMainWindow *window);


void
bb_main_window_add_page(BbMainWindow *window, BbDocumentWindow *page)
{
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->document_notebook != NULL);
    g_return_if_fail(page != NULL);

    gtk_notebook_append_page(
        window->document_notebook,
        GTK_WIDGET(page),
        GTK_WIDGET(bb_document_window_tab_new(page))
        );
}


static void
bb_main_window_class_init(BbMainWindowClass *class)
{
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
        bb_main_window_page_added
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_main_window_page_removed
        );
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

void
bb_main_window_page_added(GtkNotebook *notebook, GtkWidget *child, guint page_num, BbMainWindow *window)
{
    g_return_if_fail(child != NULL);
    g_return_if_fail(notebook != NULL);
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->document_notebook != notebook);

    // connect signals

    g_message("bb_main_window_page_added");
}

void
bb_main_window_page_removed(GtkNotebook *notebook, GtkWidget *child, guint page_num, BbMainWindow *window)
{
    g_return_if_fail(child != NULL);
    g_return_if_fail(notebook != NULL);
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->document_notebook != notebook);

    // disconnect signals

    g_message("bb_main_window_page_removed");
}
