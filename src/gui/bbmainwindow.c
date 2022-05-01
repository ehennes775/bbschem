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
#include "gedaplugin/bbgedaeditor.h"

#include "actions/bbcopyaction.h"
#include "actions/bbcutaction.h"
#include "actions/bbdeleteaction.h"
#include "actions/bbpasteaction.h"
#include "actions/bbquitaction.h"
#include "actions/bbselectallaction.h"
#include "actions/bbselectnoneaction.h"
#include "actions/bbundoaction.h"
#include "actions/bbredoaction.h"
#include "actions/bbsaveaction.h"
#include "actions/bbsaveallaction.h"
#include "actions/bbsaveasaction.h"
#include "actions/bbreloadaction.h"
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
#include "actions/bbzoomextentsaction.h"
#include "actions/bbzoominaction.h"
#include "actions/bbzoomoutaction.h"
#include "actions/bbrevealaction.h"
#include "actions/bbzoompointaction.h"
#include "actions/types/bbzoomdirection.h"
#include "actions/bbpanaction.h"
#include "actions/types/bbpandirection.h"
#include "actions/bbpanpointaction.h"
#include "bbpintoolpanel.h"
#include "bbbustoolpanel.h"
#include "bbnettoolpanel.h"
#include "actions/bbscalegridaction.h"
#include "actions/bbshowgridaction.h"
#include "actions/bbsnapactiveaction.h"
#include "actions/bbopenaction.h"
#include "actions/bbnewaction.h"
#include "bbgeneralopener.h"
#include "bbint32combobox.h"
#include "actions/bbopenreceiver.h"
#include "gedaplugin/bbgedaopener.h"
#include "textedit/bbtexteditor.h"


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

    GSList *document_actions;
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
bb_main_window_open_receiver_init(BbOpenReceiverInterface *iface);

static void
bb_main_window_page_added(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook);

static void
bb_main_window_page_removed(BbMainWindow *window, GtkWidget *child, guint page_num, GtkNotebook *notebook);

static void
bb_main_window_quit_receiver_init(BbQuitReceiverInterface *iface);

static void
bb_main_window_save_all_receiver_init(BbSaveAllReceiverInterface *iface);

static void
bb_main_window_set_opener(BbMainWindow *main_window, BbGeneralOpener *general_opener);

static void
bb_main_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_main_window_update(GtkWidget *child, BbMainWindow *window);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_EXTENDED(
        BbMainWindow,
        bb_main_window,
        GTK_TYPE_APPLICATION_WINDOW,
        0,
        G_IMPLEMENT_INTERFACE(BB_TYPE_OPEN_RECEIVER, bb_main_window_open_receiver_init)
        G_IMPLEMENT_INTERFACE(BB_TYPE_SAVE_ALL_RECEIVER, bb_main_window_save_all_receiver_init)
        G_IMPLEMENT_INTERFACE(BB_TYPE_QUIT_RECEIVER, bb_main_window_quit_receiver_init)
        )

// region From BbOpenReceiver interface

static void
bb_main_window_add_window(BbOpenReceiver *receiver, BbDocumentWindow *window)
{
    bb_main_window_add_page(BB_MAIN_WINDOW(receiver), window);
}

static void
bb_main_window_open_receiver_init(BbOpenReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->add_window = bb_main_window_add_window;
}

// endregion

// region From BbQuitReceiver interface

static void
bb_main_window_quit(BbQuitReceiver *receiver)
{
    GtkWindow *window = GTK_WINDOW(receiver);

    GApplication *application = G_APPLICATION(gtk_window_get_application(window));

    g_application_quit(application);
}

static void
bb_main_window_quit_receiver_init(BbQuitReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->quit = bb_main_window_quit;
}

// endregion

// region From BbSaveAllReceiver interface

static void
bb_main_window_save_all_receiver_init(BbSaveAllReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);
}

// endregion

