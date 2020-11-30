#ifndef __BBCLIPBOARDSUBJECT__
#define __BBCLIPBOARDSUBJECT__
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

#define BB_TYPE_CLIPBOARD_SUBJECT bb_clipboard_subject_get_type()
G_DECLARE_INTERFACE(BbClipboardSubject, bb_clipboard_subject, BB, CLIPBOARD_SUBJECT, GObject)

struct _BbClipboardSubjectInterface
{
    GTypeInterface g_iface;

    gboolean (*get_can_copy)(BbClipboardSubject *clipboard_subject);
    gboolean (*get_can_cut)(BbClipboardSubject *clipboard_subject);
    gboolean (*get_can_paste)(BbClipboardSubject *clipboard_subject);
    void (*copy)(BbClipboardSubject *clipboard_subject);
    void (*cut)(BbClipboardSubject *clipboard_subject);
    void (*paste)(BbClipboardSubject *clipboard_subject);
};


gboolean
bb_clipboard_subject_get_can_copy(BbClipboardSubject *clipboard_subject);

gboolean
bb_clipboard_subject_get_can_cut(BbClipboardSubject *clipboard_subject);

gboolean
bb_clipboard_subject_get_can_paste(BbClipboardSubject *clipboard_subject);

void
bb_clipboard_subject_copy(BbClipboardSubject *clipboard_subject);

void
bb_clipboard_subject_cut(BbClipboardSubject *clipboard_subject);

void
bb_clipboard_subject_paste(BbClipboardSubject *clipboard_subject);

#endif
