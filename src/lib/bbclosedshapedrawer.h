#ifndef __BBCLOSEDSHAPEDRAWER__
#define __BBCLOSEDSHAPEDRAWER__
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

typedef struct _BbItemRenderer BbItemRenderer;

#define BB_TYPE_CLOSED_SHAPE_DRAWER bb_closed_shape_drawer_get_type()
G_DECLARE_INTERFACE(BbClosedShapeDrawer, bb_closed_shape_drawer, BB, CLOSED_SHAPE_DRAWER, GObject)

struct _BbClosedShapeDrawerInterface
{
    GTypeInterface g_iface;

    void (*draw_hatch)(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer);
    void (*draw_outline)(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer);
};


/**
 *
 *
 * @param drawer
 * @param renderer
 */
void
bb_closed_shape_drawer_draw_hatch(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer);


/**
 *
 *
 * @param drawer
 * @param renderer
 */
void
bb_closed_shape_drawer_draw_outline(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer);


#endif
