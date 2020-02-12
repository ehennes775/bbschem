#include <gtk/gtk.h>
#include "bbapplication.h"
#include "bbmainwindow.h"

struct _BbApplication
{
    GtkApplication parent;
};

G_DEFINE_TYPE(BbApplication, bb_application, GTK_TYPE_APPLICATION);


static void
bb_application_open(GApplication *application, GFile **files, gint n_files, const gchar *hint);


static void
bb_application_activate(GApplication *application)
{
    BbMainWindow *window;

    window = bb_main_window_new(BB_APPLICATION(application));
    gtk_window_present(GTK_WINDOW(window));
}


static void
bb_application_class_init(BbApplicationClass *class)
{
    G_APPLICATION_CLASS(class)->activate = bb_application_activate;
    G_APPLICATION_CLASS(class)->open = bb_application_open;
}


static void
bb_application_init(BbApplication *app)
{
}


BbApplication*
bb_application_new(void)
{
    return g_object_new(
        BB_APPLICATION_TYPE,
        "application-id", "com.github.ehennes775.bbsch",
        "flags", G_APPLICATION_HANDLES_OPEN,
        NULL
        );
}


static void
bb_application_open(GApplication *application, GFile **files, gint n_files, const gchar *hint)
{
    int i;

    for (i = 0; i < n_files; i++)
    {
        BbMainWindow *window;

        window = bb_main_window_new(BB_APPLICATION(application));
        gtk_window_present(GTK_WINDOW(window));
    }
}




