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
#include <gtksourceview-3.0/gtksourceview/gtksource.h>
#include "bbtexteditor.h"
#include "actions/bbpastereceiver.h"
#include "actions/bbundoreceiver.h"
#include "actions/bbredoreceiver.h"
#include "actions/bbcopyreceiver.h"
#include "actions/bbcutreceiver.h"
#include "actions/bbdeletereceiver.h"
#include "actions/bbsavereceiver.h"
#include "actions/bbselectreceiver.h"

enum
{
    PROP_0,

    /* From BbCopyReceiver */
    PROP_CAN_COPY,

    /* From BbCutReceiver */
    PROP_CAN_CUT,

    /* From BbDeleteReceiver */
    PROP_CAN_DELETE,

    /* From BbRedoReceiver */
    PROP_CAN_REDO,

    /* From BbSaveReceiver */
    PROP_CAN_SAVE,

    /* From BbSelectReceiver */
    PROP_CAN_SELECT_ALL,
    PROP_CAN_SELECT_NONE,

    /* From BbUndoReceiver */
    PROP_CAN_UNDO,

    /* From BbDocumentWindow */
    PROP_TAB,

    N_PROPERTIES
};


enum
{
    N_SIGNALS
};


struct _BbTextEditor
{
    BbDocumentWindow parent;

    GFile *file;
    GtkSourceView *view;
};


// region Function Prototypes

static void
bb_text_editor_copy_receiver_init(BbCopyReceiverInterface *iface);

static void
bb_text_editor_cut_receiver_init(BbCutReceiverInterface *iface);

static void
bb_text_editor_delete_receiver_init(BbDeleteReceiverInterface *iface);

static void
bb_text_editor_dispose(GObject *object);

static void
bb_text_editor_finalize(GObject *object);

static void
bb_text_editor_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_text_editor_modified_changed(GObject *unused, GObject *editor);

static void
bb_text_editor_notify_can_redo(GObject *unused, GParamSpec *pspec, GObject *editor);

static void
bb_text_editor_notify_can_undo(GObject *unused, GParamSpec *pspec, GObject *editor);

static void
bb_text_editor_notify_has_selection(GObject *unused, GParamSpec *pspec, GObject *editor);

static void
bb_text_editor_paste_receiver_init(BbPasteReceiverInterface *iface);

static void
bb_text_editor_redo_receiver_init(BbRedoReceiverInterface *iface);

static void
bb_text_editor_save(BbTextEditor *editor, GError **error);

static void
bb_text_editor_save_receiver_init(BbSaveReceiverInterface *iface);

static void
bb_text_editor_select_receiver_init(BbSelectReceiverInterface *iface);

static void
bb_text_editor_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_text_editor_source_mark_updated(GtkSourceBuffer *buffer, GtkTextMark *mark, GObject *editor);

static void
bb_text_editor_undo_receiver_init(BbUndoReceiverInterface *iface);

// endregion

static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_EXTENDED(
    BbTextEditor,
    bb_text_editor,
    BB_TYPE_DOCUMENT_WINDOW,
    0,
    G_IMPLEMENT_INTERFACE(BB_TYPE_COPY_RECEIVER, bb_text_editor_copy_receiver_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_CUT_RECEIVER, bb_text_editor_cut_receiver_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_DELETE_RECEIVER, bb_text_editor_delete_receiver_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_PASTE_RECEIVER, bb_text_editor_paste_receiver_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_REDO_RECEIVER, bb_text_editor_redo_receiver_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_SAVE_RECEIVER, bb_text_editor_save_receiver_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_SELECT_RECEIVER, bb_text_editor_select_receiver_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_UNDO_RECEIVER, bb_text_editor_undo_receiver_init)


    //G_IMPLEMENT_INTERFACE(BB_TYPE_REVEAL_RECEIVER, bb_text_editor_reveal_receiver_init)

    //G_IMPLEMENT_INTERFACE(BB_TYPE_DRAWING_TOOL_SUPPORT, bb_text_editor_drawing_tool_support_init)
    //G_IMPLEMENT_INTERFACE(BB_TYPE_GRID_SUBJECT, bb_text_editor_grid_subject_init)
    //G_IMPLEMENT_INTERFACE(BB_TYPE_PROPERTY_SUBJECT, bb_text_editor_property_subject_init)
    //G_IMPLEMENT_INTERFACE(BB_TYPE_TOOL_SUBJECT, bb_text_editor_tool_subject_init)
    //G_IMPLEMENT_INTERFACE(BB_TYPE_ZOOM_RECEIVER, bb_text_editor_zoom_receiver_init)
    )

// region from BbCopyReceiver

