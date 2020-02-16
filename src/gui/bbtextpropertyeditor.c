#include <gtk/gtk.h>
#include "bbtextpropertyeditor.h"


struct _BbTextPropertyEditor
{
    GtkExpander parent;
};


G_DEFINE_TYPE(BbTextPropertyEditor, bb_text_property_editor, GTK_TYPE_EXPANDER);


static void
bb_text_property_editor_class_init(BbTextPropertyEditorClass *class)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbtextpropertyeditor.ui"
        );
}


static void
bb_text_property_editor_init(BbTextPropertyEditor *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_text_property_editor_register()
{
    bb_text_property_editor_get_type();
}
