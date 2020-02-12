#include <gtk/gtk.h>
#include "gui/bbapplication.h"

int
main(int argc, char *argv[])
{
    return g_application_run(
        G_APPLICATION(bb_application_new()),
        argc,
        argv
        );
}