static gboolean
bb_text_editor_copy_receiver_can_copy(BbCopyReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_val_if_fail(editor != NULL, FALSE);
    g_return_val_if_fail(editor->view != NULL, FALSE);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_val_if_fail(buffer != NULL, FALSE);

    return gtk_text_buffer_get_has_selection(buffer);
}

static void
bb_text_editor_copy_receiver_copy(BbCopyReceiver *receiver, GtkClipboard *clipboard)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_if_fail(editor != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_if_fail(buffer != NULL);

    gtk_text_buffer_copy_clipboard(buffer, clipboard);
}

static void
bb_text_editor_copy_receiver_init(BbCopyReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->can_copy = bb_text_editor_copy_receiver_can_copy;
    iface->copy = bb_text_editor_copy_receiver_copy;
}

// endregion

// region from BbCutReceiver

static gboolean
bb_text_editor_cut_receiver_can_cut(BbCutReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_val_if_fail(editor != NULL, FALSE);
    g_return_val_if_fail(editor->view != NULL, FALSE);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_val_if_fail(buffer != NULL, FALSE);

    return gtk_text_buffer_get_has_selection(buffer);
}

static void
bb_text_editor_cut_receiver_cut(BbCutReceiver *receiver, GtkClipboard *clipboard)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_if_fail(editor != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_if_fail(buffer != NULL);

    gtk_text_buffer_cut_clipboard(buffer, clipboard, TRUE);
}

static void
bb_text_editor_cut_receiver_init(BbCutReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->can_cut = bb_text_editor_cut_receiver_can_cut;
    iface->cut = bb_text_editor_cut_receiver_cut;
}

// endregion

// region from BbDeleteReceiver

static gboolean
bb_text_editor_delete_receiver_can_delete(BbDeleteReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_val_if_fail(editor != NULL, FALSE);
    g_return_val_if_fail(editor->view != NULL, FALSE);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_val_if_fail(buffer != NULL, FALSE);

    return gtk_text_buffer_get_has_selection(buffer);
}

static void
bb_text_editor_delete_receiver_delete(BbDeleteReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_if_fail(editor != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_if_fail(buffer != NULL);

    gtk_text_buffer_delete_selection(buffer, FALSE, TRUE);
}

static void
bb_text_editor_delete_receiver_init(BbDeleteReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->can_delete = bb_text_editor_delete_receiver_can_delete;
    iface->delete = bb_text_editor_delete_receiver_delete;
}

// endregion

// region from BbPasteReceiver

static gboolean
bb_text_editor_paste_receiver_can_paste(BbPasteReceiver *receiver, GtkSelectionData *selection_data)
{
    return gtk_selection_data_targets_include_text(selection_data);
}

static void
bb_text_editor_paste_receiver_paste(BbPasteReceiver *receiver, GtkClipboard *clipboard)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_if_fail(editor != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_if_fail(buffer != NULL);

    gtk_text_buffer_paste_clipboard(buffer, clipboard, NULL, TRUE);
}

static void
bb_text_editor_paste_receiver_init(BbPasteReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->can_paste = bb_text_editor_paste_receiver_can_paste;
    iface->paste = bb_text_editor_paste_receiver_paste;
}

// endregion

// region from BbRedoReceiver

static gboolean
bb_text_editor_redo_receiver_can_redo(BbRedoReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_val_if_fail(editor != NULL, FALSE);
    g_return_val_if_fail(editor->view != NULL, FALSE);

    GtkSourceBuffer *buffer = GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view)));
    
    g_return_val_if_fail(buffer != NULL, FALSE);

    return gtk_source_buffer_can_redo(buffer);
}

static void
bb_text_editor_redo_receiver_redo(BbRedoReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_if_fail(editor != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkSourceBuffer *buffer = GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view)));

    g_return_if_fail(buffer != NULL);

    gtk_source_buffer_redo(buffer);
}

static void
bb_text_editor_redo_receiver_init(BbRedoReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->can_redo = bb_text_editor_redo_receiver_can_redo;
    iface->redo = bb_text_editor_redo_receiver_redo;
}

// endregion

// region from BbSaveReceiver

static gboolean
bb_text_editor_save_receiver_can_save(BbSaveReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_val_if_fail(editor != NULL, FALSE);
    g_return_val_if_fail(editor->view != NULL, FALSE);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_val_if_fail(buffer != NULL, FALSE);

    return gtk_text_buffer_get_modified(buffer);
}

static void
bb_text_editor_save_receiver_save(BbSaveReceiver *receiver, GError **error)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    bb_text_editor_save(editor, error);
}

static void
bb_text_editor_save_receiver_init(BbSaveReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_can_save = bb_text_editor_save_receiver_can_save;
    iface->save = bb_text_editor_save_receiver_save;
}

// endregion

// region from BbSelectReceiver

