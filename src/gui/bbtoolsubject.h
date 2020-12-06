#ifndef __BBTOOLSUBJECT__
#define __BBTOOLSUBJECT__
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

#define BB_TYPE_TOOL_SUBJECT bb_tool_subject_get_type()
G_DECLARE_INTERFACE(BbToolSubject, bb_tool_subject, BB, TOOL_SUBJECT, GObject)

struct _BbToolSubjectInterface
{
    GTypeInterface g_iface;

    void (*add_item)(BbToolSubject *subject, BbSchematicItem *item);
    void (*invalidate_rect_dev)(BbToolSubject *subject, double x0, double y0, double x1, double y1);
    void (*zoom_box)(BbToolSubject *subject, double x0, double y0, double x1, double y1);
};


void
bb_tool_subject_add_item(BbToolSubject *subject, BbSchematicItem *item);

void
bb_tool_subject_invalidate_rect_dev(BbToolSubject *subject, double x0, double y0, double x1, double y1);

/**
 *
 *
 * @param subject
 * @param x0 The x coordinate of the first corner in window coordinates
 * @param y0 The y coordinate of the first corner in window coordinates
 * @param x1 The x coordinate of the second corner in window coordinates
 * @param y1 The y coordinate of the second corner in window coordinates
 */
void
bb_tool_subject_zoom_box(BbToolSubject *subject, double x0, double y0, double x1, double y1);

#endif
