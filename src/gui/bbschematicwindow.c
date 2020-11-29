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

#include <math.h>
#include <gtk/gtk.h>
#include <bbextensions.h>
#include <bblibrary.h>
#include "bbschematicwindow.h"
#include "bbschematicwindowinner.h"
#include "bbarctool.h"
#include "bbtoolchanger.h"
#include "bbgraphics.h"
#include "bbzoomsubject.h"


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
    PROP_DRAWING_TOOL,
    PROP_TOOL_CHANGER,
    N_PROPERTIES
};


struct _BbSchematicWindow
{
    BbDocumentWindow parent;

    BbSchematicWindowInner *inner_window;

    BbDrawingTool *drawing_tool;

    BbSchematic *schematic;

    GFile *file;

    GSList *redo_stack;

    GHashTable *selection;

    BbToolChanger *tool_changer;

    GSList *undo_stack;
};


static void
bb_schematic_window_add_item(BbToolSubject *subject, BbSchematicItem *item);

static gboolean
bb_schematic_window_button_pressed_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static gboolean
bb_schematic_window_button_released_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static void
bb_schematic_window_dispose(GObject *object);

static void
bb_schematic_window_draw_cb(BbSchematicWindowInner *inner, cairo_t *cairo, BbSchematicWindow *outer);

static void
bb_schematic_window_finalize(GObject *object);

static void
bb_schematic_window_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_schematic_window_invalidate_item_cb(BbDrawingTool *tool, BbSchematicItem *item, BbSchematicWindow *window);

static void
bb_schematic_window_invalidate_rect_dev(BbSchematicWindow *window, double x0, double y0, double x1, double y1);

static gboolean
bb_schematic_window_key_pressed_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static gboolean
bb_schematic_window_key_released_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static gboolean
bb_schematic_window_motion_notify_cb(GtkWidget *widget, GdkEvent  *event, gpointer user_data);

static void
bb_schematic_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_schematic_window_tool_changed_cb(BbToolChanger *changer, BbSchematicWindow *window);

static void
bb_schematic_window_tool_subject_init(BbToolSubjectInterface *iface);

static void
bb_schematic_window_zoom_extents(BbZoomSubject *zoom_subject);

static void
bb_schematic_window_zoom_in(BbZoomSubject *zoom_subject);

static void
bb_schematic_window_zoom_out(BbZoomSubject *zoom_subject);

static void
bb_schematic_window_zoom_subject_init(BbZoomSubjectInterface *iface);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbSchematicWindow,
    bb_schematic_window,
    BB_TYPE_DOCUMENT_WINDOW,
    G_IMPLEMENT_INTERFACE(BB_TYPE_TOOL_SUBJECT, bb_schematic_window_tool_subject_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_ZOOM_SUBJECT, bb_schematic_window_zoom_subject_init)
    )


static void
bb_schematic_window_add_item(BbToolSubject *subject, BbSchematicItem *item)
{
    g_message("bb_schematic_window_add_item");
}


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


static gboolean
bb_schematic_window_button_pressed_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbSchematicWindow *window = BB_SCHEMATIC_WINDOW(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    if (window->drawing_tool != NULL)
    {
        return bb_drawing_tool_button_pressed(window->drawing_tool, event->button.x, event->button.y);
    }

    return FALSE;
}


static gboolean
bb_schematic_window_button_released_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbSchematicWindow *window = BB_SCHEMATIC_WINDOW(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    if (window->drawing_tool != NULL)
    {
        return bb_drawing_tool_button_released(window->drawing_tool, event->button.x, event->button.y);
    }

    return FALSE;
}


