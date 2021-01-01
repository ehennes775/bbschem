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

#include <math.h>
#include <gtk/gtk.h>
#include <bbextensions.h>
#include <bblibrary.h>
#include <bbpropertysubject.h>
#include "bbgedaeditor.h"
#include "bbschematicwindowinner.h"
#include "bbarctool.h"
#include "bbtoolchanger.h"
#include "bbgraphics.h"
#include "bbzoomsubject.h"
#include "bbrevealsubject.h"
#include "bbgridsubject.h"
#include "bbclipboardsubject.h"
#include "bbpandirection.h"
#include "bbsavesubject.h"
#include "bbgrid.h"
#include "bbgridcontrol.h"




/**
 * The limit of zooming out
 *
 * This value must be greater than 1 / (1 - BB_ZOOM_IN_FACTOR) to be able to zoom back out again.
 */
#define BB_LIMIT_ZOOM_OUT (5.0)


/**
 * The upper limit of zooming in
 */
#define BB_LIMIT_ZOOM_IN (4000.0)


/**
 * The zoom factor for a pan operation (i.e. no zoom)
 */
#define BB_PAN_ZOOM_FACTOR (1.0)


/**
 * The zoom factor for zooming in with actions from hotkeys or menu items
 */
#define BB_ZOOM_IN_FACTOR (1.25)


/**
 * The zoom factor for zooming out with actions from hotkeys or menu items
 */
#define BB_ZOOM_OUT_FACTOR (0.8)


/**
 * A number used to snap the grid to even pixels
 *
 * The ratio of BB_LIMIT_ZOOM_OUT to this value limits how far the user can zoom out.
 */
#define BB_ZOOM_QUANTIZE (200.0)


enum
{
    PROP_0,
    PROP_CAN_COPY,
    PROP_CAN_CUT,
    PROP_CAN_DELETE,
    PROP_CAN_PASTE,
    PROP_CAN_REDO,
    PROP_CAN_SAVE,
    PROP_CAN_SAVE_AS,
    PROP_CAN_SCALE_DOWN,
    PROP_CAN_SCALE_UP,
    PROP_CAN_SELECT_ALL,
    PROP_CAN_SELECT_NONE,
    PROP_CAN_UNDO,
    PROP_CAN_ZOOM_EXTENTS,
    PROP_CAN_ZOOM_IN,
    PROP_CAN_ZOOM_OUT,
    PROP_DRAWING_TOOL,
    PROP_REVEAL,

    PROP_GRID_CONTROL,
    PROP_SCHEMATIC,
    PROP_TOOL_CHANGER,

    N_PROPERTIES
};


enum
{
    SIG_GRID_CHANGED,
    N_SIGNALS
};


struct _BbGedaEditor
{
    BbDocumentWindow parent;

    BbSchematicWindowInner *inner_window;

    /**
     * Stores the current drawing tool for this window.
     */
    BbDrawingTool *drawing_tool;

    BbSchematic *schematic;

    GFile *file;

    /**
     * The grid for this schematic window
     */
    BbGrid *grid;

    /**
     *
     */
    BbGridControl *grid_control;

    /**
     * Stores the last x coordinate, in widget space, from events that provide coordinates (e.g. button and motion
     * events). See corresponding _BbGedaEditor.last_y. These coordinates are used to process events that
     * don't provide coordinates (e.g. key press and key release).
     *
     * @ref _BbGedaEditor.last_y
     */
    double last_x;

    /**
     * @ref _BbGedaEditor.last_x
     */
    double last_y;

    /**
     * The matrix for converting user (i.e. schematic) coordinates to widget coordinates.
     */
    cairo_matrix_t matrix;

    GSList *redo_stack;

    /**
     * Indicates that hidden objects on the schematic should be revealed.
     */
    gboolean reveal;

    GHashTable *selection;

    BbToolChanger *tool_changer;

    GSList *undo_stack;
};


static void
bb_geda_editor_add_item(BbToolSubject *subject, BbGedaItem *item);

static void
bb_geda_editor_bounds_calculator_init(BbBoundsCalculatorInterface *iface);

static gboolean
bb_geda_editor_button_pressed_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static gboolean
bb_geda_editor_button_released_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static void
bb_geda_editor_clipboard_subject_init(BbClipboardSubjectInterface *iface);

static void
bb_geda_editor_dispose(GObject *object);

static void
bb_geda_editor_draw_cb(BbSchematicWindowInner *inner, cairo_t *cairo, BbGedaEditor *outer);

static void
bb_geda_editor_finalize(GObject *object);

static gboolean
bb_geda_editor_get_can_scale_down(BbGridSubject *grid_subject);

static gboolean
bb_geda_editor_get_can_scale_reset(BbGridSubject *grid_subject);

static gboolean
bb_geda_editor_get_can_scale_up(BbGridSubject *grid_subject);

static BbGridControl*
bb_geda_editor_get_grid_control(BbGedaEditor *grid_control);

static void
bb_geda_editor_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static gboolean
bb_geda_editor_get_reveal(BbRevealSubject *reveal_subject);

static void
bb_geda_editor_grid_subject_init(BbGridSubjectInterface *iface);

static void
bb_geda_editor_invalidate_all(BbToolSubject *tool_subject);

static void
bb_geda_editor_invalidate_item_cb(gpointer unused, BbGedaItem *item, BbGedaEditor *window);

static void
bb_geda_editor_invalidate_rect_dev(BbToolSubject *tool_subject, double x0, double y0, double x1, double y1);

static gboolean
bb_geda_editor_key_pressed_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static gboolean
bb_geda_editor_key_released_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data);

static gboolean
bb_geda_editor_motion_notify_cb(GtkWidget *widget, GdkEvent  *event, gpointer user_data);

static void
bb_geda_editor_notify_grid_control_cb(BbGrid *grid, GParamSpec *pspec, BbGedaEditor *window);

static void
bb_geda_editor_reveal_subject_init(BbRevealSubjectInterface *iface);

static void
bb_geda_editor_save_subject_init(BbSaveSubjectInterface *iface);

static void
bb_geda_editor_set_grid(BbGedaEditor *window, BbGrid *grid);

static void
bb_geda_editor_set_grid_control(BbGedaEditor *window, BbGridControl *grid_control);

static void
bb_geda_editor_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_geda_editor_set_reveal(BbRevealSubject *reveal_subject, gboolean reveal);

static void
bb_geda_editor_set_schematic(BbGedaEditor *window, BbSchematic *schematic);

