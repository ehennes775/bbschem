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

void
bb_item_renderer_render_graphic_arc_missing(BbItemRenderer *renderer, struct _BbGraphicArc *arc);

void
bb_item_renderer_render_graphic_box_missing(BbItemRenderer *renderer, struct _BbGraphicBox *box);

void
bb_item_renderer_render_graphic_circle_missing(BbItemRenderer *renderer, struct _BbGraphicCircle *circle);

void
bb_item_renderer_render_graphic_line_missing(BbItemRenderer *renderer, struct _BbGraphicLine *line);

void
bb_item_renderer_render_graphic_path_missing(BbItemRenderer *renderer, struct _BbGraphicPath *path);


G_DEFINE_INTERFACE(BbItemRenderer, bb_item_renderer, G_TYPE_OBJECT)


static void
bb_item_renderer_default_init(BbItemRendererInterface *class)
{
    g_return_if_fail(class != NULL);

    class->render_graphic_arc = bb_item_renderer_render_graphic_arc_missing;
    class->render_graphic_box = bb_item_renderer_render_graphic_box_missing;
    class->render_graphic_circle = bb_item_renderer_render_graphic_circle_missing;
    class->render_graphic_line = bb_item_renderer_render_graphic_line_missing;
    class->render_graphic_path = bb_item_renderer_render_graphic_path_missing;
}


void
bb_item_renderer_render_graphic_arc(BbItemRenderer *renderer, struct _BbGraphicArc *arc)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_graphic_arc != NULL);

    return iface->render_graphic_arc(renderer, arc);
}


void
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


void
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


void
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


void
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


void
bb_item_renderer_render_graphic_path_missing(BbItemRenderer *renderer, struct _BbGraphicPath *path)
{
    g_error("bb_item_renderer_render_graphic_path() not overridden");
}
