#parse("License.h")

#[[#include]]# <gtk/gtk.h>

void
main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);
    
    return g_test_run();
}
