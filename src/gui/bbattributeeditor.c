#include <gtk/gtk.h>
#include "bbattributeeditor.h"


struct _BbAttributeEditor
{
    GtkBox parent;
};


G_DEFINE_TYPE(BbAttributeEditor, bb_attribute_editor, GTK_TYPE_BOX);


static void
bb_attribute_editor_class_init(BbAttributeEditorClass *class)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbattributeeditor.ui"
        );
}


static void
bb_attribute_editor_init(BbAttributeEditor *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_attribute_editor_register()
{
    bb_attribute_editor_get_type();
}
