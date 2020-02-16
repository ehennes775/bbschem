#ifndef __BBPINPROPERTYEDITOR__
#define __BBPINPROPERTYEDITOR__
#include <gtk/gtk.h>


#define BB_TYPE_PIN_PROPERTY_EDITOR bb_pin_property_editor_get_type()
G_DECLARE_FINAL_TYPE(BbPinPropertyEditor, bb_pin_property_editor, BB, PinPropertyEditor, GtkExpander)


#endif
