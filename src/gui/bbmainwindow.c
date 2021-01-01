/*
 * bbschem
 * Copyright (C) 2020 Edward C. Hennessy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <gtk/gtk.h>
#include <libpeas/peas.h>
#include <bbextensions.h>
#include "bbmainwindow.h"
#include "bbapplication.h"
#include "bbdocumentwindowtab.h"
#include "bbschematicwindow.h"

#include "bbcopyaction.h"
#include "bbcutaction.h"
#include "bbdeleteaction.h"
#include "bbpasteaction.h"
#include "bbquitaction.h"
#include "bbselectallaction.h"
#include "bbselectnoneaction.h"
#include "bbundoaction.h"
#include "bbredoaction.h"
#include "bbsaveaction.h"
#include "bbsaveallaction.h"
#include "bbsaveasaction.h"
#include "bbreloadaction.h"
#include "bbfillstyleeditor.h"
#include "bblinestyleeditor.h"
#include "bbattributeeditor.h"
#include "bbblocktoolpanel.h"
#include "bbpinpropertyeditor.h"
#include "bbtoolpalette.h"
#include "bbtextpropertyeditor.h"
#include "bbcoloreditor.h"
#include "bbchoosetoolaction.h"
#include "bbtoolstack.h"
#include "bbtoolchanger.h"
#include "bbarctoolpanel.h"
#include "bbcircletoolpanel.h"
#include "bblinetoolpanel.h"
#include "bbboxtoolpanel.h"
#include "bbtexttoolpanel.h"
#include "bbattributetoolpanel.h"
#include "bbzoomtoolpanel.h"
#include "bbselecttoolpanel.h"
#include "bbzoomextentsaction.h"
#include "bbzoominaction.h"
#include "bbzoomoutaction.h"
#include "bbrevealaction.h"
#include "bbzoompointaction.h"
#include "bbzoomdirection.h"
#include "bbpanaction.h"
#include "bbpandirection.h"
#include "bbpanpointaction.h"
#include "bbpintoolpanel.h"
#include "bbbustoolpanel.h"
#include "bbnettoolpanel.h"
#include "bbscalegridaction.h"
#include "bbshowgridaction.h"
#include "bbsnapactiveaction.h"
#include "bbopenaction.h"
#include "bbnewaction.h"
#include "bbgeneralopener.h"


enum
{
    PROP_0,
    PROP_CURRENT_DOCUMENT_WINDOW,
    PROP_OPENER,
    N_PROPERTIES
};


struct _BbMainWindow
{
    GtkApplicationWindow parent;

    PeasExtensionSet *extensions;

    BbDocumentWindow *current_page;
    GtkNotebook *document_notebook;
    BbGeneralOpener *general_opener;
    BbToolStack *tool_stack;
};


static gboolean
bb_main_window_key_pressed_cb(GtkWidget *unused, GdkEvent *event, BbMainWindow *window);

static gboolean
bb_main_window_key_released_cb(GtkWidget *unused, GdkEvent *event, BbMainWindow *window);

static void
bb_main_window_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_main_window_dispose(GObject *object);

static void
bb_main_window_notify_page_num(BbMainWindow *window, GParamSpec *pspec, GtkNotebook *notebook);

static void
bb_main_window_page_added(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook);

static void
bb_main_window_page_removed(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook);

static void
bb_main_window_set_opener(BbMainWindow *main_window, BbGeneralOpener *general_opener);

static void
bb_main_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_main_window_update(GtkWidget *child, BbMainWindow *window);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE(BbMainWindow, bb_main_window, GTK_TYPE_APPLICATION_WINDOW)


static void
bb_main_window_extension_added(
    PeasExtensionSet *extensions,
    PeasPluginInfo *plugin_info,
    PeasExtension *extension,
    BbMainWindow *main_window
    )
{
    peas_activatable_activate(PEAS_ACTIVATABLE(extension));
}


static void
bb_main_window_extension_removed(
    PeasExtensionSet *extensions,
    PeasPluginInfo *plugin_info,
    PeasExtension *extension,
    BbMainWindow *main_window
    )
{
    peas_activatable_deactivate(PEAS_ACTIVATABLE(extension));
}


static gboolean
bb_main_window_key_pressed_cb(GtkWidget *unused, GdkEvent *event, BbMainWindow *window)
{
    /* temporary key handling */

    switch (event->key.keyval)
    {
        case GDK_KEY_Z:
            g_action_group_activate_action(
                G_ACTION_GROUP(window),
                "zoom-point",
                g_variant_ref(g_variant_new_int32(BB_ZOOM_DIRECTION_OUT))
                );
            return TRUE;

        case GDK_KEY_z:
            g_action_group_activate_action(
                G_ACTION_GROUP(window),
                "zoom-point",
                g_variant_ref(g_variant_new_int32(BB_ZOOM_DIRECTION_IN))
                );
            return TRUE;

        /* WASD for now, the default handler is intercepting the arrow keys */

        case GDK_KEY_a:
            g_action_group_activate_action(
                G_ACTION_GROUP(window),
                "pan",
                g_variant_ref(g_variant_new_int32(BB_PAN_DIRECTION_LEFT))
                );
            return TRUE;

        case GDK_KEY_d:
            g_action_group_activate_action(
                G_ACTION_GROUP(window),
                "pan",
                g_variant_ref(g_variant_new_int32(BB_PAN_DIRECTION_RIGHT))
                );
            return TRUE;

        case GDK_KEY_w:
            g_action_group_activate_action(
                G_ACTION_GROUP(window),
                "pan",
                g_variant_ref(g_variant_new_int32(BB_PAN_DIRECTION_UP))
                );
            return TRUE;

        case GDK_KEY_s:
            g_action_group_activate_action(
                G_ACTION_GROUP(window),
                "pan",
                g_variant_ref(g_variant_new_int32(BB_PAN_DIRECTION_DOWN))
                );
            return TRUE;

        case GDK_KEY_p:
            g_action_group_activate_action(
                G_ACTION_GROUP(window),
                "pan-point",
                NULL
                );
            return TRUE;

        case GDK_KEY_bracketleft:
            g_action_group_activate_action(
                G_ACTION_GROUP(window),
                "scale-down",
                NULL
            );
            return TRUE;


        case GDK_KEY_bracketright:
            g_action_group_activate_action(
                G_ACTION_GROUP(window),
                "scale-up",
                NULL
            );
            return TRUE;

        default:
            return FALSE;
    }


}

