/*
 * bbschem
 * Copyright (C) 2021 Edward C. Hennessy
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
#include "bbpropertysubject.h"


G_DEFINE_INTERFACE(BbPropertySubject, bb_property_subject, G_TYPE_OBJECT)


static void
bb_property_subject_apply_selection_missing(BbPropertySubject *subject, BbApplyFunc func, gpointer user_data);

static void
bb_property_subject_query_selection_missing(BbPropertySubject *subject, BbQueryFunc func, gpointer user_data);


static void
bb_property_subject_default_init(BbPropertySubjectInterface *class)
{
    g_return_if_fail(class != NULL);

    class->apply_selection = bb_property_subject_apply_selection_missing;
    class->query_selection = bb_property_subject_query_selection_missing;
}


void
bb_property_subject_apply_selection(BbPropertySubject *subject, BbApplyFunc func, gpointer user_data)
{
    g_return_if_fail(BB_IS_PROPERTY_SUBJECT(subject));

    BbPropertySubjectInterface *iface = BB_PROPERTY_SUBJECT_GET_IFACE(subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->apply_selection != NULL);

    iface->apply_selection(subject, func, user_data);
}


static void
bb_property_subject_apply_selection_missing(BbPropertySubject *subject, BbApplyFunc func, gpointer user_data)
{
    g_error("bb_property_subject_apply_selection() not overridden");
}


void
bb_property_subject_query_selection(BbPropertySubject *subject, BbQueryFunc func, gpointer user_data)
{
    g_return_if_fail(BB_IS_PROPERTY_SUBJECT(subject));

    BbPropertySubjectInterface *iface = BB_PROPERTY_SUBJECT_GET_IFACE(subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->query_selection != NULL);

    iface->query_selection(subject, func, user_data);
}


static void
bb_property_subject_query_selection_missing(BbPropertySubject *subject, BbQueryFunc func, gpointer user_data)
{
    g_error("bb_property_subject_query_selection() not overridden");
}
