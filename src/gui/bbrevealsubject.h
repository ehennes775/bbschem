#ifndef __BBREVEALSUBJECT__
#define __BBREVEALSUBJECT__
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

#define BB_TYPE_REVEAL_SUBJECT bb_reveal_subject_get_type()
G_DECLARE_INTERFACE(BbRevealSubject, bb_reveal_subject, BB, REVEAL_SUBJECT, GObject)

struct _BbRevealSubjectInterface
{
    GTypeInterface g_iface;

    gboolean (*get_reveal)(BbRevealSubject *reveal_subject);
    void (*set_reveal)(BbRevealSubject *reveal_subject, gboolean reveal);
};

gboolean
bb_reveal_subject_get_reveal(BbRevealSubject *reveal_subject);

void
bb_reveal_subject_set_reveal(BbRevealSubject *reveal_subject, gboolean reveal);

#endif
