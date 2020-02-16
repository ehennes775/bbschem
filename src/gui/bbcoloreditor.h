#ifndef __BBCOLOREDITOR__
#define __BBCOLOREDITOR__
#include <gtk/gtk.h>


#define BB_TYPE_COLOR_EDITOR bb_color_editor_get_type()
G_DECLARE_FINAL_TYPE(BbColorEditor, bb_color_editor, BB, ColorEditor, GtkExpander)


#endif
