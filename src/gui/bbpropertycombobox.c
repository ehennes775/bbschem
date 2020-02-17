#include <gtk/gtk.h>
#include "bbpropertycombobox.h"
#include "bbpinpropertyeditor.h"


struct _BbPropertyComboBox
{
    GtkComboBox parent;
};


G_DEFINE_TYPE(BbPropertyComboBox, bb_property_combo_box, GTK_TYPE_COMBO_BOX);


static void
bb_property_combo_box_class_init(BbPropertyComboBoxClass *class)
{
}


static void
bb_property_combo_box_init(BbPropertyComboBox *window)
{
}


__attribute__((constructor)) void
bb_property_combo_box_register()
{
    bb_property_combo_box_get_type();
}