static void
bb_geda_editor_snap_coordinate(BbToolSubject *subject, int x0, int y0, int *x1, int *y1);

static void
bb_geda_editor_tool_changed_cb(BbToolChanger *changer, BbGedaEditor *window);

static void
bb_geda_editor_tool_subject_init(BbToolSubjectInterface *iface);

static void
bb_geda_editor_undo(BbClipboardSubject *clipboard_subject);

static void
bb_geda_editor_user_to_widget_distance(BbToolSubject *subject, double ux, double uy, double *wx, double *wy);

static gboolean
bb_geda_editor_widget_to_user(BbToolSubject *tool_subject, double wx, double wy, double *ux, double *uy);

static void
bb_geda_editor_zoom_box(BbToolSubject *tool_subject, double x0, double y0, double x1, double y1);

static void
bb_geda_editor_zoom_extents(BbZoomSubject *zoom_subject);

static void
bb_geda_editor_zoom_in(BbZoomSubject *zoom_subject);

static void
bb_geda_editor_zoom_out(BbZoomSubject *zoom_subject);

static void
bb_geda_editor_zoom_point(BbGedaEditor *editor, double x, double y, double factor);

static void
bb_geda_editor_zoom_subject_init(BbZoomSubjectInterface *iface);


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_DYNAMIC_TYPE_EXTENDED(
    BbGedaEditor,
    bb_geda_editor,
    BB_TYPE_DOCUMENT_WINDOW,
    0,
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_BOUNDS_CALCULATOR, bb_geda_editor_bounds_calculator_init)
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_CLIPBOARD_SUBJECT, bb_geda_editor_clipboard_subject_init)
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_GRID_SUBJECT, bb_geda_editor_grid_subject_init)
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_REVEAL_SUBJECT, bb_geda_editor_reveal_subject_init)
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_SAVE_SUBJECT, bb_geda_editor_save_subject_init)
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_TOOL_SUBJECT, bb_geda_editor_tool_subject_init)
    G_IMPLEMENT_INTERFACE_DYNAMIC(BB_TYPE_ZOOM_SUBJECT, bb_geda_editor_zoom_subject_init)
    )

// region From BbBoundsCalculator interface

static BbBounds*
bb_geda_editor_calculate_from_corners(BbBoundsCalculator *calculator, int x0, int y0, int x1, int y1, int width)
{
    BbBounds *temp = bb_bounds_new_with_points(x0, y0, x1, y1);

    // TODO expand with the width

    return temp;
}


static void
bb_geda_editor_bounds_calculator_init(BbBoundsCalculatorInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->calculate_from_corners = bb_geda_editor_calculate_from_corners;
}

// endregion

// region From BbClipboardSubject interface

static gboolean
bb_geda_editor_get_can_copy(BbClipboardSubject *clipboard_subject)
{
    g_return_val_if_fail(clipboard_subject != NULL, FALSE);

    return TRUE; // (g_hash_table_size(window->selection) > 0);
}


static gboolean
bb_geda_editor_get_can_cut(BbClipboardSubject *clipboard_subject)
{
    g_return_val_if_fail(clipboard_subject != NULL, FALSE);

    return TRUE; // (g_hash_table_size(window->selection) > 0);
}


static gboolean
bb_geda_editor_get_can_delete(BbClipboardSubject *clipboard_subject)
{
    g_return_val_if_fail(clipboard_subject != NULL, FALSE);

    return TRUE; // (g_hash_table_size(window->selection) > 0);
}


static gboolean
bb_geda_editor_get_can_paste(BbClipboardSubject *clipboard_subject)
{
    g_return_val_if_fail(clipboard_subject != NULL, FALSE);

    return TRUE;
}


static gboolean
bb_geda_editor_get_can_redo(BbClipboardSubject *clipboard_subject)
{
    g_return_val_if_fail(clipboard_subject != NULL, FALSE);

    return TRUE; //(window->redo_stack != NULL);
}


static gboolean
bb_geda_editor_get_can_select_all(BbClipboardSubject *clipboard_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(clipboard_subject);

    g_return_val_if_fail(window != NULL, FALSE);
    g_return_val_if_fail(window->selection != NULL, FALSE);

    return TRUE;
}


static gboolean
bb_geda_editor_get_can_select_none(BbClipboardSubject *clipboard_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(clipboard_subject);

    g_return_val_if_fail(window != NULL, FALSE);
    g_return_val_if_fail(window->selection != NULL, FALSE);

    return (g_hash_table_size(window->selection) > 0);
}


static gboolean
bb_geda_editor_get_can_undo(BbClipboardSubject *clipboard_subject)
{
    g_return_val_if_fail(clipboard_subject != NULL, FALSE);

    return TRUE; //(window->undo_stack != NULL);
}


static void
bb_geda_editor_copy(BbClipboardSubject *clipboard_subject)
{
    g_return_if_fail(clipboard_subject != NULL);

    g_message("bb_geda_editor_copy");
}


static void
bb_geda_editor_cut(BbClipboardSubject *clipboard_subject)
{
    g_return_if_fail(clipboard_subject != NULL);

    g_message("bb_geda_editor_cut");
}


static void
bb_geda_editor_delete(BbClipboardSubject *clipboard_subject)
{
    g_return_if_fail(clipboard_subject != NULL);

    g_message("bb_geda_editor_delete");
}


static void
bb_geda_editor_paste(BbClipboardSubject *clipboard_subject)
{
    g_return_if_fail(clipboard_subject != NULL);

    g_message("bb_geda_editor_paste");
}


static void
bb_geda_editor_redo(BbClipboardSubject *clipboard_subject)
{
    g_return_if_fail(clipboard_subject != NULL);

    g_message("bb_geda_editor_redo");
}


static void
bb_geda_editor_select_all(BbClipboardSubject *clipboard_subject)
{
    g_return_if_fail(clipboard_subject != NULL);

    g_message("bb_geda_editor_select_all");
}


static void
bb_geda_editor_select_none(BbClipboardSubject *clipboard_subject)
{
    g_return_if_fail(clipboard_subject != NULL);

    g_message("bb_geda_editor_select_none");
}


static void
bb_geda_editor_undo(BbClipboardSubject *clipboard_subject)
{
    g_return_if_fail(clipboard_subject != NULL);

    g_message("bb_geda_editor_undo");
}


