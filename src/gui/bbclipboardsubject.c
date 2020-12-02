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
#include "bbclipboardsubject.h"


static gboolean
bb_clipboard_subject_get_can_copy_missing(BbClipboardSubject *clipboard_subject);

static gboolean
bb_clipboard_subject_get_can_cut_missing(BbClipboardSubject *clipboard_subject);

static gboolean
bb_clipboard_subject_get_can_delete_missing(BbClipboardSubject *clipboard_subject);

static gboolean
bb_clipboard_subject_get_can_paste_missing(BbClipboardSubject *clipboard_subject);

static gboolean
bb_clipboard_subject_get_can_redo_missing(BbClipboardSubject *clipboard_subject);

static gboolean
bb_clipboard_subject_get_can_select_all_missing(BbClipboardSubject *clipboard_subject);

static gboolean
bb_clipboard_subject_get_can_select_none_missing(BbClipboardSubject *clipboard_subject);

static gboolean
bb_clipboard_subject_get_can_undo_missing(BbClipboardSubject *clipboard_subject);

static void
bb_clipboard_subject_copy_missing(BbClipboardSubject *clipboard_subject);

static void
bb_clipboard_subject_cut_missing(BbClipboardSubject *clipboard_subject);

static void
bb_clipboard_subject_delete_missing(BbClipboardSubject *clipboard_subject);

static void
bb_clipboard_subject_paste_missing(BbClipboardSubject *clipboard_subject);

static void
bb_clipboard_subject_redo_missing(BbClipboardSubject *clipboard_subject);

static void
bb_clipboard_subject_select_all_missing(BbClipboardSubject *clipboard_subject);

static void
bb_clipboard_subject_select_none_missing(BbClipboardSubject *clipboard_subject);

static void
bb_clipboard_subject_undo_missing(BbClipboardSubject *clipboard_subject);


G_DEFINE_INTERFACE(
    BbClipboardSubject,
    bb_clipboard_subject,
    G_TYPE_OBJECT
    )


static void
bb_clipboard_subject_default_init(BbClipboardSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_can_copy = bb_clipboard_subject_get_can_copy_missing;
    iface->get_can_cut = bb_clipboard_subject_get_can_cut_missing;
    iface->get_can_delete = bb_clipboard_subject_get_can_delete_missing;
    iface->get_can_paste = bb_clipboard_subject_get_can_paste_missing;
    iface->get_can_redo = bb_clipboard_subject_get_can_redo_missing;
    iface->get_can_select_all = bb_clipboard_subject_get_can_select_all_missing;
    iface->get_can_select_none = bb_clipboard_subject_get_can_select_none_missing;
    iface->get_can_undo = bb_clipboard_subject_get_can_undo_missing;
    iface->copy = bb_clipboard_subject_copy_missing;
    iface->cut = bb_clipboard_subject_cut_missing;
    iface->delete = bb_clipboard_subject_delete;
    iface->paste = bb_clipboard_subject_paste_missing;
    iface->redo = bb_clipboard_subject_redo_missing;
    iface->select_all = bb_clipboard_subject_select_all_missing;
    iface->select_none = bb_clipboard_subject_select_none_missing;
    iface->undo = bb_clipboard_subject_undo_missing;

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-copy",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-cut",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-delete",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-paste",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-redo",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-select-all",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-select-none",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-undo",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );
}


gboolean
bb_clipboard_subject_get_can_copy(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_copy != NULL, FALSE);

    return iface->get_can_copy(clipboard_subject);
}


static gboolean
bb_clipboard_subject_get_can_copy_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_get_can_copy() not overridden");
}

gboolean
bb_clipboard_subject_get_can_cut(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_cut != NULL, FALSE);

    return iface->get_can_cut(clipboard_subject);
}


static gboolean
bb_clipboard_subject_get_can_cut_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_get_can_cut() not overridden");
}


gboolean
bb_clipboard_subject_get_can_delete(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_delete != NULL, FALSE);

    return iface->get_can_delete(clipboard_subject);
}


static gboolean
bb_clipboard_subject_get_can_delete_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_get_can_delete() not overridden");
}


gboolean
bb_clipboard_subject_get_can_paste(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_paste != NULL, FALSE);

    return iface->get_can_paste(clipboard_subject);
}


static gboolean
bb_clipboard_subject_get_can_paste_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_get_can_paste() not overridden");
}


gboolean
bb_clipboard_subject_get_can_redo(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_redo != NULL, FALSE);

    return iface->get_can_redo(clipboard_subject);
}


static gboolean
bb_clipboard_subject_get_can_redo_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_get_can_redo() not overridden");
}


gboolean
bb_clipboard_subject_get_can_select_all(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_select_all != NULL, FALSE);

    return iface->get_can_select_all(clipboard_subject);
}


static gboolean
bb_clipboard_subject_get_can_select_all_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_get_can_select_all() not overridden");
}


gboolean
bb_clipboard_subject_get_can_select_none(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_select_none != NULL, FALSE);

    return iface->get_can_select_none(clipboard_subject);
}


static gboolean
bb_clipboard_subject_get_can_select_none_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_get_can_select_none() not overridden");
}

gboolean
bb_clipboard_subject_get_can_undo(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_undo != NULL, FALSE);

    return iface->get_can_undo(clipboard_subject);
}


static gboolean
bb_clipboard_subject_get_can_undo_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_get_can_undo() not overridden");
}


void
bb_clipboard_subject_copy(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->copy != NULL);

    iface->copy(clipboard_subject);
}


static void
bb_clipboard_subject_copy_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_copy() not overridden");
}


void
bb_clipboard_subject_cut(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->cut != NULL);

    iface->cut(clipboard_subject);
}


static void
bb_clipboard_subject_cut_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_cut() not overridden");
}


void
bb_clipboard_subject_delete(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->delete != NULL);

    iface->delete(clipboard_subject);
}


static void
bb_clipboard_subject_delete_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_delete() not overridden");
}


void
bb_clipboard_subject_paste(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->paste != NULL);

    iface->paste(clipboard_subject);
}


static void
bb_clipboard_subject_paste_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_paste() not overridden");
}


void
bb_clipboard_subject_redo(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->redo != NULL);

    iface->redo(clipboard_subject);
}


static void
bb_clipboard_subject_redo_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_redo() not overridden");
}


void
bb_clipboard_subject_select_all(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->select_all != NULL);

    iface->select_all(clipboard_subject);
}


static void
bb_clipboard_subject_select_all_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_select_all() not overridden");
}


void
bb_clipboard_subject_select_none(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->select_none != NULL);

    iface->select_none(clipboard_subject);
}


static void
bb_clipboard_subject_select_none_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_select_none() not overridden");
}


void
bb_clipboard_subject_undo(BbClipboardSubject *clipboard_subject)
{
    BbClipboardSubjectInterface *iface = BB_CLIPBOARD_SUBJECT_GET_IFACE(clipboard_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->undo != NULL);

    iface->undo(clipboard_subject);
}


static void
bb_clipboard_subject_undo_missing(BbClipboardSubject *clipboard_subject)
{
    g_error("bb_clipboard_subject_undo() not overridden");
}
