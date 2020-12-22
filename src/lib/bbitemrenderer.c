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
bb_item_renderer_render_arc_missing(BbItemRenderer *renderer, int x, int y, int radius, int start, int sweep);

static void
bb_item_renderer_render_relative_line_to_missing(BbItemRenderer *renderer, int dx, int dy);

static void
bb_item_renderer_render_relative_move_to_missing(BbItemRenderer *renderer, int dx, int dy);

static void
bb_item_renderer_set_color_missing(BbItemRenderer *renderer, int color);

static void
bb_item_renderer_set_fill_style_missing(BbItemRenderer *renderer, BbFillStyle *style);

static void
bb_item_renderer_set_line_style_missing(BbItemRenderer *renderer, BbLineStyle *style);


G_DEFINE_INTERFACE(BbItemRenderer, bb_item_renderer, G_TYPE_OBJECT)


void
bb_item_renderer_close_path(BbItemRenderer *renderer)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->close_path != NULL);

    return iface->close_path(renderer);
}


void
bb_item_renderer_close_path_missing(BbItemRenderer *renderer)
{
    g_error("bb_item_renderer_close_path() not overridden");
}


static void
bb_item_renderer_default_init(BbItemRendererInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->close_path = bb_item_renderer_close_path;
    iface->render_absolute_line_to = bb_item_renderer_render_absolute_line_to_missing;
    iface->render_absolute_move_to = bb_item_renderer_render_absolute_move_to_missing;
    iface->render_arc = bb_item_renderer_render_arc_missing;
    iface->render_relative_line_to = bb_item_renderer_render_relative_line_to_missing;
    iface->render_relative_move_to = bb_item_renderer_render_relative_move_to_missing;
    iface->set_color = bb_item_renderer_set_color_missing;
    iface->set_fill_style = bb_item_renderer_set_fill_style_missing;
    iface->set_line_style = bb_item_renderer_set_line_style_missing;
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
bb_item_renderer_render_arc(BbItemRenderer *renderer, int x, int y, int radius, int start, int sweep)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_arc != NULL);

    return iface->render_arc(renderer, x, y, radius, start, sweep);
}


static void
bb_item_renderer_render_arc_missing(BbItemRenderer *renderer, int x, int y, int radius, int start, int sweep)
{
    g_error("bb_item_renderer_render_arc() not overridden");
}


void
bb_item_renderer_render_relative_line_to(BbItemRenderer *renderer, int dx, int dy)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_relative_line_to != NULL);

    return iface->render_relative_line_to(renderer, dx, dy);
}


static void
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


void
bb_item_renderer_render_text(
    BbItemRenderer *renderer,
    int insert_x,
    int insert_y,
    char *text
    )
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->render_text != NULL);

    return iface->render_text(renderer, insert_x, insert_y, text);
}


static void
bb_item_renderer_render_text_missing(
    BbItemRenderer *renderer,
    int insert_x,
    int insert_y,
    char *text
    )
{
    g_error("bb_item_renderer_render_text() not overridden");
}


void
bb_item_renderer_set_color(BbItemRenderer *renderer, int color)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->set_color != NULL);

    return iface->set_color(renderer, color);
}


static void
bb_item_renderer_set_color_missing(BbItemRenderer *renderer, int color)
{
    g_error("bb_item_renderer_set_color() not overridden");
}


void
bb_item_renderer_set_fill_style(BbItemRenderer *renderer, BbFillStyle *style)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->set_fill_style != NULL);

    return iface->set_fill_style(renderer, style);
}


static void
bb_item_renderer_set_fill_style_missing(BbItemRenderer *renderer, BbFillStyle *style)
{
    g_error("bb_item_renderer_set_fill_style() not overridden");
}


void
bb_item_renderer_set_line_style(BbItemRenderer *renderer, BbLineStyle *style)
{
    BbItemRendererInterface *iface = BB_ITEM_RENDERER_GET_IFACE(renderer);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->set_line_style != NULL);

    return iface->set_line_style(renderer, style);
}


static void
bb_item_renderer_set_line_style_missing(BbItemRenderer *renderer, BbLineStyle *style)
{
    g_error("bb_item_renderer_set_line_style() not overridden");
}