static void
bb_geda_editor_clipboard_subject_init(BbClipboardSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_can_copy = bb_geda_editor_get_can_copy;
    iface->get_can_cut = bb_geda_editor_get_can_cut;
    iface->get_can_delete = bb_geda_editor_get_can_delete;
    iface->get_can_paste = bb_geda_editor_get_can_paste;
    iface->get_can_redo = bb_geda_editor_get_can_redo;
    iface->get_can_select_all = bb_geda_editor_get_can_select_all;
    iface->get_can_select_none = bb_geda_editor_get_can_select_none;
    iface->get_can_undo = bb_geda_editor_get_can_undo;
    iface->copy = bb_geda_editor_copy;
    iface->cut = bb_geda_editor_cut;
    iface->delete = bb_geda_editor_delete;
    iface->paste = bb_geda_editor_paste;
    iface->redo = bb_geda_editor_redo;
    iface->select_all = bb_geda_editor_select_all;
    iface->select_none = bb_geda_editor_select_none;
    iface->undo = bb_geda_editor_undo;
}

// endregion

// region From BbGridSubject interface

static gboolean
bb_geda_editor_get_can_scale(BbGridSubject *grid_subject, BbScaleGridDirection direction)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(grid_subject);
    g_return_val_if_fail(window != NULL, FALSE);

    return bb_grid_get_can_scale(window->grid, direction);
}


static gboolean
bb_geda_editor_get_can_scale_down(BbGridSubject *grid_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(grid_subject);
    g_return_val_if_fail(window != NULL, FALSE);

    return bb_grid_get_can_scale_down(window->grid);
}


static gboolean
bb_geda_editor_get_can_scale_reset(BbGridSubject *grid_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(grid_subject);
    g_return_val_if_fail(window != NULL, FALSE);

    return bb_grid_get_can_scale_reset(window->grid);
}


static gboolean
bb_geda_editor_get_can_scale_up(BbGridSubject *grid_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(grid_subject);
    g_return_val_if_fail(window != NULL, FALSE);

    return bb_grid_get_can_scale_up(window->grid);
}


static void
bb_geda_editor_scale(BbGridSubject *grid_subject, BbScaleGridDirection direction)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(grid_subject);
    g_return_if_fail(window != NULL);

    bb_grid_scale(window->grid, direction);
}


static void
bb_geda_editor_scale_down(BbGridSubject *grid_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(grid_subject);
    g_return_if_fail(window != NULL);

    bb_grid_scale_down(window->grid);
}


static void
bb_geda_editor_scale_reset(BbGridSubject *grid_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(grid_subject);
    g_return_if_fail(window != NULL);

    bb_grid_scale_reset(window->grid);
}


static void
bb_geda_editor_scale_up(BbGridSubject *grid_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(grid_subject);
    g_return_if_fail(window != NULL);

    bb_grid_scale_up(window->grid);
}


static void
bb_geda_editor_grid_subject_init(BbGridSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->can_scale = bb_geda_editor_get_can_scale;
    iface->can_scale_down = bb_geda_editor_get_can_scale_down;
    iface->can_scale_reset = bb_geda_editor_get_can_scale_reset;
    iface->can_scale_up = bb_geda_editor_get_can_scale_up;
    iface->scale = bb_geda_editor_scale;
    iface->scale_down = bb_geda_editor_scale_down;
    iface->scale_reset = bb_geda_editor_scale_reset;
    iface->scale_up = bb_geda_editor_scale_up;
}

// endregion

// region From BbPropertySubject

static void
bb_geda_editor_apply_selection(BbPropertySubject *subject, BbApplyFunc func, gpointer user_data)
{
    BbGedaEditor *editor = BB_GEDA_EDITOR(subject);
    g_return_if_fail(BB_IS_GEDA_EDITOR(editor));

    bb_schematic_foreach_modify(
        editor->schematic,
        bb_pred_always,
        NULL,
        func,
        user_data
    );
}


static void
bb_geda_editor_query_selection(BbPropertySubject *subject, BbQueryFunc func, gpointer user_data)
{
    BbGedaEditor *editor = BB_GEDA_EDITOR(subject);
    g_return_if_fail(BB_IS_GEDA_EDITOR(editor));

    bb_schematic_foreach_query(
        editor->schematic,
        bb_pred_always,
        NULL,
        func,
        user_data
    );
}


static void
bb_geda_editor_property_subject_init(BbPropertySubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->apply_selection = bb_geda_editor_apply_selection;
    iface->query_selection = bb_geda_editor_query_selection;
}


// endregion

// region From BbRevealSubject interface

static gboolean
bb_geda_editor_get_reveal(BbRevealSubject *reveal_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(reveal_subject);
    g_return_val_if_fail(window != NULL, FALSE);

    return window->reveal;
}


static void
bb_geda_editor_set_reveal(BbRevealSubject *reveal_subject, gboolean reveal)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(reveal_subject);
    g_return_if_fail(window != NULL);

    if (!bb_boolean_equals(window->reveal, reveal))
    {
        window->reveal = reveal;

        gtk_widget_queue_draw(GTK_WIDGET(window->inner_window));

        g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_REVEAL]);
    }
}


static void
bb_geda_editor_reveal_subject_init(BbRevealSubjectInterface *iface)
{
    iface->get_reveal = bb_geda_editor_get_reveal;
    iface->set_reveal = bb_geda_editor_set_reveal;
}

// endregion

// region From BbSaveSubject interface

static gboolean
bb_geda_editor_get_can_save(BbSaveSubject *subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(subject);
    g_return_val_if_fail(window != NULL, FALSE);

    return TRUE;
}


static gboolean
bb_geda_editor_get_can_save_as(BbSaveSubject *subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(subject);
    g_return_val_if_fail(window != NULL, FALSE);

    return TRUE;
}


static void
bb_geda_editor_save(BbSaveSubject *subject, GError **error)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(subject);
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->file != NULL);
    g_return_if_fail(window->schematic != NULL);

    GError *local_error = NULL;

    GFileOutputStream *stream = g_file_replace(
        window->file,
        NULL,
        TRUE,
        G_FILE_CREATE_NONE,
        NULL,
        &local_error
    );

    if (local_error == NULL)
    {
        bb_schematic_write(
            window->schematic,
            G_OUTPUT_STREAM(stream),
            NULL,
            &local_error
        );
    }

    g_clear_object(&stream);

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
    }
}


