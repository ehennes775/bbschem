#ifndef __BBPROPERTYSUBJECT__
#define __BBPROPERTYSUBJECT__
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
#include "bbapplyfunc.h"
#include "bbqueryfunc.h"

#define BB_TYPE_PROPERTY_SUBJECT bb_property_subject_get_type()
G_DECLARE_INTERFACE(BbPropertySubject, bb_property_subject, BB, PROPERTY_SUBJECT, GObject)

struct _BbPropertySubjectInterface
{
    GTypeInterface g_iface;

    void (*apply_selection)(BbPropertySubject *subject, BbApplyFunc func, gpointer user_data);
    void (*query_selection)(BbPropertySubject *subject, BbQueryFunc func, gpointer user_data);
};


/**
 * Perform an operation to modify all items in the selection
 *
 * @param subject
 * @param func The operation to perform on each item in the selection
 * @param user_data Data to pass through to the operation as a parameter
 */
void
bb_property_subject_apply_selection(BbPropertySubject *subject, BbApplyFunc func, gpointer user_data);


/**
 * Perform an operation to query all items in the selection
 *
 * @param subject
 * @param func The operation to perform on each item in the selection
 * @param user_data Data to pass through to the operation as a parameter
 */
void
bb_property_subject_query_selection(BbPropertySubject *subject, BbQueryFunc func, gpointer user_data);


#endif
