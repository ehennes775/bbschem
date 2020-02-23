#ifndef __BBMAINWINDOW__
#define __BBMAINWINDOW__

#include <gtk/gtk.h>
#include "bbapplication.h"
#include "bbdocumentwindow.h"


#define BB_MAIN_WINDOW_TYPE bb_main_window_get_type()
G_DECLARE_FINAL_TYPE(BbMainWindow, bb_main_window, BB, MAIN_WINDOW, GtkApplicationWindow)


void
bb_main_window_add_page(BbMainWindow *window, BbDocumentWindow *page);

GtkWidget*
bb_main_window_get_current_document_window(BbMainWindow* window);

BbMainWindow*
bb_main_window_new(BbApplication *application);


#endif
