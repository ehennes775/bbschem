#ifndef __BBCOMPONENTSELECTOR__
#define __BBCOMPONENTSELECTOR__
#include <gtk/gtk.h>


#define BB_TYPE_COMPONENT_SELECTOR bb_component_selector_get_type()
G_DECLARE_FINAL_TYPE(BbComponentSelector, bb_component_selector, BB, ComponentSelector, GtkBin)


#endif
