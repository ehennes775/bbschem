/*
 * bbsch
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


static void
bb_item_renderer_default_init(BbItemRendererInterface *class);

static void
bb_item_renderer_render_absolute_line_to_missing(BbItemRenderer *renderer, int x, int y);

static void
bb_item_renderer_render_absolute_move_to_missing(BbItemRenderer *renderer, int x, int y);

static void
bb_item_renderer_render_graphic_arc_missing(BbItemRenderer *renderer, struct _BbGraphicArc *arc);

static void
bb_item_renderer_render_graphic_box_missing(BbItemRenderer *renderer, struct _BbGraphicBox *box);

static void
bb_item_renderer_render_graphic_circle_missing(BbItemRenderer *renderer, struct _BbGraphicCircle *circle);

static void
bb_item_renderer_render_graphic_line_missing(BbItemRenderer *renderer, struct _BbGraphicLine *line);

static void
bb_item_renderer_render_graphic_path_missing(BbItemRenderer *renderer, struct _BbGraphicPath *path);


G_DEFINE_INTERFACE(BbItemRenderer, bb_item_renderer, G_TYPE_OBJECT)


static void
bb_item_renderer_default_init(BbItemRendererInterface *class)
{
    g_return_if_fail(class != NULL);

    class->render_absolute_line_to = bb_item_renderer_render_absolute_line_to_missing;
    class->render_absolute_move_to = bb_item_renderer_render_absolute_move_to_missing;
    class->render_graphic_arc = bb_item_renderer_render_graphic_arc_missing;
    class->render_graphic_box = bb_item_renderer_render_graphic_box_missing;
    class->render_graphic_circle = bb_item_renderer_render_graphic_circle_missing;
    class->render_graphic_line = bb_item_renderer_render_graphic_line_missing;
    class->render_graphic_path = bb_item_renderer_render_graphic_path_missing;
}


void
bb_item_renderer_render_absolute_line_to(BbItemRenderer *renderer, int x, int y)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_absolute_line_to != NULL);

    return iface->render_absolute_line_to(renderer, x, y);
}


static void
bb_item_renderer_render_absolute_line_to_missing(BbItemRenderer *renderer, int x, int y)
{
    g_error("bb_item_renderer_render_absolute_line_to() not overridden");
}


void
bb_item_renderer_render_absolute_move_to(BbItemRenderer *renderer, int x, int y)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_absolute_move_to != NULL);

    return iface->render_absolute_move_to(renderer, x, y);
}


static void
bb_item_renderer_render_absolute_move_to_missing(BbItemRenderer *renderer, int x, int y)
{
    g_error("bb_item_renderer_render_absolute_move_to() not overridden");
}


void
bb_item_renderer_render_graphic_arc(BbItemRenderer *renderer, struct _BbGraphicArc *arc)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_graphic_arc != NULL);

    return iface->render_graphic_arc(renderer, arc);
}


static void
bb_item_renderer_render_graphic_arc_missing(BbItemRenderer *renderer, struct _BbGraphicArc *arc)
{
    g_error("bb_item_renderer_render_graphic_arc() not overridden");
}


void
bb_item_renderer_render_graphic_box(BbItemRenderer *renderer, struct _BbGraphicBox *box)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_graphic_box != NULL);

    return iface->render_graphic_box(renderer, box);
}


static void
bb_item_renderer_render_graphic_box_missing(BbItemRenderer *renderer, struct _BbGraphicBox *box)
{
    g_error("bb_item_renderer_render_graphic_box() not overridden");
}


void
bb_item_renderer_render_graphic_circle(BbItemRenderer *renderer, struct _BbGraphicCircle *circle)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_graphic_circle != NULL);

    return iface->render_graphic_circle(renderer, circle);
}


static void
bb_item_renderer_render_graphic_circle_missing(BbItemRenderer *renderer, struct _BbGraphicCircle *circle)
{
    g_error("bb_item_renderer_render_graphic_circle() not overridden");
}


void
bb_item_renderer_render_graphic_line(BbItemRenderer *renderer, struct _BbGraphicLine *line)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_graphic_line != NULL);

    return iface->render_graphic_line(renderer, line);
}


static void
bb_item_renderer_render_graphic_line_missing(BbItemRenderer *renderer, struct _BbGraphicLine *line)
{
    g_error("bb_item_renderer_render_graphic_line() not overridden");
}


void
bb_item_renderer_render_graphic_path(BbItemRenderer *renderer, struct _BbGraphicPath *path)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_graphic_path != NULL);

    return iface->render_graphic_path(renderer, path);
}


static void
bb_item_renderer_render_graphic_path_missing(BbItemRenderer *renderer, struct _BbGraphicPath *path)
{
    g_error("bb_item_renderer_render_graphic_path() not overridden");
}


void
bb_item_renderer_render_relative_line_to(BbItemRenderer *renderer, int dx, int dy)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_relative_line_to != NULL);

    return iface->render_relative_line_to(renderer, dx, dy);
}


void
bb_item_renderer_render_relative_line_to_missing(BbItemRenderer *renderer, int dx, int dy)
{
    g_error("bb_item_renderer_render_relative_line_to() not overridden");
}


void
bb_item_renderer_render_relative_move_to(BbItemRenderer *renderer, int dx, int dy)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_relative_move_to != NULL);

    return iface->render_relative_move_to(renderer, dx, dy);
}


void
bb_item_renderer_render_relative_move_to_missing(BbItemRenderer *renderer, int dx, int dy)
{
    g_error("bb_item_renderer_render_relative_move_to() not overridden");
}
