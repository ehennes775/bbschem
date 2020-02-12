#ifndef __BBAPPLICATION__
#define __BBAPPLICATION__

#include <gtk/gtk.h>


#define BB_APPLICATION_TYPE bb_application_get_type()
G_DECLARE_FINAL_TYPE(BbApplication, bb_application, BB, APPLICATION, GtkApplication)


BbApplication*
bb_application_new(void);

#endif
