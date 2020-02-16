#include <gtk/gtk.h>
#include "bblinestyleeditor.h"


struct _BbLineStyleEditor
{
    GtkExpander parent;
};


G_DEFINE_TYPE(BbLineStyleEditor, bb_line_style_editor, GTK_TYPE_EXPANDER);


static void
bb_line_style_editor_class_init(BbLineStyleEditorClass *class)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bblinestyleeditor.ui"
        );
}


static void
bb_line_style_editor_init(BbLineStyleEditor *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_line_style_editor_register()
{
    bb_line_style_editor_get_type();
}
