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
bb_application_startup(GApplication *application);


static void
bb_application_activate(GApplication *application)
{
    BbMainWindow *window;

    window = bb_main_window_new(BB_APPLICATION(application));
    gtk_widget_show_all(GTK_WIDGET(window));
    gtk_window_present(GTK_WINDOW(window));
}


static void
bb_application_class_init(BbApplicationClass *class)
{
    G_APPLICATION_CLASS(class)->activate = bb_application_activate;
    G_APPLICATION_CLASS(class)->open = bb_application_open;
    G_APPLICATION_CLASS(class)->startup = bb_application_startup;
}


static void
bb_application_init(BbApplication *app)
{
}


BbApplication*
bb_application_new(void)
{
    return BB_APPLICATION(g_object_new(
        BB_TYPE_APPLICATION,
        "application-id", "com.github.ehennes775.bbschem",
        "flags", G_APPLICATION_HANDLES_OPEN,
        NULL
        ));
}


static void
bb_application_open(GApplication *application, GFile **files, gint n_files, const gchar *hint)
{
    int i;

    for (i = 0; i < n_files; i++)
    {
        BbMainWindow *window;

        window = bb_main_window_new(BB_APPLICATION(application));
        gtk_widget_show_all(GTK_WIDGET(window));
        gtk_window_present(GTK_WINDOW(window));
    }
}


static void
bb_application_startup(GApplication *application)
{
    G_APPLICATION_CLASS(bb_application_parent_class)->startup(application);

    GtkCssProvider* provider = gtk_css_provider_new();

    gtk_css_provider_load_from_resource(
        provider,
        "/com/github/ehennes775/bbsch/gui/bbapplication.css"
        );

    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(provider),
        600
        );

    GtkBuilder *builder = gtk_builder_new_from_resource(
        "/com/github/ehennes775/bbsch/gui/bbmenus.ui"
        );

    GMenuModel *model = G_MENU_MODEL(gtk_builder_get_object(builder, "menubar"));

    gtk_application_set_menubar(GTK_APPLICATION(application), model);
}