static void
bb_main_window_extension_added(
        PeasExtensionSet *extensions,
        PeasPluginInfo *plugin_info,
        PeasExtension *extension,
        BbMainWindow *main_window
)
{
    g_message("bb_main_window_extension_added");

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
    g_message("bb_main_window_extension_removed");

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
bb_main_window_add_document_action(BbMainWindow *main_window, BbGenericReceiver *action)
{
    g_return_if_fail(BB_IS_MAIN_WINDOW(main_window));

    main_window->document_actions = g_slist_append(
        main_window->document_actions,
        g_object_ref(action)
        );

    g_action_map_add_action(
        G_ACTION_MAP(main_window),
        G_ACTION(action)
        );
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
    BB_TYPE_INT32_COMBO_BOX;

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

    G_OBJECT_CLASS(bb_main_window_parent_class)->dispose(object);
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


BbToolChanger*
bb_main_window_get_tool_changer(BbMainWindow *main_window)
{
    g_return_val_if_fail(BB_IS_MAIN_WINDOW(main_window), NULL);

    return BB_TOOL_CHANGER(main_window->tool_stack);
}


static void
bb_main_window_init(BbMainWindow *window)
{
    GtkClipboard *clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

    gtk_widget_init_template(GTK_WIDGET(window));

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_open_action_new(BB_OPEN_RECEIVER(window)))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_quit_action_new(BB_QUIT_RECEIVER(window)))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_save_all_action_new(BB_SAVE_ALL_RECEIVER(window)))
        );

    bb_main_window_add_document_action(
        window,
        BB_GENERIC_RECEIVER(bb_copy_action_new())
        );

    bb_main_window_add_document_action(
        window,
        BB_GENERIC_RECEIVER(bb_cut_action_new(clipboard))
        );

    bb_main_window_add_document_action(
        window,
        BB_GENERIC_RECEIVER(bb_paste_action_new(clipboard))
        );

    bb_main_window_add_document_action(
        window,
        BB_GENERIC_RECEIVER(bb_delete_action_new())
        );

    bb_main_window_add_document_action(
        window,
        BB_GENERIC_RECEIVER(bb_redo_action_new())
        );

    bb_main_window_add_document_action(
        (window),
        BB_GENERIC_RECEIVER(bb_save_action_new())
        );

    bb_main_window_add_document_action(
        window,
        BB_GENERIC_RECEIVER(bb_select_all_action_new())
        );

    bb_main_window_add_document_action(
        window,
        BB_GENERIC_RECEIVER(bb_select_none_action_new())
        );

    bb_main_window_add_document_action(
        window,
        BB_GENERIC_RECEIVER(bb_undo_action_new())
        );

    bb_main_window_add_page(window, g_object_new(
        BB_TYPE_TEXT_EDITOR,
        NULL
        ));
#if 0
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
        G_ACTION(bb_redo_action_new(window))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_reload_action_new(window))
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
        G_ACTION(bb_scale_grid_action_new(BB_SCALE_GRID_DIRECTION_DOWN))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_scale_grid_action_new(BB_SCALE_GRID_DIRECTION_RESET))
        );

    g_action_map_add_action(
        G_ACTION_MAP(window),
        G_ACTION(bb_scale_grid_action_new(BB_SCALE_GRID_DIRECTION_UP))
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
#endif
    bb_main_window_set_opener(
        window,
        bb_general_opener_new()
        );
#if 0
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
#endif

    BbGeneralOpener *general_opener = bb_main_window_get_opener(window);
    BbGedaOpener *geda_opener = bb_geda_opener_new(window);
    BbSpecificOpener *specific_opener = BB_SPECIFIC_OPENER(geda_opener);

    bb_general_opener_add_specific_opener(
        general_opener,
        "application/x-geda-schematic",
        specific_opener
        );

    bb_general_opener_add_specific_opener(
        general_opener,
        "application/x-geda-symbol",
        specific_opener
        );

    bb_general_opener_add_specific_opener(
        general_opener,
        "application/x-lepton-schematic",
        specific_opener
        );

    bb_general_opener_add_specific_opener(
        general_opener,
        "application/x-lepton-symbol",
        specific_opener
        );
}


BbMainWindow*
bb_main_window_new(BbApplication *application)
{
    return g_object_new(
        BB_TYPE_MAIN_WINDOW,
        "application", application,
        NULL
        );
}


static void
bb_main_window_notify_page_num(BbMainWindow *window, GParamSpec *pspec, GtkNotebook *notebook)
{
    g_return_if_fail(BB_IS_MAIN_WINDOW(window));

    GtkWidget *temp_page = bb_main_window_get_current_document_window(window);
    BbDocumentWindow *next_page = BB_IS_DOCUMENT_WINDOW(temp_page) ? BB_DOCUMENT_WINDOW(temp_page) : NULL;

    if (window->current_page != next_page)
    {
        g_set_object(&window->current_page, next_page);

        for (GSList *iter = window->document_actions; iter != NULL; iter = iter->next)
        {
            bb_generic_receiver_set_receiver(
                    BB_GENERIC_RECEIVER(iter->data),
                    G_OBJECT(next_page)
                    );
        }

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
