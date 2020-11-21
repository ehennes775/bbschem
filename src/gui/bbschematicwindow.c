/*
 * bbsch
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
#include <src/lib/bbschematic.h>
#include "bbschematicwrapper.h"
#include "bbschematicwindow.h"
#include "bbschematicwindowinner.h"


enum
{
    PROP_0,
    PROP_CAN_COPY,
    PROP_CAN_CUT,
    PROP_CAN_DELETE,
    PROP_CAN_PASTE,
    PROP_CAN_REDO,
    PROP_CAN_SELECT_ALL,
    PROP_CAN_SELECT_NONE,
    PROP_CAN_UNDO,
    PROP_SCHEMATIC_WRAPPER,
    N_PROPERTIES
};


struct _BbSchematicWindow
{
    BbDocumentWindow parent;

    BbSchematicWindowInner *inner_window;

    BbSchematicWrapper *schematic_wrapper;

    BbSchematic *schematic;


    GSList *redo_stack;

    GHashTable *selection;

    GSList *undo_stack;
};




static void
bb_schematic_window_attach_actions(BbDocumentWindow *window, GActionMap *map);

static void
bb_schematic_window_detach_actions(BbDocumentWindow *window, GActionMap *map);

static void
bb_schematic_window_dispose(GObject *object);

static void
bb_schematic_window_finalize(GObject *object);

static void
bb_schematic_window_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_schematic_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE(
    BbSchematicWindow,
    bb_schematic_window,
    BB_TYPE_DOCUMENT_WINDOW
    )


void
bb_schematic_window_apply_selection(BbSchematicWindow *window, BbApplyFunc func, gpointer user_data)
{
    g_return_if_fail(window != NULL);

    bb_schematic_foreach_modify(
        window->schematic,
        bb_pred_always,
        NULL,
        func,
        user_data
        );
}


static void
bb_schematic_window_attach_actions(BbDocumentWindow *window, GActionMap *map)
{
    BbSchematicWindow *x = BB_SCHEMATIC_WINDOW(window);
    g_return_if_fail(x != NULL);

    if (x->schematic_wrapper)
    {
        bb_schematic_wrapper_attach_actions(x->schematic_wrapper, map);
    }
}


static void
bb_schematic_window_class_init(BbSchematicWindowClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_schematic_window_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_schematic_window_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_schematic_window_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_schematic_window_set_property;

    BB_DOCUMENT_WINDOW_CLASS(klasse)->attach_actions = bb_schematic_window_attach_actions;
    BB_DOCUMENT_WINDOW_CLASS(klasse)->detach_actions = bb_schematic_window_detach_actions;

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAN_COPY,
        properties[PROP_CAN_COPY] = g_param_spec_boolean(
            "can-copy",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAN_CUT,
        properties[PROP_CAN_CUT] = g_param_spec_boolean(
            "can-cut",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAN_DELETE,
        properties[PROP_CAN_DELETE] = g_param_spec_boolean(
            "can-delete",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAN_PASTE,
        properties[PROP_CAN_PASTE] = g_param_spec_boolean(
            "can-paste",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAN_REDO,
        properties[PROP_CAN_REDO] = g_param_spec_boolean(
            "can-redo",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAN_SELECT_ALL,
        properties[PROP_CAN_SELECT_ALL] = g_param_spec_boolean(
            "can-select-all",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAN_SELECT_NONE,
        properties[PROP_CAN_SELECT_NONE] = g_param_spec_boolean(
            "can-select-none",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAN_UNDO,
        properties[PROP_CAN_UNDO] = g_param_spec_boolean(
            "can-undo",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_SCHEMATIC_WRAPPER,
        properties[PROP_SCHEMATIC_WRAPPER] = g_param_spec_object(
            "schematic-wrapper",
            "",
            "",
            BB_TYPE_SCHEMATIC_WRAPPER,
            G_PARAM_READWRITE
            )
        );

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/bbschematicwindow.ui"
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbSchematicWindow,
        inner_window
        );
}


void
bb_schematic_window_copy(BbSchematicWindow *window)
{

}


void
bb_schematic_window_cut(BbSchematicWindow *window)
{

}


static void
bb_schematic_window_detach_actions(BbDocumentWindow *window, GActionMap *map)
{
    BbSchematicWindow *w = BB_SCHEMATIC_WINDOW(window);
    g_return_if_fail(w != NULL);

    if (w->schematic_wrapper)
    {
        bb_schematic_wrapper_detach_actions(w->schematic_wrapper, map);
    }
}


static void
bb_schematic_window_dispose(GObject *object)
{
    //BbSchematicWindow* privat = BBSCHEMATIC_WINDOW_GET_PRIVATE(object);
}


static void
bb_schematic_window_finalize(GObject *object)
{
    //BbSchematicWindow* privat = BBSCHEMATIC_WINDOW_GET_PRIVATE(object);
}


gboolean
bb_schematic_window_get_can_copy(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return (g_hash_table_size(window->selection) > 0);
}


gboolean
bb_schematic_window_get_can_cut(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return (g_hash_table_size(window->selection) > 0);
}


gboolean
bb_schematic_window_get_can_delete(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return (g_hash_table_size(window->selection) > 0);
}


gboolean
bb_schematic_window_get_can_paste(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return TRUE;
}


gboolean
bb_schematic_window_get_can_redo(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return (window->redo_stack != NULL);
}


gboolean
bb_schematic_window_get_can_select_all(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);
    g_return_val_if_fail(window->selection != NULL, FALSE);

    return TRUE;
}


gboolean
bb_schematic_window_get_can_select_none(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);
    g_return_val_if_fail(window->selection != NULL, FALSE);

    return (g_hash_table_size(window->selection) > 0);
}


gboolean
bb_schematic_window_get_can_undo(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return (window->undo_stack != NULL);
}


static void
bb_schematic_window_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    BbSchematicWindow *window = BB_SCHEMATIC_WINDOW(object);
    g_return_if_fail(window != NULL);

    switch (property_id)
    {
        case PROP_CAN_COPY:
            g_value_set_boolean(value, bb_schematic_window_get_can_copy(window));
            break;

        case PROP_CAN_CUT:
            g_value_set_boolean(value, bb_schematic_window_get_can_cut(window));
            break;

        case PROP_CAN_DELETE:
            g_value_set_boolean(value, bb_schematic_window_get_can_delete(window));
            break;

        case PROP_CAN_PASTE:
            g_value_set_boolean(value, bb_schematic_window_get_can_paste(window));
            break;

        case PROP_CAN_REDO:
            g_value_set_boolean(value, bb_schematic_window_get_can_redo(window));
            break;

        case PROP_CAN_SELECT_ALL:
            g_value_set_boolean(value, bb_schematic_window_get_can_select_all(window));
            break;

        case PROP_CAN_SELECT_NONE:
            g_value_set_boolean(value, bb_schematic_window_get_can_select_none(window));
            break;

        case PROP_CAN_UNDO:
            g_value_set_boolean(value, bb_schematic_window_get_can_undo(window));
            break;

        case PROP_SCHEMATIC_WRAPPER:
            g_value_set_object(value, bb_schematic_window_get_schematic_wrapper(window));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


BbSchematicWrapper*
bb_schematic_window_get_schematic_wrapper(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, NULL);

    return window->schematic_wrapper;
}


static void
bb_schematic_window_init(BbSchematicWindow *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));

    window->schematic_wrapper = g_object_new(BB_TYPE_SCHEMATIC_WRAPPER, NULL);

    window->schematic = bb_schematic_new();
}


void
bb_schematic_window_paste(BbSchematicWindow *window)
{

}


void
bb_schematic_window_query_selection(BbSchematicWindow *window, BbQueryFunc func, gpointer user_data)
{
    g_return_if_fail(window != NULL);

    bb_schematic_foreach_query(
        window->schematic,
        bb_pred_always,
        NULL,
        func,
        user_data
        );
}


__attribute__((constructor)) void
bb_schematic_window_register()
{
    bb_schematic_window_get_type();
}


static void
bb_schematic_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    BbSchematicWindow *window = BB_SCHEMATIC_WINDOW(object);
    g_return_if_fail(window != NULL);

    switch (property_id)
    {
        case PROP_SCHEMATIC_WRAPPER:
            bb_schematic_window_set_schematic_wrapper(window, BB_SCHEMATIC_WRAPPER(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_schematic_window_set_schematic_wrapper(BbSchematicWindow *window, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(window != NULL);

    g_set_object(&window->schematic_wrapper, wrapper);

    if (window->schematic_wrapper != NULL)
    {
        // attach action ports
    }
    else
    {
        // detach action ports
    }
}


void
bb_schematic_window_select_all(BbSchematicWindow *window)
{

}


void
bb_schematic_window_select_none(BbSchematicWindow *window)
{

}


void
bb_schematic_window_select_undo(BbSchematicWindow *window)
{

}

