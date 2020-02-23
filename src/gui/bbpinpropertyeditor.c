#include <gtk/gtk.h>
#include "bbmainwindow.h"
#include "bbpinpropertyeditor.h"
#include "bbpropertycombobox.h"


enum
{
    PROP_0,
    PROP_MAIN_WINDOW
};


struct _BbPinPropertyEditor
{
    GtkExpander parent;

    BbMainWindow *main_window;
};

G_DEFINE_TYPE(BbPinPropertyEditor, bb_pin_property_editor, GTK_TYPE_EXPANDER);


static void
bb_pin_property_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec);

static void
bb_pin_property_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec);


static void
bb_pin_property_editor_apply(BbPropertyComboBox *combo, BbPinPropertyEditor *editor)
{
    g_message("Apply Pin Property");
}


static void
bb_pin_property_editor_class_init(BbPinPropertyEditorClass *class)
{
    G_OBJECT_CLASS(class)->get_property = bb_pin_property_editor_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_pin_property_editor_set_property;

    g_object_class_install_property(
        G_OBJECT_CLASS(class),
        PROP_MAIN_WINDOW,
        g_param_spec_object(
            "main-window",
            "",
            "",
            BB_MAIN_WINDOW_TYPE,
            G_PARAM_READWRITE
            )
        );

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbpinpropertyeditor.ui"
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_pin_property_editor_apply
        );
}


BbMainWindow*
bb_pin_property_editor_get_main_window(BbPinPropertyEditor *editor)
{
    g_return_val_if_fail(editor != NULL, NULL);

    return editor->main_window;
}


static void
bb_pin_property_editor_get_property(GObject *object, guint param_id, GValue* value, GParamSpec* pspec)
{
    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            g_value_set_object(value, bb_pin_property_editor_get_main_window(BB_PIN_PROPERTY_EDITOR(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
    }
}


static void
bb_pin_property_editor_init(BbPinPropertyEditor *editor)
{
    gtk_widget_init_template(GTK_WIDGET(editor));
}


__attribute__((constructor)) void
bb_pin_property_editor_register()
{
    bb_pin_property_editor_get_type();
}


void
bb_pin_property_editor_set_main_window(BbPinPropertyEditor *editor, BbMainWindow *window)
{
    g_return_if_fail(editor != NULL);

    if (editor->main_window != NULL)
    {
        g_object_unref(editor->main_window);
    }

    editor->main_window = window;

    if (editor->main_window != NULL)
    {
        g_object_ref(editor->main_window);
    }
}


static void
bb_pin_property_editor_set_property(GObject *object, guint param_id, const GValue* value, GParamSpec* pspec)
{
    switch (param_id)
    {
        case PROP_MAIN_WINDOW:
            bb_pin_property_editor_set_main_window(BB_PIN_PROPERTY_EDITOR(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, param_id, pspec);
    }
}
