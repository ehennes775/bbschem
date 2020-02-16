#include <gtk/gtk.h>
#include "bbcoloreditor.h"


struct _BbColorEditor
{
    GtkExpander parent;
};


G_DEFINE_TYPE(BbColorEditor, bb_color_editor, GTK_TYPE_EXPANDER);


static void
bb_color_editor_class_init(BbColorEditorClass *class)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbcoloreditor.ui"
        );
}


static void
bb_color_editor_init(BbColorEditor *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_color_editor_register()
{
    bb_color_editor_get_type();
}
