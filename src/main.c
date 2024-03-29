#include <gtk/gtk.h>
#include <libpeas/peas.h>
#include <gedaplugin/bbgedaplugin.h>
#include "gui/bbapplication.h"


int
main(int argc, char *argv[])
{
#if 0
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

    gboolean success = peas_engine_load_plugin(
        engine,
        plugin_info
        );

    g_message("peas_engine_load_plugin() = %d", success);

#endif

    int status = g_application_run(
        G_APPLICATION(bb_application_new()),
        argc,
        argv
        );

#if 0
    g_clear_object(&engine);
#endif

    return status;
}