static gboolean
bb_main_window_key_released_cb(GtkWidget *unused, GdkEvent *event, BbMainWindow *window)
{
    g_message("bb_main_window_key_released_cb");
}


void
bb_main_window_add_page(BbMainWindow *window, BbDocumentWindow *page)
{
    g_return_if_fail(BB_IS_MAIN_WINDOW(window));
    g_return_if_fail(window->document_notebook != NULL);
    g_return_if_fail(BB_IS_DOCUMENT_WINDOW(page));

    gtk_notebook_append_page(
        window->document_notebook,
        GTK_WIDGET(page),
        GTK_WIDGET(bb_document_window_tab_new(page))
        );

    gtk_widget_show_all(GTK_WIDGET(page));
}


static void
bb_main_window_class_init(BbMainWindowClass *class)
{
    BB_TYPE_ATTRIBUTE_EDITOR;
    BB_TYPE_COLOR_EDITOR;
    BB_TYPE_FILL_STYLE_EDITOR;
    BB_TYPE_LINE_STYLE_EDITOR;
    BB_TYPE_TEXT_PROPERTY_EDITOR;
    BB_TYPE_PIN_PROPERTY_EDITOR;
    BB_TYPE_TOOL_PALETTE;
    BB_TYPE_TOOL_STACK;

    BB_TYPE_ARC_TOOL_PANEL;
    BB_TYPE_ATTRIBUTE_TOOL_PANEL;
    BB_TYPE_BLOCK_TOOL_PANEL;
    BB_TYPE_CIRCLE_TOOL_PANEL;
    BB_TYPE_LINE_TOOL_PANEL;
    BB_TYPE_BOX_TOOL_PANEL;
    BB_TYPE_TEXT_TOOL_PANEL;
    BB_TYPE_ZOOM_TOOL_PANEL;
    BB_TYPE_SELECT_TOOL_PANEL;
    BB_TYPE_PIN_TOOL_PANEL;
    BB_TYPE_BUS_TOOL_PANEL;
    BB_TYPE_NET_TOOL_PANEL;


    G_OBJECT_CLASS(class)->dispose = bb_main_window_dispose;
    G_OBJECT_CLASS(class)->get_property = bb_main_window_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_main_window_set_property;

    properties[PROP_CURRENT_DOCUMENT_WINDOW] = bb_object_class_install_property(
        G_OBJECT_CLASS(class),
        PROP_CURRENT_DOCUMENT_WINDOW,
        g_param_spec_object(
            "current-document-window",
            "",
            "",
            BB_TYPE_DOCUMENT_WINDOW,
            G_PARAM_READABLE
            )
        );

    properties[PROP_OPENER] = bb_object_class_install_property(
        G_OBJECT_CLASS(class),
        PROP_OPENER,
        g_param_spec_object(
            "opener",
            "",
            "",
            BB_TYPE_GENERAL_OPENER,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbmainwindow.ui"
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbMainWindow,
        document_notebook
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_main_window_notify_page_num
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_main_window_page_added
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(class),
        bb_main_window_page_removed
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(class),
        BbMainWindow,
        tool_stack
        );

    g_signal_new(
        "update",
        G_OBJECT_CLASS_TYPE(class),
        (GSignalFlags) 0,
        0,
        NULL,
        NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE,
        0
        );
}


static void
bb_main_window_dispose(GObject *object)
{
    BbMainWindow *window = BB_MAIN_WINDOW(object);

    g_return_if_fail(window != NULL);

    g_set_object(&window->current_page, NULL);
    g_clear_object(&window->extensions);
}


GtkWidget*
bb_main_window_get_current_document_window(BbMainWindow* window)
{
    g_return_val_if_fail(window != NULL, NULL);
    g_return_val_if_fail(window->document_notebook != NULL, NULL);

    gint page_num = gtk_notebook_get_current_page(window->document_notebook);
    gboolean empty = (page_num < 0);

    return empty ? NULL : gtk_notebook_get_nth_page(window->document_notebook, page_num);
}


BbGeneralOpener*
bb_main_window_get_opener(BbMainWindow *main_window)
{
    g_return_val_if_fail(BB_IS_MAIN_WINDOW(main_window), NULL);

    return main_window->general_opener;
}



static void
bb_main_window_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CURRENT_DOCUMENT_WINDOW:
            g_value_set_object(value, bb_main_window_get_current_document_window(BB_MAIN_WINDOW(object)));
            break;

        case PROP_OPENER:
            g_value_set_object(value, bb_main_window_get_opener(BB_MAIN_WINDOW(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_main_window_init(BbMainWindow *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));

//    bb_main_window_add_page(window, g_object_new(
//        BB_TYPE_SCHEMATIC_WINDOW,
//        "grid-control", window->tool_stack,
//        "tool-changer", window->tool_stack,
//        NULL
//        ));
//
//    bb_main_window_add_page(window, g_object_new(
//        BB_TYPE_SCHEMATIC_WINDOW,
//        "tool-changer", window->tool_stack,
//        NULL
//        ));

    BbDocumentWindow *temp = g_object_new(BB_TYPE_SCHEMATIC_WINDOW, NULL);
    bb_main_window_add_page(window, temp);

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_copy_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_cut_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_delete_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_paste_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_quit_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_redo_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_reload_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_save_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_save_all_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_save_as_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_select_all_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_select_none_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_undo_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_choose_tool_action_new(window, GTK_STACK(window->tool_stack)))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_zoom_extents_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_zoom_in_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_zoom_out_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_reveal_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_zoom_point_action_new(window))
        );

    gtk_widget_add_events(
        GTK_WIDGET(window),
        GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_pan_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_pan_point_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_scale_grid_action_new(window, BB_SCALE_GRID_DIRECTION_DOWN))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_scale_grid_action_new(window, BB_SCALE_GRID_DIRECTION_RESET))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_scale_grid_action_new(window, BB_SCALE_GRID_DIRECTION_UP))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_show_grid_action_new(BB_GRID_CONTROL(window->tool_stack)))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_snap_active_action_new(BB_GRID_CONTROL(window->tool_stack)))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_open_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_new_action_new(window))
        );

    g_signal_connect_after(
        window,
        "key-press-event",
        G_CALLBACK(bb_main_window_key_pressed_cb),
        window
        );

    g_signal_connect_after(
        window,
        "key-release-event",
        G_CALLBACK(bb_main_window_key_released_cb),
        window
        );

    bb_main_window_set_opener(
        window,
        bb_general_opener_new()
        );

    window->extensions = peas_extension_set_new(
        peas_engine_get_default(),
        PEAS_TYPE_ACTIVATABLE,
        "object", window,
        NULL
        );

    peas_extension_set_foreach(
        window->extensions,
        (PeasExtensionSetForeachFunc) bb_main_window_extension_added,
        window
        );

    g_signal_connect(
        window->extensions,
        "extension-added",
        G_CALLBACK(bb_main_window_extension_added),
        window
        );

    g_signal_connect(
        window->extensions,
        "extension-removed",
        G_CALLBACK(bb_main_window_extension_removed),
        window
        );
}


