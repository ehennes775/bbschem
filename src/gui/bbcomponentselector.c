#include <gtk/gtk.h>
#include "bbcomponentselector.h"


struct _BbComponentSelector
{
    GtkBox parent;
};


G_DEFINE_TYPE(BbComponentSelector, bb_component_selector, GTK_TYPE_BOX);


static void
bb_component_selector_class_init(BbComponentSelectorClass *class)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbcomponentselector.ui"
        );
}


static void
bb_component_selector_init(BbComponentSelector *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_component_selector_register()
{
    bb_component_selector_get_type();
}