static gboolean
bb_text_editor_select_receiver_can_select_all(BbSelectReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_val_if_fail(editor != NULL, FALSE);
    g_return_val_if_fail(editor->view != NULL, FALSE);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_val_if_fail(buffer != NULL, FALSE);

    GtkTextIter iter0;
    GtkTextIter iter1;

    GtkTextIter iter2;
    GtkTextIter iter3;

    gtk_text_buffer_get_bounds(buffer, &iter0, &iter1);

    gtk_text_buffer_get_selection_bounds(buffer, &iter2, &iter3);

    return !gtk_text_iter_compare(&iter0, &iter2) || !gtk_text_iter_compare(&iter1, &iter3);
}

static gboolean
bb_text_editor_select_receiver_can_select_none(BbSelectReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_val_if_fail(editor != NULL, FALSE);
    g_return_val_if_fail(editor->view != NULL, FALSE);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_val_if_fail(buffer != NULL, FALSE);

    return gtk_text_buffer_get_has_selection(buffer);
}

static void
bb_text_editor_select_receiver_select_all(BbSelectReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_if_fail(editor != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_if_fail(buffer != NULL);

    GtkTextIter iter0;
    GtkTextIter iter1;

    gtk_text_buffer_get_bounds(buffer, &iter0, &iter1);
    gtk_text_buffer_select_range(buffer, &iter0, &iter1);
}

static void
bb_text_editor_select_receiver_select_none(BbSelectReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_if_fail(editor != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_if_fail(buffer != NULL);

    GtkTextIter iter;

    gtk_text_buffer_get_iter_at_mark(buffer, &iter, gtk_text_buffer_get_insert(buffer));
    gtk_text_buffer_move_mark_by_name(buffer, "selection_bound", &iter);
}

static void
bb_text_editor_select_receiver_init(BbSelectReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->can_select_all = bb_text_editor_select_receiver_can_select_all;
    iface->can_select_none = bb_text_editor_select_receiver_can_select_none;
    iface->select_all = bb_text_editor_select_receiver_select_all;
    iface->select_none = bb_text_editor_select_receiver_select_none;
}

// endregion

// region from BbUndoReceiver

static gboolean
bb_text_editor_undo_receiver_can_undo(BbUndoReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_val_if_fail(editor != NULL, FALSE);
    g_return_val_if_fail(editor->view != NULL, FALSE);

    GtkSourceBuffer *buffer = GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view)));

    g_return_val_if_fail(buffer != NULL, FALSE);

    return gtk_source_buffer_can_undo(buffer);
}

static void
bb_text_editor_undo_receiver_undo(BbUndoReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_if_fail(editor != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkSourceBuffer *buffer = GTK_SOURCE_BUFFER(gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view)));

    g_return_if_fail(buffer != NULL);

    gtk_source_buffer_undo(buffer);
}
        
static void
bb_text_editor_undo_receiver_init(BbUndoReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->can_undo = bb_text_editor_undo_receiver_can_undo;
    iface->undo = bb_text_editor_undo_receiver_undo;
}

// endregion
    
    
static void
bb_text_editor_class_init(BbTextEditorClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);

    object_class->dispose = bb_text_editor_dispose;
    object_class->finalize = bb_text_editor_finalize;
    object_class->get_property = bb_text_editor_get_property;
    object_class->set_property = bb_text_editor_set_property;

    BbDocumentWindowClass *document_window_class = BB_DOCUMENT_WINDOW_CLASS(klasse);

    //document_window_class->get_tab = bb_text_editor_get_tab;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/textedit/bbtexteditor.ui"
        );

    gtk_widget_class_bind_template_child(
        GTK_WIDGET_CLASS(klasse),
        BbTextEditor,
        view
        );

    /* From BbCutReceiver */

    g_object_class_override_property(
        object_class,
        PROP_CAN_COPY,
        "can-copy"
        );

    properties[PROP_CAN_COPY] = g_object_class_find_property(
        object_class,
        "can-copy"
        );

    /* From BbCutReceiver */

    g_object_class_override_property(
        object_class,
        PROP_CAN_CUT,
        "can-cut"
        );

    properties[PROP_CAN_CUT] = g_object_class_find_property(
        object_class,
        "can-cut"
        );

    /* From BbDeleteReceiver */

    g_object_class_override_property(
        object_class,
        PROP_CAN_DELETE,
        "can-delete"
        );

    properties[PROP_CAN_DELETE] = g_object_class_find_property(
        object_class,
        "can-delete"
        );

    /* From BbRedoReceiver */

    g_object_class_override_property(
        object_class,
        PROP_CAN_REDO,
        "can-redo"
        );

    properties[PROP_CAN_REDO] = g_object_class_find_property(
        object_class,
        "can-redo"
        );

    /* From BbSaveReceiver */

    g_object_class_override_property(
        object_class,
        PROP_CAN_SAVE,
        "can-save"
        );

    properties[PROP_CAN_SAVE] = g_object_class_find_property(
        object_class,
        "can-save"
        );

    /* From BbSaveReceiver */

    g_object_class_override_property(
        object_class,
        PROP_CAN_SELECT_ALL,
        "can-select-all"
        );

    properties[PROP_CAN_SELECT_ALL] = g_object_class_find_property(
        object_class,
        "can-select-all"
        );

    g_object_class_override_property(
        object_class,
        PROP_CAN_SELECT_NONE,
        "can-select-none"
        );

    properties[PROP_CAN_SELECT_NONE] = g_object_class_find_property(
        object_class,
        "can-select-none"
        );

    /* From BbUndoReceiver */

    g_object_class_override_property(
        object_class,
        PROP_CAN_UNDO,
        "can-undo"
        );

    properties[PROP_CAN_UNDO] = g_object_class_find_property(
        object_class,
        "can-undo"
        );

    /* From BbDocumentWindow */

    properties[PROP_TAB] = g_object_class_find_property(
        object_class,
        "tab"
        );
}