BbMainWindow*
bb_main_window_new(BbApplication *application)
{
    return g_object_new(
        BB_TYPE_MAIN_WINDOW,
        "application", application,
        //"opener", bb_general_opener_new(),
        NULL
        );
}


static void
bb_main_window_notify_page_num(BbMainWindow *window, GParamSpec *pspec, GtkNotebook *notebook)
{
    g_return_if_fail(window != NULL);

    GtkWidget *temp_page = bb_main_window_get_current_document_window(window);
    BbDocumentWindow *next_page = BB_IS_DOCUMENT_WINDOW(temp_page) ? BB_DOCUMENT_WINDOW(temp_page) : NULL;

    if (window->current_page != next_page)
    {
        g_set_object(&window->current_page, next_page);

        g_signal_emit_by_name(window, "update");
        g_object_notify(G_OBJECT(window), "current-document-window");
    }
}


static void
bb_main_window_page_added(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook)
{
    g_return_if_fail(child != NULL);
    g_return_if_fail(notebook != NULL);
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->document_notebook == notebook);

    g_signal_connect(
        child,
        "update",
        G_CALLBACK(bb_main_window_update),
        window
        );
}


static void
bb_main_window_page_removed(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook)
{
    g_return_if_fail(child != NULL);
    g_return_if_fail(notebook != NULL);
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->document_notebook == notebook);

    g_signal_handlers_disconnect_by_func(
        child,
        G_CALLBACK(bb_main_window_update),
        window
        );
}


__attribute__((constructor)) void
bb_main_window_register()
{
    bb_main_window_get_type();
}


static void
bb_main_window_set_opener(BbMainWindow *main_window, BbGeneralOpener *general_opener)
{
    g_return_if_fail(BB_IS_MAIN_WINDOW(main_window));
    g_return_if_fail(BB_IS_GENERAL_OPENER(general_opener));

    if (main_window->general_opener != general_opener)
    {
        if (main_window->general_opener != NULL)
        {
            g_object_unref(general_opener);
        }

        main_window->general_opener = general_opener;

        if (main_window->general_opener != NULL)
        {
            g_object_ref(general_opener);
        }

        g_object_notify_by_pspec(G_OBJECT(main_window), properties[PROP_OPENER]);
    }
}

static void
bb_main_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_OPENER:
            bb_main_window_set_opener(BB_MAIN_WINDOW(object), BB_GENERAL_OPENER(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_main_window_update(GtkWidget* child, BbMainWindow *window)
{
    g_return_if_fail(window != NULL);

    g_signal_emit_by_name(window, "update");
}
