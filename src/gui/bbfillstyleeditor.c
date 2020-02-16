#include <gtk/gtk.h>
#include "bbfillstyleeditor.h"


struct _BbFillStyleEditor
{
    GtkExpander parent;
};


G_DEFINE_TYPE(BbFillStyleEditor, bb_fill_style_editor, GTK_TYPE_EXPANDER);


static void
bb_fill_style_editor_class_init(BbFillStyleEditorClass *class)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbfillstyleeditor.ui"
        );
}


static void
bb_fill_style_editor_init(BbFillStyleEditor *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_fill_style_editor_register()
{
    bb_fill_style_editor_get_type();
}