static void
bb_text_editor_class_finalize(BbTextEditorClass *klasse)
{
}


static void
bb_text_editor_dispose(GObject *object)
{
}


static void
bb_text_editor_finalize(GObject *object)
{
}


static void
bb_text_editor_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    BbTextEditor *window = BB_TEXT_EDITOR(object);
    g_return_if_fail(window != NULL);

    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_text_editor_init(BbTextEditor *editor)
{
    gtk_widget_init_template(GTK_WIDGET(editor));

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_signal_connect(
        buffer,
        "modified-changed",
        G_CALLBACK(bb_text_editor_modified_changed),
        editor
        );

    g_signal_connect(
        buffer,
        "notify::can-redo",
        G_CALLBACK(bb_text_editor_notify_can_redo),
        editor
        );

    g_signal_connect(
        buffer,
        "notify::can-undo",
        G_CALLBACK(bb_text_editor_notify_can_undo),
        editor
        );

    g_signal_connect(
        buffer,
        "notify::has-selection",
        G_CALLBACK(bb_text_editor_notify_has_selection),
        editor
        );

    g_signal_connect(
        buffer,
        "source-mark-updated",
        G_CALLBACK(bb_text_editor_source_mark_updated),
        editor
        );
}


static void
bb_text_editor_modified_changed(GObject *unused, GObject *editor)
{
    g_object_notify_by_pspec(editor, properties[PROP_CAN_SAVE]);
}


BbTextEditor*
bb_text_editor_new()
{
    return BB_TEXT_EDITOR(g_object_new(
        BB_TYPE_TEXT_EDITOR,
        NULL
        ));
}


static void
bb_text_editor_notify_can_redo(GObject *unused, GParamSpec *pspec, GObject *editor)
{
    g_object_notify_by_pspec(editor, properties[PROP_CAN_REDO]);
}


static void
bb_text_editor_notify_can_undo(GObject *unused, GParamSpec *pspec, GObject *editor)
{
    g_object_notify_by_pspec(editor, properties[PROP_CAN_UNDO]);
}


static void
bb_text_editor_notify_has_selection(GObject *unused, GParamSpec *pspec, GObject *editor)
{
    g_object_notify_by_pspec(editor, properties[PROP_CAN_COPY]);
    g_object_notify_by_pspec(editor, properties[PROP_CAN_CUT]);
    g_object_notify_by_pspec(editor, properties[PROP_CAN_DELETE]);
    g_object_notify_by_pspec(editor, properties[PROP_CAN_SELECT_NONE]);
}


static void
bb_text_editor_save(BbTextEditor *editor, GError **error)
{
    g_return_if_fail(BB_IS_TEXT_EDITOR(editor));
    g_return_if_fail(editor->file != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    GtkTextIter iter0;
    GtkTextIter iter1;

    gtk_text_buffer_get_start_iter(buffer, &iter0);
    gtk_text_buffer_get_end_iter(buffer, &iter1);

    gchar *text = gtk_text_buffer_get_text(buffer, &iter0, &iter1, FALSE);

    //g_file_set

    g_free(text);
}


static void
bb_text_editor_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    g_return_if_fail(BB_IS_TEXT_EDITOR(object));

    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}

static void
bb_text_editor_source_mark_updated(GtkSourceBuffer *buffer, GtkTextMark *mark, GObject *editor)
{
    g_message("Hello from bb_text_editor_source_mark_updated()");

    g_object_notify_by_pspec(editor, properties[PROP_CAN_SELECT_ALL]);
}