static void
bb_geda_editor_save_as(BbSaveSubject *subject, GError **error)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(subject);
    g_return_if_fail(window != NULL);

    GError *local_error = NULL;

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
        "Save Schematic As",
        GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(window))),
        GTK_FILE_CHOOSER_ACTION_SAVE,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Save", GTK_RESPONSE_ACCEPT,
        NULL
    );

    gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);

    // TODO Add file filters

    if (window->file == NULL)
    {
        gtk_file_chooser_set_current_name(
            GTK_FILE_CHOOSER(dialog),
            bb_document_window_get_tab(BB_DOCUMENT_WINDOW(window))
        );
    }
    else
    {
        gtk_file_chooser_set_file(
            GTK_FILE_CHOOSER(dialog),
            window->file,
            &local_error
        );
    }

    if (local_error == NULL)
    {
        if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
        {
            window->file = gtk_file_chooser_get_file(GTK_FILE_CHOOSER(dialog));

            bb_geda_editor_save(subject, &local_error);
        }
    }

    gtk_widget_destroy(dialog);

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
    }
}

static void
bb_geda_editor_save_subject_init(BbSaveSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_can_save = bb_geda_editor_get_can_save;
    iface->get_can_save_as = bb_geda_editor_get_can_save_as;
    iface->save = bb_geda_editor_save;
    iface->save_as = bb_geda_editor_save_as;
}

// endregion

// region From BbToolSubject interface
// endregion

// region From BbZoomSubject interface

static gboolean
bb_geda_editor_get_can_zoom_in(BbGedaEditor *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return BB_ZOOM_QUANTIZE * window->matrix.xx < BB_LIMIT_ZOOM_IN;
}


static gboolean
bb_geda_editor_get_can_zoom_out(BbGedaEditor *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return BB_ZOOM_QUANTIZE * window->matrix.xx > BB_LIMIT_ZOOM_OUT;
}


static void
bb_geda_editor_pan(BbZoomSubject *zoom_subject, BbPanDirection direction)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(zoom_subject);
    g_return_if_fail(window != NULL);

    double center_x = gtk_widget_get_allocated_width(GTK_WIDGET(window->inner_window)) / 2.0;
    double center_y = gtk_widget_get_allocated_height(GTK_WIDGET(window->inner_window)) / 2.0;

    double step = 100.0;

    switch (direction)
    {
        case BB_PAN_DIRECTION_DOWN:
            center_y += step;
            break;

        case BB_PAN_DIRECTION_LEFT:
            center_x -= step;
            break;

        case BB_PAN_DIRECTION_RIGHT:
            center_x += step;
            break;

        case BB_PAN_DIRECTION_UP:
            center_y -= step;
            break;

        default:
            g_return_if_reached();
    }

    bb_geda_editor_zoom_point(window, center_x, center_y, BB_PAN_ZOOM_FACTOR);
}


static void
bb_geda_editor_pan_point(BbZoomSubject *zoom_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(zoom_subject);
    g_return_if_fail(window != NULL);

    bb_geda_editor_zoom_point(window, window->last_x, window->last_y, BB_PAN_ZOOM_FACTOR);
}


static void
bb_geda_editor_zoom_box(BbToolSubject *tool_subject, double x0, double y0, double x1, double y1)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(tool_subject);
    g_return_if_fail(window != NULL);

    double width = ABS(x1 - x0);
    double height = ABS(y1 - y0);

    if (width >= 5 && height >= 5)
    {
        double center_x = (x1 + x0) / 2.0;
        double center_y = (y1 + y0) / 2.0;

        int widget_width = gtk_widget_get_allocated_width(GTK_WIDGET(window->inner_window));
        int widget_height = gtk_widget_get_allocated_height(GTK_WIDGET(window->inner_window));

        double scale_x = widget_width / width;
        double scale_y = widget_height / height;
        double scale = MIN(scale_x, scale_y);

        bb_geda_editor_zoom_point(window, center_x, center_y, scale);
    }
}


static void
bb_geda_editor_zoom_extents(BbZoomSubject *zoom_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(zoom_subject);
    g_return_if_fail(window != NULL);

    int width = gtk_widget_get_allocated_width(GTK_WIDGET(window->inner_window));
    int height = gtk_widget_get_allocated_height(GTK_WIDGET(window->inner_window));

    BbBounds *bounds = bb_bounds_new_with_points(0, 0, 100, 100);

    if (window->schematic != NULL)
    {
        bb_schematic_calculate_bounds(
            window->schematic,
            bb_pred_always,
            NULL,
            BB_BOUNDS_CALCULATOR(window),
            bounds
        );
    }

    double scale_x = 0.9 * width / MAX(abs(bounds->max_x - bounds->min_x), 100.0);
    double scale_y = 0.9 * height / MAX(abs(bounds->max_y - bounds->min_y), 100.0);
    double scale = MIN(scale_x, scale_y);

    cairo_matrix_t matrix;
    cairo_matrix_init_identity(&matrix);
    cairo_matrix_translate(&matrix, width / 2.0, height / 2.0);
    cairo_matrix_scale(&matrix, scale, -scale);
    cairo_matrix_translate(&matrix, (bounds->max_x + bounds->min_x) / -2.0, (bounds->max_y + bounds->min_y) / -2.0);

    /* Snap zoom to even pixels and clamp to maximum zoom out */
    matrix.xx = CLAMP(trunc(matrix.xx * BB_ZOOM_QUANTIZE), BB_LIMIT_ZOOM_OUT, BB_LIMIT_ZOOM_IN) / BB_ZOOM_QUANTIZE;
    matrix.yy = CLAMP(trunc(matrix.yy * BB_ZOOM_QUANTIZE), -BB_LIMIT_ZOOM_IN, -BB_LIMIT_ZOOM_OUT) / BB_ZOOM_QUANTIZE;

    /* Snap coordinate translation to even pixels */
    matrix.x0 = floor(matrix.x0) + 0.5;
    matrix.y0 = floor(matrix.y0) + 0.5;

    window->matrix = matrix;

    g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_CAN_ZOOM_IN]);
    g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_CAN_ZOOM_OUT]);

    gtk_widget_queue_draw(GTK_WIDGET(window->inner_window));
}


static void
bb_geda_editor_zoom_in(BbZoomSubject *zoom_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(zoom_subject);
    g_return_if_fail(window != NULL);

    double center_x = gtk_widget_get_allocated_width(GTK_WIDGET(window->inner_window)) / 2.0;
    double center_y = gtk_widget_get_allocated_height(GTK_WIDGET(window->inner_window)) / 2.0;

    bb_geda_editor_zoom_point(window, center_x, center_y, BB_ZOOM_IN_FACTOR);
}


