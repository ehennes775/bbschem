#ifndef __BBATTRIBUTEEDITOR__
#define __BBATTRIBUTEEDITOR__
#include <gtk/gtk.h>


#define BB_TYPE_COMPONENT_SELECTOR bb_attribute_editor_get_type()
G_DECLARE_FINAL_TYPE(BbAttributeEditor, bb_attribute_editor, BB, AttributeEditor, GtkBox)


#endif
