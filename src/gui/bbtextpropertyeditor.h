#ifndef __BBTEXTPROPERTYEDITOR__
#define __BBTEXTPROPERTYEDITOR__
#include <gtk/gtk.h>


#define BB_TYPE_COLOR_EDITOR bb_text_property_editor_get_type()
G_DECLARE_FINAL_TYPE(BbTextPropertyEditor, bb_text_property_editor, BB, TEXT_PROPERTY_EDITOR, GtkExpander)


BbMainWindow*
bb_text_property_editor_get_main_window(BbTextPropertyEditor *editor);

void
bb_text_property_editor_set_main_window(BbTextPropertyEditor *editor, BbMainWindow *window);


#endif