static void
bb_geda_editor_zoom_out(BbZoomSubject *zoom_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(zoom_subject);
    g_return_if_fail(window != NULL);

    double center_x = gtk_widget_get_allocated_width(GTK_WIDGET(window->inner_window)) / 2.0;
    double center_y = gtk_widget_get_allocated_height(GTK_WIDGET(window->inner_window)) / 2.0;

    bb_geda_editor_zoom_point(window, center_x, center_y, BB_ZOOM_OUT_FACTOR);
}


/**
 *
 *
 * @param editor
 * @param x The x coordinate in widget space
 * @param y The y coordinate in widget space
 * @param factor The relative zoom factor
 */
static void
bb_geda_editor_zoom_point(BbGedaEditor *editor, double x, double y, double factor)
{
    g_return_if_fail((factor <= 1.0) || bb_geda_editor_get_can_zoom_in(editor));
    g_return_if_fail((factor >= 1.0) || bb_geda_editor_get_can_zoom_out(editor));

    cairo_matrix_t matrix;

    double center_x = gtk_widget_get_allocated_width(GTK_WIDGET(editor->inner_window)) / 2.0;
    double center_y = gtk_widget_get_allocated_height(GTK_WIDGET(editor->inner_window)) / 2.0;

    cairo_matrix_init_identity(&matrix);
    cairo_matrix_multiply(&matrix, &matrix, &editor->matrix);

    matrix.xx *= factor;
    matrix.yy *= factor;

    matrix.x0 = factor * (matrix.x0 - x) + center_x;
    matrix.y0 = factor * (matrix.y0 - y) + center_y;

    /* Snap zoom to even pixels and clamp to maximum zoom out */
    matrix.xx = CLAMP(trunc(matrix.xx * BB_ZOOM_QUANTIZE), BB_LIMIT_ZOOM_OUT, BB_LIMIT_ZOOM_IN) / BB_ZOOM_QUANTIZE;
    matrix.yy = CLAMP(trunc(matrix.yy * BB_ZOOM_QUANTIZE), -BB_LIMIT_ZOOM_IN, -BB_LIMIT_ZOOM_OUT) / BB_ZOOM_QUANTIZE;

    /* Snap coordinate translation to even pixels */
    matrix.x0 = floor(matrix.x0) + 0.5;
    matrix.y0 = floor(matrix.y0) + 0.5;

    editor->matrix = matrix;

    g_object_notify_by_pspec(G_OBJECT(editor), properties[PROP_CAN_ZOOM_IN]);
    g_object_notify_by_pspec(G_OBJECT(editor), properties[PROP_CAN_ZOOM_OUT]);

    gtk_widget_queue_draw(GTK_WIDGET(editor->inner_window));
}


static void
bb_geda_editor_zoom_point2(BbZoomSubject *zoom_subject, BbZoomDirection direction)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(zoom_subject);
    g_return_if_fail(window != NULL);

    switch (direction)
    {
        case BB_ZOOM_DIRECTION_IN:
            bb_geda_editor_zoom_point(window, window->last_x, window->last_y, BB_ZOOM_IN_FACTOR);
            break;

        case BB_ZOOM_DIRECTION_OUT:
            bb_geda_editor_zoom_point(window, window->last_x, window->last_y, BB_ZOOM_OUT_FACTOR);
            break;

        default:
            g_return_if_reached();
    }
}


static void
bb_geda_editor_zoom_subject_init(BbZoomSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->pan = bb_geda_editor_pan;
    iface->pan_point = bb_geda_editor_pan_point;
    iface->zoom_extents = bb_geda_editor_zoom_extents;
    iface->zoom_in = bb_geda_editor_zoom_in;
    iface->zoom_out = bb_geda_editor_zoom_out;
    iface->zoom_point = bb_geda_editor_zoom_point2;
}

// endregion

static void
bb_geda_editor_add_item(BbToolSubject *subject, BbGedaItem *item)
{
    g_message("bb_geda_editor_add_item");

    BbGedaEditor *window = BB_GEDA_EDITOR(subject);

    g_return_if_fail(window != NULL);
    g_return_if_fail(window->schematic != NULL);

    GSList *items = g_slist_append(NULL, item);

    bb_schematic_add_items(window->schematic, items);
}


static gboolean
bb_geda_editor_button_pressed_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    window->last_x = event->button.x;
    window->last_y = event->button.y;

    if (window->drawing_tool != NULL)
    {
        return bb_drawing_tool_button_pressed(window->drawing_tool, event->button.x, event->button.y);
    }

    return FALSE;
}


static gboolean
bb_geda_editor_button_released_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    window->last_x = event->button.x;
    window->last_y = event->button.y;

    if (window->drawing_tool != NULL)
    {
        return bb_drawing_tool_button_released(window->drawing_tool, event->button.x, event->button.y);
    }

    return FALSE;
}


