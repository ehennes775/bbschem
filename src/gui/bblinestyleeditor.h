#ifndef __BBLINESTYLEEDITOR__
#define __BBLINESTYLEEDITOR__
#include <gtk/gtk.h>


#define BB_TYPE_LINE_STYLE_EDITOR bb_line_style_editor_get_type()
G_DECLARE_FINAL_TYPE(BbLineStyleEditor, bb_line_style_editor, BB, LINE_STYLE_EDITOR, GtkExpander)


BbMainWindow*
bb_line_style_editor_get_main_window(BbLineStyleEditor *editor);

void
bb_line_style_editor_set_main_window(BbLineStyleEditor *editor, BbMainWindow *window);


#endif
