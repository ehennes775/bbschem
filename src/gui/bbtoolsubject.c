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
#include <bblibrary.h>
#include "bbtoolsubject.h"


static void
bb_tool_subject_add_item_missing(BbToolSubject *subject, BbSchematicItem *item);

static void
bb_tool_subject_invalidate_rect_dev_missing(BbToolSubject *subject, double x0, double y0, double x1, double y1);


G_DEFINE_INTERFACE(BbToolSubject, bb_tool_subject, G_TYPE_OBJECT)


static void
bb_tool_subject_default_init(BbToolSubjectInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->add_item = bb_tool_subject_add_item_missing;
    iface->invalidate_rect_dev = bb_tool_subject_invalidate_rect_dev_missing;
}


void
bb_tool_subject_add_item(BbToolSubject *subject, BbSchematicItem *item)
{
    g_return_if_fail(subject != NULL);

    BbToolSubjectInterface *iface = BB_TOOL_SUBJECT_GET_IFACE(subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->add_item != NULL);

    return iface->add_item(subject, item);
}


static void
bb_tool_subject_add_item_missing(BbToolSubject *subject, BbSchematicItem *item)
{
    g_error("bb_tool_subject_add_item_missing() not overridden");
}


void
bb_tool_subject_invalidate_rect_dev(BbToolSubject *subject, double x0, double y0, double x1, double y1)
{
    g_return_if_fail(subject != NULL);

    BbToolSubjectInterface *iface = BB_TOOL_SUBJECT_GET_IFACE(subject);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->invalidate_rect_dev != NULL);

    return iface->invalidate_rect_dev(subject, x0, y0, x1, y1);
}


static void
bb_tool_subject_invalidate_rect_dev_missing(BbToolSubject *subject, double x0, double y0, double x1, double y1)
{
    g_error("bb_tool_subject_invalidate_rect_dev() not overridden");
}