static void
bb_geda_editor_class_init(BbGedaEditorClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);

    G_OBJECT_CLASS(klasse)->dispose = bb_geda_editor_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_geda_editor_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_geda_editor_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_geda_editor_set_property;

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

    /*
     * The schematic property remains hidden so the undo functionality is not bypassed with direct calls to the
     * schematic object.
     */
    properties[PROP_SCHEMATIC] = bb_object_class_install_property(
        object_class,
        PROP_SCHEMATIC,
        g_param_spec_object(
            "schematic",
            "",
            "",
            BB_TYPE_SCHEMATIC,
            G_PARAM_WRITABLE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_GRID_CONTROL] = bb_object_class_install_property(
        object_class,
        PROP_GRID_CONTROL,
        g_param_spec_object(
            "grid-control",
            "",
            "",
            BB_TYPE_TOOL_CHANGER,
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
        "/com/github/ehennes775/bbschem/bbgedaplugin/bbgedaeditor.ui"
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbGedaEditor,
        inner_window
        );

    /* From BbClipboardSubject */

    properties[PROP_CAN_COPY] = bb_object_class_override_property(
        object_class,
        PROP_CAN_COPY,
        "can-copy"
        );

    properties[PROP_CAN_CUT] = bb_object_class_override_property(
        object_class,
        PROP_CAN_CUT,
        "can-cut"
        );

    properties[PROP_CAN_DELETE] = bb_object_class_override_property(
        object_class,
        PROP_CAN_DELETE,
        "can-delete"
        );

    properties[PROP_CAN_PASTE] = bb_object_class_override_property(
        object_class,
        PROP_CAN_PASTE,
        "can-paste"
        );

    properties[PROP_CAN_REDO] = bb_object_class_override_property(
        object_class,
        PROP_CAN_REDO,
        "can-redo"
        );

    properties[PROP_CAN_SELECT_ALL] = bb_object_class_override_property(
        object_class,
        PROP_CAN_SELECT_ALL,
        "can-select-all"
        );

    properties[PROP_CAN_SELECT_NONE] = bb_object_class_override_property(
        object_class,
        PROP_CAN_SELECT_NONE,
        "can-select-none"
        );

    properties[PROP_CAN_UNDO] = bb_object_class_override_property(
        object_class,
        PROP_CAN_UNDO,
        "can-undo"
        );

    /* From BbGridSubject */

    properties[PROP_CAN_SCALE_DOWN] = bb_object_class_override_property(
        object_class,
        PROP_CAN_SCALE_DOWN,
        "can-scale-down"
        );

    properties[PROP_CAN_SCALE_UP] = bb_object_class_override_property(
        object_class,
        PROP_CAN_SCALE_UP,
        "can-scale-up"
        );

    /* From BbRevealSubject */

    properties[PROP_REVEAL] = bb_object_class_override_property(
        object_class,
        PROP_REVEAL,
        "reveal"
        );

    /* From BbSaveSubject */

    properties[PROP_CAN_SAVE] = bb_object_class_override_property(
        object_class,
        PROP_CAN_SAVE,
        "can-save"
        );

    properties[PROP_CAN_SAVE_AS] = bb_object_class_override_property(
        object_class,
        PROP_CAN_SAVE_AS,
        "can-save-as"
        );

    /* From BbZoomSubject */

    properties[PROP_CAN_ZOOM_EXTENTS] = bb_object_class_override_property(
        object_class,
        PROP_CAN_ZOOM_EXTENTS,
        "can-zoom-extents"
        );

    properties[PROP_CAN_ZOOM_IN] = bb_object_class_override_property(
        object_class,
        PROP_CAN_ZOOM_IN,
        "can-zoom-in"
        );

    properties[PROP_CAN_ZOOM_OUT] = bb_object_class_override_property(
        object_class,
        PROP_CAN_ZOOM_OUT,
        "can-zoom-out"
        );

    signals[SIG_GRID_CHANGED] = g_signal_lookup(
        "grid-changed",
        BB_TYPE_GRID_SUBJECT
        );
}

static void
bb_geda_editor_class_finalize(BbGedaEditorClass *klasse)
{
}


static void
bb_geda_editor_dispose(GObject *object)
{
}


static void
bb_geda_editor_draw_cb(BbSchematicWindowInner *inner, cairo_t *cairo, BbGedaEditor *outer)
{
    g_return_if_fail(cairo != NULL);
    g_return_if_fail(BB_IS_GEDA_EDITOR(outer));

    cairo_matrix_t widget_matrix;
    cairo_get_matrix(cairo, &widget_matrix);

    GtkStyleContext *style = gtk_widget_get_style_context(GTK_WIDGET(outer));
    BbGraphics *graphics = bb_graphics_new(cairo, &widget_matrix, outer->reveal, style);

    cairo_save(cairo);
    cairo_transform(cairo, &outer->matrix);

    if (outer->grid != NULL & (outer->grid_control == NULL || bb_grid_control_get_grid_visible(outer->grid_control)))
    {
        bb_grid_draw(outer->grid, graphics);
    }

    if (outer->schematic != NULL)
    {
        bb_schematic_render(outer->schematic, BB_ITEM_RENDERER(graphics));
    }

    // TODO remove
    cairo_stroke(cairo);

    if (outer->drawing_tool != NULL)
    {
        bb_drawing_tool_draw(outer->drawing_tool, graphics);
    }

    // TODO remove
    cairo_stroke(cairo);

    cairo_restore(cairo);
}


static void
bb_geda_editor_finalize(GObject *object)
{
}


gboolean
bb_geda_editor_get_can_reload(BbGedaEditor *window)
{
    g_warn_if_fail(window != NULL);

    return TRUE;
}


static gboolean
bb_geda_editor_get_can_zoom_extents(BbGedaEditor *window)
{
    g_return_val_if_fail(window != NULL, FALSE);

    return TRUE;
}


BbDrawingTool*
bb_geda_editor_get_drawing_tool(BbGedaEditor *window)
{
    g_return_val_if_fail(window != NULL, NULL);

    return window->drawing_tool;
}


static BbGridControl*
bb_geda_editor_get_grid_control(BbGedaEditor *window)
{
    g_return_val_if_fail(BB_IS_GEDA_EDITOR(window), NULL);

    return window->grid_control;
}


static void
bb_geda_editor_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(object);
    g_return_if_fail(window != NULL);

    switch (property_id)
    {
        case PROP_CAN_COPY:
            g_value_set_boolean(value, bb_geda_editor_get_can_copy(BB_CLIPBOARD_SUBJECT(object)));
            break;

        case PROP_CAN_CUT:
            g_value_set_boolean(value, bb_geda_editor_get_can_cut(BB_CLIPBOARD_SUBJECT(object)));
            break;

        case PROP_CAN_DELETE:
            g_value_set_boolean(value, bb_geda_editor_get_can_delete(BB_CLIPBOARD_SUBJECT(object)));
            break;

        case PROP_CAN_PASTE:
            g_value_set_boolean(value, bb_geda_editor_get_can_paste(BB_CLIPBOARD_SUBJECT(object)));
            break;

        case PROP_CAN_REDO:
            g_value_set_boolean(value, bb_geda_editor_get_can_redo(BB_CLIPBOARD_SUBJECT(object)));
            break;

        case PROP_CAN_SAVE:
            g_value_set_boolean(value, bb_geda_editor_get_can_save(BB_SAVE_SUBJECT(object)));
            break;

        case PROP_CAN_SAVE_AS:
            g_value_set_boolean(value, bb_geda_editor_get_can_save_as(BB_SAVE_SUBJECT(object)));
            break;

        case PROP_CAN_SCALE_DOWN:
            g_value_set_boolean(value, bb_geda_editor_get_can_scale_down(BB_GRID_SUBJECT(object)));
            break;

        case PROP_CAN_SCALE_UP:
            g_value_set_boolean(value, bb_geda_editor_get_can_scale_up(BB_GRID_SUBJECT(object)));
            break;

        case PROP_CAN_SELECT_ALL:
            g_value_set_boolean(value, bb_geda_editor_get_can_select_all(BB_CLIPBOARD_SUBJECT(object)));
            break;

        case PROP_CAN_SELECT_NONE:
            g_value_set_boolean(value, bb_geda_editor_get_can_select_none(BB_CLIPBOARD_SUBJECT(object)));
            break;

        case PROP_CAN_UNDO:
            g_value_set_boolean(value, bb_geda_editor_get_can_undo(BB_CLIPBOARD_SUBJECT(object)));
            break;

        case PROP_CAN_ZOOM_EXTENTS:
            g_value_set_boolean(value, bb_geda_editor_get_can_zoom_extents(window));
            break;

        case PROP_CAN_ZOOM_IN:
            g_value_set_boolean(value, bb_geda_editor_get_can_zoom_in(window));
            break;

        case PROP_CAN_ZOOM_OUT:
            g_value_set_boolean(value, bb_geda_editor_get_can_zoom_out(window));
            break;

        case PROP_GRID_CONTROL:
            g_value_set_object(value, bb_geda_editor_get_grid_control(window));
            break;

        case PROP_REVEAL:
            g_value_set_boolean(value, bb_geda_editor_get_reveal(BB_REVEAL_SUBJECT(object)));
            break;

        case PROP_DRAWING_TOOL:
            g_value_set_object(value, bb_geda_editor_get_drawing_tool(window));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_editor_init(BbGedaEditor *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));

    window->schematic = bb_schematic_new();
    bb_geda_editor_set_grid(window, bb_grid_new(BB_TOOL_SUBJECT(window)));
    window->redo_stack = NULL;
    window->selection = g_hash_table_new(g_direct_hash, g_direct_equal);
    window->undo_stack = NULL;

    cairo_matrix_init_identity(&window->matrix);

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
        G_CALLBACK(bb_geda_editor_button_pressed_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "button-release-event",
        G_CALLBACK(bb_geda_editor_button_released_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "key-press-event",
        G_CALLBACK(bb_geda_editor_key_pressed_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "key-release-event",
        G_CALLBACK(bb_geda_editor_key_released_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "motion-notify-event",
        G_CALLBACK(bb_geda_editor_motion_notify_cb),
        window
        );

    g_signal_connect(
        window->inner_window,
        "draw",
        G_CALLBACK(bb_geda_editor_draw_cb),
        window
        );
}


static void
bb_geda_editor_invalidate_all(BbToolSubject *tool_subject)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(tool_subject);

    g_return_if_fail(window != NULL);
    g_return_if_fail(window->inner_window != NULL);

    gtk_widget_queue_draw(GTK_WIDGET(window->inner_window));
}


/**
 * Invalidate a single item
 *
 * @param unused Represents the source, which could be emitted from unrelated classes
 * @param item The item that changed and needs to be invalidated
 * @param window This schematic window
 */
static void
bb_geda_editor_invalidate_item_cb(gpointer unused, BbGedaItem *item, BbGedaEditor *window)
{
    g_return_if_fail(window != NULL);
    g_return_if_fail(window->inner_window != NULL);

    // TODO Just invalidate everything until the bounds calculations are working
    gtk_widget_queue_draw(GTK_WIDGET(window->inner_window));
}


static void
bb_geda_editor_invalidate_rect_dev(BbToolSubject *tool_subject, double x0, double y0, double x1, double y1)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(tool_subject);
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
bb_geda_editor_key_pressed_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    if (window->drawing_tool != NULL)
    {
        bb_drawing_tool_key_pressed(window->drawing_tool);
    }

    return FALSE;
}


static gboolean
bb_geda_editor_key_released_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    if (window->drawing_tool != NULL)
    {
        bb_drawing_tool_key_released(window->drawing_tool);
    }

    return FALSE;
}