static void
bb_schematic_window_class_init(BbSchematicWindowClass *klasse)
{
    BB_TYPE_SCHEMATIC_WINDOW_INNER;

    G_OBJECT_CLASS(klasse)->dispose = bb_schematic_window_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_schematic_window_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_schematic_window_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_schematic_window_set_property;

    bb_object_class_install_property(
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

    bb_object_class_install_property(
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

    bb_object_class_install_property(
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

    bb_object_class_install_property(
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

    bb_object_class_install_property(
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

    bb_object_class_install_property(
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

    bb_object_class_install_property(
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

    bb_object_class_install_property(
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

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_DRAWING_TOOL,
        properties[PROP_DRAWING_TOOL] = g_param_spec_object(
            "drawing-tool",
            "",
            "",
            BB_TYPE_DRAWING_TOOL,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_TOOL_CHANGER,
        properties[PROP_TOOL_CHANGER] = g_param_spec_object(
            "tool-changer",
            "",
            "",
            BB_TYPE_TOOL_CHANGER,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
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
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_copy");
}


void
bb_schematic_window_cut(BbSchematicWindow *window)
{
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_cut");
}


void
bb_schematic_window_delete(BbSchematicWindow *window)
{
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_delete");
}


static void
bb_schematic_window_dispose(GObject *object)
{
    //BbSchematicWindow* privat = BBSCHEMATIC_WINDOW_GET_PRIVATE(object);
}


static void
bb_schematic_window_draw_cb(BbSchematicWindowInner *inner, cairo_t *cairo, BbSchematicWindow *outer)
{
    g_return_if_fail(cairo != NULL);
    g_return_if_fail(BB_IS_SCHEMATIC_WINDOW(outer));

    GtkStyleContext *style = gtk_widget_get_style_context(outer);
    BbGraphics *graphics = bb_graphics_new(cairo, style);

    if (outer->drawing_tool != NULL)
    {
        bb_drawing_tool_draw(outer->drawing_tool, graphics);
    }

    cairo_stroke(cairo);
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

    return TRUE; // (g_hash_table_size(window->selection) > 0);
}


gboolean
bb_schematic_window_get_can_cut(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return TRUE; // (g_hash_table_size(window->selection) > 0);
}


gboolean
bb_schematic_window_get_can_delete(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return TRUE; // (g_hash_table_size(window->selection) > 0);
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

    return TRUE; //(window->redo_stack != NULL);
}


gboolean
bb_schematic_window_get_can_reload(BbSchematicWindow *window)
{
    g_warn_if_fail(window != NULL);

    return TRUE;
}


gboolean
bb_schematic_window_get_can_save(BbSchematicWindow *window)
{
    g_warn_if_fail(window != NULL);

    return TRUE;
}


gboolean
bb_schematic_window_get_can_save_as(BbSchematicWindow *window)
{
    g_warn_if_fail(window != NULL);

    return TRUE;
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

    return TRUE; //(window->undo_stack != NULL);
}


BbDrawingTool*
bb_schematic_window_get_drawing_tool(BbSchematicWindow *window)
{
    g_return_val_if_fail(window != NULL, NULL);

    return window->drawing_tool;
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

        case PROP_DRAWING_TOOL:
            g_value_set_object(value, bb_schematic_window_get_drawing_tool(window));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_schematic_window_init(BbSchematicWindow *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));

    window->schematic = bb_schematic_new();

    window->redo_stack = NULL;
    window->selection = g_hash_table_new(g_direct_hash, g_direct_equal);
    window->undo_stack = NULL;

    gtk_widget_add_events(
        GTK_WIDGET(window->inner_window),
        GDK_BUTTON_PRESS_MASK |
            GDK_BUTTON_RELEASE_MASK |
            GDK_KEY_PRESS_MASK |
            GDK_KEY_RELEASE_MASK |
            GDK_POINTER_MOTION_MASK
        );

    g_signal_connect(
        window->inner_window,
        "button-press-event",
        G_CALLBACK(bb_schematic_window_button_pressed_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "button-release-event",
        G_CALLBACK(bb_schematic_window_button_released_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "key-press-event",
        G_CALLBACK(bb_schematic_window_key_pressed_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "key-release-event",
        G_CALLBACK(bb_schematic_window_key_released_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "motion-notify-event",
        G_CALLBACK(bb_schematic_window_motion_notify_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "draw",
        G_CALLBACK(bb_schematic_window_draw_cb),
        window
        );
}


static void
bb_schematic_window_invalidate_item_cb(BbDrawingTool *tool, BbSchematicItem *item, BbSchematicWindow *window)
{
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->inner_window != NULL);

    // TODO Just invalidate everything until the bounds calculations are working
    gtk_widget_queue_draw(GTK_WIDGET(window->inner_window));
}


static void
bb_schematic_window_invalidate_rect_dev(BbSchematicWindow *window, double x0, double y0, double x1, double y1)
{
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->inner_window != NULL);

    int min_x = floor(MIN(x0, x1));
    int min_y = floor(MIN(y0, y1));
    int max_x = ceil(MAX(x0, x1));
    int max_y = ceil(MAX(y0, y1));
    int width = max_x - min_x + 1;
    int height = max_y - min_y + 1;

    gtk_widget_queue_draw_area(GTK_WIDGET(window->inner_window), min_x, min_y, width, height);
}


static gboolean
bb_schematic_window_key_pressed_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbSchematicWindow *window = BB_SCHEMATIC_WINDOW(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    if (window->drawing_tool != NULL)
    {
        bb_drawing_tool_key_pressed(window->drawing_tool);
    }

    return FALSE;
}


static gboolean
bb_schematic_window_key_released_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbSchematicWindow *window = BB_SCHEMATIC_WINDOW(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    if (window->drawing_tool != NULL)
    {
        bb_drawing_tool_key_released(window->drawing_tool);
    }

    return FALSE;
}


static gboolean
bb_schematic_window_motion_notify_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbSchematicWindow *window = BB_SCHEMATIC_WINDOW(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    if (window->drawing_tool != NULL)
    {
        return bb_drawing_tool_motion_notify(window->drawing_tool, event->motion.x, event->motion.y);
    }

    return FALSE;
}


void
bb_schematic_window_paste(BbSchematicWindow *window)
{
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_paste");
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


void
bb_schematic_window_redo(BbSchematicWindow *window)
{
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_redo");
}


void
bb_schematic_window_reload(BbSchematicWindow *window, GError **error)
{
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_reload");
}


__attribute__((constructor)) void
bb_schematic_window_register()
{
    bb_schematic_window_get_type();
}


void
bb_schematic_window_save(BbSchematicWindow *window, GCancellable *cancellable, GError **error)
{
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->file != NULL);
    g_return_if_fail(window->schematic != NULL);

    GFileOutputStream *stream = g_file_replace(
        window->file,
        NULL,
        TRUE,
        G_FILE_CREATE_NONE,
        cancellable,
        error
        );

    if (error == NULL || *error == NULL)
    {
        bb_schematic_write(
            window->schematic,
            G_OUTPUT_STREAM(stream),
            cancellable,
            error
            );
    }
}


void
bb_schematic_window_save_as(BbSchematicWindow *window, GError **error)
{
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_save_as");
}


static void
bb_schematic_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    BbSchematicWindow *window = BB_SCHEMATIC_WINDOW(object);
    g_return_if_fail(window != NULL);

    switch (property_id)
    {
        case PROP_DRAWING_TOOL:
            bb_schematic_window_set_drawing_tool(BB_SCHEMATIC_WINDOW(object), g_value_get_object(value));
            break;

        case PROP_TOOL_CHANGER:
            bb_schematic_window_set_tool_changer(BB_SCHEMATIC_WINDOW(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_schematic_window_select_all(BbSchematicWindow *window)
{
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_select_all");
}


void
bb_schematic_window_select_none(BbSchematicWindow *window)
{
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_select_none");
}


void
bb_schematic_window_set_drawing_tool(BbSchematicWindow *window, BbDrawingTool *tool)
{
    g_return_if_fail(window != NULL);

    if (window->drawing_tool != tool)
    {
        if (window->drawing_tool != NULL)
        {
            g_signal_handlers_disconnect_by_func(
                window->drawing_tool,
                G_CALLBACK(bb_schematic_window_invalidate_item_cb),
                window
                );

            g_object_unref(window->drawing_tool);
        }

        window->drawing_tool = tool;

        if (window->drawing_tool != NULL)
        {
            g_object_ref(window->drawing_tool);

            g_signal_connect_object(
                window->drawing_tool,
                "invalidate-item",
                G_CALLBACK(bb_schematic_window_invalidate_item_cb),
                window,
                0
                );
        }

        g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_DRAWING_TOOL]);
    }
}


void
bb_schematic_window_set_tool_changer(BbSchematicWindow *window, BbToolChanger *tool_changer)
{
    g_return_if_fail(window != NULL);

    if (window->tool_changer != tool_changer)
    {
        bb_schematic_window_set_drawing_tool(window, NULL);

        if (window->tool_changer != NULL)
        {
            g_signal_handlers_disconnect_by_func(
                tool_changer,
                G_CALLBACK(bb_schematic_window_tool_changed_cb),
                window
                );

            g_object_unref(window->tool_changer);
        }

        window->tool_changer = tool_changer;

        if (window->tool_changer != NULL)
        {
            g_object_ref(window->tool_changer);

            g_signal_connect_object(
                tool_changer,
                "tool-changed",
                G_CALLBACK(bb_schematic_window_tool_changed_cb),
                window,
                0
                );

            bb_schematic_window_set_drawing_tool(
                window,
                bb_tool_changer_create_tool(tool_changer, BB_TOOL_SUBJECT(window))
                );
        }

        g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_DRAWING_TOOL]);
    }
}


static void
bb_schematic_window_tool_changed_cb(BbToolChanger *changer, BbSchematicWindow *window)
{
    bb_schematic_window_set_drawing_tool(
        window,
        bb_tool_changer_create_tool(
            changer,
            BB_TOOL_SUBJECT(window)
            )
        );
}


static void
bb_schematic_window_tool_subject_init(BbToolSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->add_item = bb_schematic_window_add_item;
    iface->invalidate_rect_dev = bb_schematic_window_invalidate_rect_dev;
}


void
bb_schematic_window_undo(BbSchematicWindow *window)
{
    g_return_if_fail(window != NULL);

    g_message("bb_schematic_window_undo");
}


static void
bb_schematic_window_zoom_extents(BbZoomSubject *zoom_subject)
{
    g_message("bb_schematic_window_zoom_extents");
}


static void
bb_schematic_window_zoom_in(BbZoomSubject *zoom_subject)
{
    g_message("bb_schematic_window_zoom_in");
}


static void
bb_schematic_window_zoom_out(BbZoomSubject *zoom_subject)
{
    g_message("bb_schematic_window_zoom_out");
}


static void
bb_schematic_window_zoom_subject_init(BbZoomSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->zoom_extents = bb_schematic_window_zoom_extents;
    iface->zoom_in = bb_schematic_window_zoom_in;
    iface->zoom_out = bb_schematic_window_zoom_out;
}
