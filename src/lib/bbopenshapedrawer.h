#ifndef __BBOPENSHAPEDRAWER__
#define __BBOPENSHAPEDRAWER__
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

#define BB_TYPE_OPEN_SHAPE_DRAWER bb_open_shape_drawer_get_type()
G_DECLARE_INTERFACE(BbOpenShapeDrawer, bb_open_shape_drawer, BB, OPEN_SHAPE_DRAWER, GObject)

struct _BbOpenShapeDrawerInterface
{
    GTypeInterface g_iface;

    void (*draw_shape)(BbOpenShapeDrawer *drawer, BbItemRenderer *renderer);
};


/**
 *
 *
 * @param drawer
 * @param renderer
 */
void
bb_open_shape_drawer_draw_shape(BbOpenShapeDrawer *drawer, BbItemRenderer *renderer);


#endif