static gboolean
bb_geda_editor_motion_notify_cb(GtkWidget *widget, GdkEvent *event, gpointer user_data)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(user_data);
    g_return_val_if_fail(window != NULL, FALSE);

    window->last_x = event->motion.x;
    window->last_y = event->motion.y;

    if (window->drawing_tool != NULL)
    {
        return bb_drawing_tool_motion_notify(window->drawing_tool, event->motion.x, event->motion.y);
    }

    return FALSE;
}


BbGedaEditor*
bb_geda_editor_new(BbSchematic *schematic)
{
    return BB_GEDA_EDITOR(g_object_new(
        BB_TYPE_GEDA_EDITOR,
        "schematic", schematic,
        NULL
        ));
}


static void
bb_geda_editor_notify_grid_cb(BbGrid *grid, GParamSpec *pspec, BbGedaEditor *window)
{
    g_signal_emit(window, signals[SIG_GRID_CHANGED], 0);
}


static void
bb_geda_editor_notify_grid_control_cb(BbGrid *grid, GParamSpec *pspec, BbGedaEditor *window)
{
    gtk_widget_queue_draw(GTK_WIDGET(window));
}


void
bb_geda_editor_reload(BbGedaEditor *window, GError **error)
{
    g_return_if_fail(window != NULL);

    g_message("bb_geda_editor_reload");
}


void
bb_geda_editor_register(GTypeModule *module)
{
    bb_geda_editor_register_type(module);
}


