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

enum
{
    PROP_0,

    /* From BbCopyReceiver */
    PROP_CAN_COPY,

    /* From BbCutReceiver */
    PROP_CAN_CUT,

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

    GtkSourceView *view;
};


// region Function Prototypes

static void
bb_text_editor_copy_receiver_init(BbCopyReceiverInterface *iface);

static void
bb_text_editor_cut_receiver_init(BbCutReceiverInterface *iface);

static void
bb_text_editor_dispose(GObject *object);

static void
bb_text_editor_finalize(GObject *object);

static void
bb_text_editor_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_text_editor_notify_has_selection(GObject *unused, GParamSpec *pspec, GObject *editor);

static void
bb_text_editor_paste_receiver_init(BbPasteReceiverInterface *iface);

static void
bb_text_editor_redo_receiver_init(BbRedoReceiverInterface *iface);

static void
bb_text_editor_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

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
    G_IMPLEMENT_INTERFACE(BB_TYPE_PASTE_RECEIVER, bb_text_editor_paste_receiver_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_REDO_RECEIVER, bb_text_editor_redo_receiver_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_UNDO_RECEIVER, bb_text_editor_undo_receiver_init)


    //G_IMPLEMENT_INTERFACE(BB_TYPE_DELETE_RECEIVER, bb_text_editor_delete_receiver_init)
    //G_IMPLEMENT_INTERFACE(BB_TYPE_REVEAL_RECEIVER, bb_text_editor_reveal_receiver_init)
    //G_IMPLEMENT_INTERFACE(BB_TYPE_SAVE_RECEIVER, bb_text_editor_save_receiver_init)
    //G_IMPLEMENT_INTERFACE(BB_TYPE_SELECT_RECEIVER, bb_text_editor_select_receiver_init)

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

// region from BbPasteReceiver

static gboolean
bb_text_editor_paste_receiver_can_paste(BbPasteReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_val_if_fail(editor != NULL, FALSE);
    g_return_val_if_fail(editor->view != NULL, FALSE);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_val_if_fail(buffer != NULL, FALSE);

    return gtk_text_buffer_get_has_selection(buffer);
}

static void
bb_text_editor_paste_receiver_paste(BbPasteReceiver *receiver)
{
    BbTextEditor *editor = BB_TEXT_EDITOR(receiver);

    g_return_if_fail(editor != NULL);
    g_return_if_fail(editor->view != NULL);

    GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(editor->view));

    g_return_if_fail(buffer != NULL);

    // gtk_text_buffer_paste_clipboard(buffer, NULL, TRUE);
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
        "notify::has-selection",
        G_CALLBACK(bb_text_editor_notify_has_selection),
        editor
        );
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
bb_text_editor_notify_has_selection(GObject *unused, GParamSpec *pspec, GObject *editor)
{
    g_object_notify_by_pspec(editor, properties[PROP_CAN_COPY]);
    g_object_notify_by_pspec(editor, properties[PROP_CAN_CUT]);
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
