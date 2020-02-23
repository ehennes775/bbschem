#ifndef __BBFILLSTYLEEDITOR__
#define __BBFILLSTYLEEDITOR__
#include <gtk/gtk.h>


#define BB_TYPE_FILL_STYLE_EDITOR bb_fill_style_editor_get_type()
G_DECLARE_FINAL_TYPE(BbFillStyleEditor, bb_fill_style_editor, BB, FILL_STYLE_EDITOR, GtkExpander)


BbMainWindow*
bb_fill_style_editor_get_main_window(BbFillStyleEditor *editor);

void
bb_fill_style_editor_set_main_window(BbFillStyleEditor *editor, BbMainWindow *window);


#endif
