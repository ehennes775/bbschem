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
#include <bblibrary.h>

#define BB_TYPE_TOOL_SUBJECT bb_tool_subject_get_type()
G_DECLARE_INTERFACE(BbToolSubject, bb_tool_subject, BB, TOOL_SUBJECT, GObject)

struct _BbToolSubjectInterface
{
    GTypeInterface g_iface;

    void (*add_item)(BbToolSubject *subject, BbSchematicItem *item);
    void (*invalidate_all)(BbToolSubject *subject);
    void (*invalidate_rect_dev)(BbToolSubject *subject, double x0, double y0, double x1, double y1);
    void (*snap_coordinate)(BbToolSubject *subject, int x0, int y0, int *x1, int *y1);
    gboolean (*widget_to_user)(BbToolSubject *subject, double wx, double wy, double *ux, double *uy);
    void (*zoom_box)(BbToolSubject *subject, double x0, double y0, double x1, double y1);
};


void
bb_tool_subject_add_item(BbToolSubject *subject, BbSchematicItem *item);


void
bb_tool_subject_invalidate_all(BbToolSubject *subject);


/**
 * Invalidate a rectangle
 *
 * @param subject A BbToolSubject
 * @param x0 The x coordinate of the first corner in window coordinates
 * @param y0 The y coordinate of the first corner in window coordinates
 * @param x1 The x coordinate of the second corner in window coordinates
 * @param y1 The y coordinate of the second corner in window coordinates
 */
void
bb_tool_subject_invalidate_rect_dev(BbToolSubject *subject, double x0, double y0, double x1, double y1);


/**
 * Snap a coordinate to the rectangular grid
 *
 * The input and output points can be the same variable.
 *
 * @param subject A BbToolSubject
 * @param x0 The x coordinate of the input point
 * @param y0 The y coordinate of the input point
 * @param x1 The x coordinate of the output point
 * @param y1 The y coordinate of the output point
 */
void
bb_tool_subject_snap_coordinate(BbToolSubject *subject, int x0, int y0, int *x1, int *y1);


/**
 * Convert user (i.e. schematic) coordinates to widget coordinates
 *
 * @param subject A BbToolSubject
 * @param wx The x coordinate of the input in widget coordinates
 * @param wy The y coordinate of the input in widget coordinates
 * @param ux The x coordinate of the output in user coordinates
 * @param uy The y coordinate of the output in user coordinates
 * @return Returns TRUE if successful
 */
gboolean
bb_tool_subject_widget_to_user(BbToolSubject *subject, double wx, double wy, double *ux, double *uy);


/**
 * Zoom to a box
 *
 * @param subject A BbToolSubject
 * @param x0 The x coordinate of the first corner in window coordinates
 * @param y0 The y coordinate of the first corner in window coordinates
 * @param x1 The x coordinate of the second corner in window coordinates
 * @param y1 The y coordinate of the second corner in window coordinates
 */
void
bb_tool_subject_zoom_box(BbToolSubject *subject, double x0, double y0, double x1, double y1);

#endif
