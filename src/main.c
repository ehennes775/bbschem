#include <gtk/gtk.h>
#include <libpeas/peas.h>
#include <gedaplugin/bbgedaplugin.h>
#include "gui/bbapplication.h"


int
main(int argc, char *argv[])
{
    PeasEngine *engine = peas_engine_get_default();

    peas_engine_add_search_path(
        engine,
        ".",
        NULL
        );

    peas_engine_add_search_path(
        engine,
        "/home/ehennes/Projects/bbschem/cmake-build-debug/src/gedaplugin",
        NULL
        );

    PeasPluginInfo *plugin_info = peas_engine_get_plugin_info(
        engine,
        "bbgedaplugin"
        );

    peas_engine_load_plugin(
        engine,
        plugin_info
        );

    gchar **plugin_names = peas_engine_get_loaded_plugins(engine);
    gchar *all = g_strjoinv("\n", plugin_names);
    g_message("Loaded: %s", all);
    g_free(all);
    g_strfreev(plugin_names);

    int status = g_application_run(
        G_APPLICATION(bb_application_new()),
        argc,
        argv
        );

    g_clear_object(&engine);

    return status;
}
