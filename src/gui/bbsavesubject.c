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
#include "bbsavesubject.h"


static gboolean
bb_save_subject_get_can_save_missing(BbSaveSubject *save_subject);

static gboolean
bb_save_subject_get_can_save_as_missing(BbSaveSubject *save_subject);

static void
bb_save_subject_save_missing(BbSaveSubject *save_subject, GError **error);

static void
bb_save_subject_save_as_missing(BbSaveSubject *save_subject, GError **error);


G_DEFINE_INTERFACE(
    BbSaveSubject,
    bb_save_subject,
    G_TYPE_OBJECT
    )


static void
bb_save_subject_default_init(BbSaveSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_can_save = bb_save_subject_get_can_save_missing;
    iface->get_can_save_as = bb_save_subject_get_can_save_as_missing;
    iface->save = bb_save_subject_save_missing;
    iface->save_as = bb_save_subject_save_as_missing;

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-save",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "can-save-as",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );
}


gboolean
bb_save_subject_get_can_save(BbSaveSubject *save_subject)
{
    g_return_val_if_fail(BB_IS_SAVE_SUBJECT(save_subject), FALSE);

    BbSaveSubjectInterface *iface = BB_SAVE_SUBJECT_GET_IFACE(save_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_save != NULL, FALSE);

    return iface->get_can_save(save_subject);
}


static gboolean
bb_save_subject_get_can_save_missing(BbSaveSubject *save_subject)
{
    g_error("bb_save_subject_get_can_save() not overridden");
}


gboolean
bb_save_subject_get_can_save_as(BbSaveSubject *save_subject)
{
    g_return_val_if_fail(BB_IS_SAVE_SUBJECT(save_subject), FALSE);

    BbSaveSubjectInterface *iface = BB_SAVE_SUBJECT_GET_IFACE(save_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_can_save_as != NULL, FALSE);

    return iface->get_can_save_as(save_subject);
}


static gboolean
bb_save_subject_get_can_save_as_missing(BbSaveSubject *save_subject)
{
    g_error("bb_save_subject_get_can_save_as() not overridden");
}


void
bb_save_subject_save(BbSaveSubject *save_subject, GError **error)
{
    g_return_if_fail(BB_IS_SAVE_SUBJECT(save_subject));

    BbSaveSubjectInterface *iface = BB_SAVE_SUBJECT_GET_IFACE(save_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->save != NULL);

    return iface->save(save_subject, error);
}


static void
bb_save_subject_save_missing(BbSaveSubject *save_subject, GError **error)
{
    g_error("bb_save_subject_as() not overridden");
}


void
bb_save_subject_save_as(BbSaveSubject *save_subject, GError **error)
{
    g_return_if_fail(BB_IS_SAVE_SUBJECT(save_subject));

    BbSaveSubjectInterface *iface = BB_SAVE_SUBJECT_GET_IFACE(save_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->save_as != NULL);

    return iface->save_as(save_subject, error);
}


static void
bb_save_subject_save_as_missing(BbSaveSubject *save_subject, GError **error)
{
    g_error("bb_save_subject_save_as() not overridden");
}
