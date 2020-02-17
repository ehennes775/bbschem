#ifndef __BBPROPERTYCOMBOBOX__
#define __BBPROPERTYCOMBOBOX__
#include <gtk/gtk.h>


#define BB_TYPE_PROPERTY_COMBO_BOX bb_property_combo_box_get_type()
G_DECLARE_FINAL_TYPE(BbPropertyComboBox, bb_property_combo_box, BB, PROPERTY_COMBO_BOX, GtkComboBox)


#endif
