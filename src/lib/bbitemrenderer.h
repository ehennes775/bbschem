#ifndef __BBITEMRENDERER__
#define __BBITEMRENDERER__
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

/* forward declarations */

struct _BbGraphicArc;
struct _BbGraphicBox;
struct _BbGraphicCircle;
struct _BbGraphicLine;
struct _BbGraphicPath;


#define BB_TYPE_ITEM_RENDERER bb_item_renderer_get_type()
G_DECLARE_INTERFACE(BbItemRenderer, bb_item_renderer, BB, ITEM_RENDERER, GObject)

struct _BbItemRendererInterface
{
    GTypeInterface g_iface;

    void (*render_absolute_line_to)(BbItemRenderer *renderer, int x, int y);
    void (*render_absolute_move_to)(BbItemRenderer *renderer, int x, int y);
    void (*render_graphic_arc)(BbItemRenderer *renderer, struct _BbGraphicArc *arc);
    void (*render_graphic_box)(BbItemRenderer *renderer, struct _BbGraphicBox *box);
    void (*render_graphic_circle)(BbItemRenderer *renderer, struct _BbGraphicCircle *circle);
    void (*render_graphic_line)(BbItemRenderer *renderer, struct _BbGraphicLine *line);
    void (*render_graphic_path)(BbItemRenderer *renderer, struct _BbGraphicPath *path);
    void (*render_relative_line_to)(BbItemRenderer *renderer, int dx, int dy);
    void (*render_relative_move_to)(BbItemRenderer *renderer, int dx, int dy);
};


void
bb_item_renderer_render_absolute_line_to(BbItemRenderer *renderer, int x, int y);

void
bb_item_renderer_render_absolute_move_to(BbItemRenderer *renderer, int x, int y);

void
bb_item_renderer_render_graphic_arc(BbItemRenderer *renderer, struct _BbGraphicArc *arc);

void
bb_item_renderer_render_graphic_box(BbItemRenderer *renderer, struct _BbGraphicBox *box);

void
bb_item_renderer_render_graphic_circle(BbItemRenderer *renderer, struct _BbGraphicCircle *circle);

void
bb_item_renderer_render_graphic_line(BbItemRenderer *renderer, struct _BbGraphicLine *line);

void
bb_item_renderer_render_graphic_path(BbItemRenderer *renderer, struct _BbGraphicPath *path);

void
bb_item_renderer_render_relative_line_to(BbItemRenderer *renderer, int dx, int dy);

void
bb_item_renderer_render_relative_move_to(BbItemRenderer *renderer, int dx, int dy);

#endif