static void
bb_geda_editor_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(BB_IS_GEDA_EDITOR(object));

    switch (property_id)
    {
        case PROP_DRAWING_TOOL:
            bb_geda_editor_set_drawing_tool(BB_GEDA_EDITOR(object), g_value_get_object(value));
            break;

        case PROP_GRID_CONTROL:
            bb_geda_editor_set_grid_control(BB_GEDA_EDITOR(object), g_value_get_object(value));
            break;

        case PROP_REVEAL:
            bb_geda_editor_set_reveal(BB_REVEAL_SUBJECT(object), g_value_get_boolean(value));
            break;

        case PROP_SCHEMATIC:
            bb_geda_editor_set_schematic(BB_GEDA_EDITOR(object), g_value_get_object(value));
            break;

        case PROP_TOOL_CHANGER:
            bb_geda_editor_set_tool_changer(BB_GEDA_EDITOR(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_geda_editor_set_drawing_tool(BbGedaEditor *window, BbDrawingTool *tool)
{
    g_return_if_fail(window != NULL);

    if (window->drawing_tool != tool)
    {
        if (window->drawing_tool != NULL)
        {
            g_signal_handlers_disconnect_by_func(
                window->drawing_tool,
                G_CALLBACK(bb_geda_editor_invalidate_item_cb),
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
                G_CALLBACK(bb_geda_editor_invalidate_item_cb),
                window,
                0
                );
        }

        g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_DRAWING_TOOL]);
    }
}

void
bb_geda_editor_set_grid(BbGedaEditor *window, BbGrid *grid)
{
    g_return_if_fail(BB_IS_GEDA_EDITOR(window));

    if (window->grid != grid)
    {
        if (window->grid != NULL)
        {
            g_signal_handlers_disconnect_by_func(
                window->grid,
                G_CALLBACK(bb_geda_editor_invalidate_item_cb),
                window
                );

            g_object_unref(window->grid);
        }

        window->grid = grid;

        if (window->grid != NULL)
        {
            g_object_ref(window->grid);

            g_signal_connect_object(
                window->grid,
                "notify",
                G_CALLBACK(bb_geda_editor_notify_grid_cb),
                window,
                0
                );
        }

        //g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_GRID]);
    }
}

static void
bb_geda_editor_set_grid_control(BbGedaEditor *window, BbGridControl *grid_control)
{
    g_return_if_fail(BB_IS_GEDA_EDITOR(window));

    if (window->grid_control != grid_control)
    {
        if (window->grid_control != NULL)
        {
            g_signal_handlers_disconnect_by_func(
                window->grid_control,
                G_CALLBACK(bb_geda_editor_notify_grid_control_cb),
                window
                );

            g_object_unref(window->grid_control);
        }

        window->grid_control = grid_control;

        if (window->grid_control != NULL)
        {
            g_object_ref(window->grid_control);

            g_signal_connect(
                window->grid_control,
                "notify::grid-visible",
                G_CALLBACK(bb_geda_editor_notify_grid_control_cb),
                window
                );
        }

        g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_GRID_CONTROL]);
    }
}


static void
bb_geda_editor_set_schematic(BbGedaEditor *window, BbSchematic *schematic)
{
    g_return_if_fail(BB_IS_GEDA_EDITOR(window));

    if (window->schematic != schematic)
    {
        if (window->schematic != NULL)
        {
            g_signal_handlers_disconnect_by_func(
                window->schematic,
                bb_geda_editor_invalidate_item_cb,
                window
                );

            g_object_unref(window->schematic);
        }

        window->schematic = schematic;

        if (window->schematic != NULL)
        {
            g_object_ref(window->schematic);

            g_signal_connect(
                window->schematic,
                "invalidate-item",
                G_CALLBACK(bb_geda_editor_invalidate_item_cb),
                window
                );
        }

        g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_SCHEMATIC]);
    }
}


void
bb_geda_editor_set_tool_changer(BbGedaEditor *window, BbToolChanger *tool_changer)
{
    g_return_if_fail(window != NULL);

    if (window->tool_changer != tool_changer)
    {
        bb_geda_editor_set_drawing_tool(window, NULL);

        if (window->tool_changer != NULL)
        {
            g_signal_handlers_disconnect_by_func(
                tool_changer,
                G_CALLBACK(bb_geda_editor_tool_changed_cb),
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
                G_CALLBACK(bb_geda_editor_tool_changed_cb),
                window,
                0
                );

            bb_geda_editor_set_drawing_tool(
                window,
                bb_tool_changer_create_tool(tool_changer, BB_TOOL_SUBJECT(window))
                );
        }

        g_object_notify_by_pspec(G_OBJECT(window), properties[PROP_DRAWING_TOOL]);
    }
}


static void
bb_geda_editor_snap_coordinate(BbToolSubject *subject, int x0, int y0, int *x1, int *y1)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(subject);

    int grid_size = BB_GRID_DEFAULT_SIZE;

    if (window != NULL && window->grid != NULL)
    {
        grid_size = bb_grid_get_snap_size(window->grid);
    }

    int x = x0;
    int y = y0;

    if (window != NULL && window->grid_control != NULL && bb_grid_control_get_snap_active(window->grid_control))
    {
        x = bb_coord_snap(x0, grid_size);
        y = bb_coord_snap(y0, grid_size);
    }

    if (x1 != NULL)
    {
        *x1 = x;
    }

    if (y1 != NULL)
    {
        *y1 = y;
    }
}


static void
bb_geda_editor_tool_changed_cb(BbToolChanger *changer, BbGedaEditor *window)
{
    bb_geda_editor_set_drawing_tool(
        window,
        bb_tool_changer_create_tool(
            changer,
            BB_TOOL_SUBJECT(window)
            )
        );
}


static void
bb_geda_editor_tool_subject_init(BbToolSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->add_item = bb_geda_editor_add_item;
    iface->invalidate_all = bb_geda_editor_invalidate_all;
    iface->invalidate_rect_dev = bb_geda_editor_invalidate_rect_dev;
    iface->snap_coordinate = bb_geda_editor_snap_coordinate;
    iface->user_to_widget_distance = bb_geda_editor_user_to_widget_distance;
    iface->widget_to_user = bb_geda_editor_widget_to_user;
    iface->zoom_box = bb_geda_editor_zoom_box;
}


static void
bb_geda_editor_user_to_widget_distance(BbToolSubject *subject, double ux, double uy, double *wx, double *wy)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(subject);
    g_return_if_fail(window != NULL);

    double x = ux;
    double y = uy;

    cairo_matrix_transform_distance(&window->matrix, &x, &y);

    if (wx != NULL)
    {
        *wx = x;
    }

    if (wy != NULL)
    {
        *wy = y;
    }
}


static gboolean
bb_geda_editor_widget_to_user(BbToolSubject *tool_subject, double wx, double wy, double *ux, double *uy)
{
    BbGedaEditor *window = BB_GEDA_EDITOR(tool_subject);
    g_return_val_if_fail(window != NULL, FALSE);

    cairo_matrix_t inverse = window->matrix;
    cairo_status_t status = cairo_matrix_invert(&inverse);
    g_return_val_if_fail(status == CAIRO_STATUS_SUCCESS, FALSE);

    double x = wx;
    double y = wy;

    cairo_matrix_transform_point(&inverse, &x, &y);

    if (ux != NULL)
    {
        *ux = x;
    }

    if (uy != NULL)
    {
        *uy = y;
    }

    return TRUE;
}