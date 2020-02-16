#include <gtk/gtk.h>
#include "bbpinpropertyeditor.h"


struct _BbPinPropertyEditor
{
    GtkExpander parent;
};


G_DEFINE_TYPE(BbPinPropertyEditor, bb_pin_property_editor, GTK_TYPE_EXPANDER);


static void
bb_pin_property_editor_class_init(BbPinPropertyEditorClass *class)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbpinpropertyeditor.ui"
        );
}


static void
bb_pin_property_editor_init(BbPinPropertyEditor *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_pin_property_editor_register()
{
    bb_pin_property_editor_get_type();
}
