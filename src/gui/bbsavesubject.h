#ifndef __BBSAVESUBJECT__
#define __BBSAVESUBJECT__
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

#define BB_TYPE_SAVE_SUBJECT bb_save_subject_get_type()
G_DECLARE_INTERFACE(BbSaveSubject, bb_save_subject, BB, SAVE_SUBJECT, GObject)

struct _BbSaveSubjectInterface
{
    GTypeInterface g_iface;

    gboolean (*get_can_save)(BbSaveSubject *save_subject);
    gboolean (*get_can_save_as)(BbSaveSubject *save_subject);
    void (*save)(BbSaveSubject *save_subject, GError **error);
    void (*save_as)(BbSaveSubject *save_subject, GError **error);
};


/**
 * Indicates the underlying document can be saved
 *
 * @param save_subject A BbSaveSubject
 * @return TRUE if the bb_save_subject_save() functionality is available
 */
gboolean
bb_save_subject_get_can_save(BbSaveSubject *save_subject);


/**
 * Indicates the underlying document can be saved with an alternate filename
 *
 * @param save_subject A BbSaveSubject
 * @return TRUE if the bb_save_subject_save_as() functionality is available
 */
gboolean
bb_save_subject_get_can_save_as(BbSaveSubject *save_subject);


/**
 * Save the underlying document
 *
 * @param save_subject A BbSaveSubject
 */
void
bb_save_subject_save(BbSaveSubject *save_subject, GError **error);


/**
 * Save the underlying document with an alternate filename
 *
 * @param save_subject A BbSaveSubject
 */
void
bb_save_subject_save_as(BbSaveSubject *save_subject, GError **error);


#endif
