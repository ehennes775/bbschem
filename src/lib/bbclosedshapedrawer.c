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
#include "bbclosedshapedrawer.h"


static void
bb_closed_shape_drawer_draw_hatch_missing(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer);

static void
bb_closed_shape_drawer_draw_outline_missing(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer);


G_DEFINE_INTERFACE(BbClosedShapeDrawer, bb_closed_shape_drawer, G_TYPE_OBJECT)


static void
bb_closed_shape_drawer_default_init(BbClosedShapeDrawerInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->draw_hatch = bb_closed_shape_drawer_draw_hatch_missing;
    iface->draw_outline = bb_closed_shape_drawer_draw_outline_missing;
}


void
bb_closed_shape_drawer_draw_hatch(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer)
{
    g_return_if_fail(BB_IS_CLOSED_SHAPE_DRAWER(drawer));
    g_return_if_fail(BB_IS_ITEM_RENDERER(renderer));

    BbClosedShapeDrawerInterface *iface = BB_CLOSED_SHAPE_DRAWER_GET_IFACE(drawer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->draw_hatch != NULL);

    iface->draw_hatch(drawer, renderer);
}


static void
bb_closed_shape_drawer_draw_hatch_missing(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer)
{
    g_error("bb_closed_shape_draw_hatch() not overridden");
}


void
bb_closed_shape_drawer_draw_outline(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer)
{
    g_return_if_fail(BB_IS_CLOSED_SHAPE_DRAWER(drawer));
    g_return_if_fail(BB_IS_ITEM_RENDERER(renderer));

    BbClosedShapeDrawerInterface *iface = BB_CLOSED_SHAPE_DRAWER_GET_IFACE(drawer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->draw_outline != NULL);

    iface->draw_outline(drawer, renderer);
}


static void
bb_closed_shape_drawer_draw_outline_missing(BbClosedShapeDrawer *drawer, BbItemRenderer *renderer)
{
    g_error("bb_closed_shape_draw_outline() not overridden");
}
