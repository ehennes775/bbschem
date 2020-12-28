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
#include "bbitemrenderer.h"
#include "bbopenshapedrawer.h"


static void
bb_open_shape_drawer_draw_shape_missing(BbOpenShapeDrawer *drawer, BbItemRenderer *renderer);


G_DEFINE_INTERFACE(BbOpenShapeDrawer, bb_open_shape_drawer, G_TYPE_OBJECT)


static void
bb_open_shape_drawer_default_init(BbOpenShapeDrawerInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->draw_shape = bb_open_shape_drawer_draw_shape_missing;
}




void
bb_open_shape_drawer_draw_shape(BbOpenShapeDrawer *drawer, BbItemRenderer *renderer)
{
    g_return_if_fail(BB_IS_OPEN_SHAPE_DRAWER(drawer));
    g_return_if_fail(BB_IS_ITEM_RENDERER(renderer));

    BbOpenShapeDrawerInterface *iface = BB_OPEN_SHAPE_DRAWER_GET_IFACE(drawer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->draw_shape != NULL);

    iface->draw_shape(drawer, renderer);
}


static void
bb_open_shape_drawer_draw_shape_missing(BbOpenShapeDrawer *drawer, BbItemRenderer *renderer)
{
    g_error("bb_open_shape_draw_shape() not overridden");
}
