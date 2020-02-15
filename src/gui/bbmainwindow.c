#include <gtk/gtk.h>
#include "bbmainwindow.h"
#include "bbapplication.h"


struct _BbMainWindow
{
    GtkApplicationWindow parent;
};


G_DEFINE_TYPE(BbMainWindow, bb_main_window, GTK_TYPE_APPLICATION_WINDOW);


static void
bb_main_window_class_init(BbMainWindowClass *class)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbmainwindow.ui"
        );
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
