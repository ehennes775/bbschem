#ifndef __BBCOLOREDITOR__
#define __BBCOLOREDITOR__
#include <gtk/gtk.h>


#define BB_TYPE_COLOR_EDITOR bb_color_editor_get_type()
G_DECLARE_FINAL_TYPE(BbColorEditor, bb_color_editor, BB, COLOR_EDITOR, GtkExpander)


BbMainWindow*
bb_color_editor_get_main_window(BbColorEditor *editor);

void
bb_color_editor_set_main_window(BbColorEditor *editor, BbMainWindow *window);


#endif
