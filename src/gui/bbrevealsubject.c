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
#include "bbrevealsubject.h"


static gboolean
bb_reveal_subject_get_reveal_missing(BbRevealSubject *reveal_subject);

void
bb_reveal_subject_set_reveal_missing(BbRevealSubject *reveal_subject, gboolean reveal);


G_DEFINE_INTERFACE(
    BbRevealSubject,
    bb_reveal_subject,
    G_TYPE_OBJECT
    )


static void
bb_reveal_subject_default_init(BbRevealSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_reveal = bb_reveal_subject_get_reveal_missing;
    iface->set_reveal = bb_reveal_subject_set_reveal_missing;

    g_object_interface_install_property(
        iface,
        g_param_spec_boolean(
            "reveal",
            "",
            "",
            FALSE,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );
}


gboolean
bb_reveal_subject_get_reveal(BbRevealSubject *reveal_subject)
{
    g_return_val_if_fail(BB_IS_REVEAL_SUBJECT(reveal_subject), FALSE);

    BbRevealSubjectInterface *iface = BB_REVEAL_SUBJECT_GET_IFACE(reveal_subject);

    g_return_val_if_fail(iface != NULL, FALSE);
    g_return_val_if_fail(iface->get_reveal != NULL, FALSE);

    return iface->get_reveal(reveal_subject);
}


static gboolean
bb_reveal_subject_get_reveal_missing(BbRevealSubject *reveal_subject)
{
    g_error("bb_reveal_subject_get_reveal() not overridden");
}


void
bb_reveal_subject_set_reveal(BbRevealSubject *reveal_subject, gboolean reveal)
{
    g_return_if_fail(BB_IS_REVEAL_SUBJECT(reveal_subject));

    BbRevealSubjectInterface *iface = BB_REVEAL_SUBJECT_GET_IFACE(reveal_subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->set_reveal != NULL);

    return iface->set_reveal(reveal_subject, reveal);
}


void
bb_reveal_subject_set_reveal_missing(BbRevealSubject *reveal_subject, gboolean reveal)
{
    g_error("bb_reveal_subject_set_reveal() not overridden");
}
