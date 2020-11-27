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
#include <bbextensions.h>
#include "bbgraphics.h"


enum
{
    PROP_0,
    PROP_CAIRO,
    N_PROPERTIES
};


struct _BbGraphics
{
    GObject parent;

    cairo_t *cairo;
};


static void
bb_graphics_dispose(GObject *object);

static void
bb_graphics_finalize(GObject *object);

static void
bb_graphics_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphics_item_renderer_init(BbItemRendererInterface *iface);

static void
bb_graphics_render_absolute_line_to(BbItemRenderer *renderer, int x, int y);

static void
bb_graphics_render_absolute_move_to(BbItemRenderer *renderer, int x, int y);

static void
bb_graphics_render_arc(BbItemRenderer *renderer, int x, int y, int radius, int start, int sweep);

static void
bb_graphics_render_relative_line_to(BbItemRenderer *renderer, int dx, int dy);

static void
bb_graphics_render_relative_move_to(BbItemRenderer *renderer, int dx, int dy);

static void
bb_graphics_set_cairo(BbGraphics *graphics, cairo_t* cairo);

static void
bb_graphics_set_color(BbItemRenderer *renderer, int color);

static void
bb_graphics_set_fill_style(BbItemRenderer *renderer, BbFillStyle *style);

static void
bb_graphics_set_line_style(BbItemRenderer *renderer, BbLineStyle *style);

static void
bb_graphics_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbGraphics,
    bb_graphics,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ITEM_RENDERER, bb_graphics_item_renderer_init)
    )


static void
bb_graphics_class_init(BbGraphicsClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_graphics_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_graphics_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_graphics_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_graphics_set_property;

    properties[PROP_CAIRO] = bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAIRO,
        g_param_spec_pointer(
            "cairo",
            "",
            "",
            G_PARAM_READWRITE
            )
        );
}


static void
bb_graphics_close_path(BbItemRenderer *renderer)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    cairo_close_path(graphics->cairo);
}


static void
bb_graphics_dispose(GObject *object)
{
}


static void
bb_graphics_finalize(GObject *object)
{
}


cairo_t*
bb_graphics_get_cairo(BbGraphics *graphics)
{
    g_return_val_if_fail(graphics != NULL, NULL);

    return graphics->cairo;
}


static void
bb_graphics_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
            g_value_set_pointer(value, bb_graphics_get_cairo(BB_GRAPHICS(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_graphics_init(BbGraphics *graphics)
{
}


BbGraphics*
bb_graphics_new(cairo_t *cairo)
{
    return BB_GRAPHICS(g_object_new(
        BB_TYPE_GRAPHICS,
        "cairo", cairo,
        NULL
        ));
}


static void
bb_graphics_item_renderer_init(BbItemRendererInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->close_path = bb_graphics_close_path;
    iface->render_absolute_line_to = bb_graphics_render_absolute_line_to;
    iface->render_absolute_move_to = bb_graphics_render_absolute_move_to;
    iface->render_arc = bb_graphics_render_arc;
    iface->render_relative_line_to = bb_graphics_render_relative_line_to;
    iface->render_relative_move_to = bb_graphics_render_relative_move_to;
    iface->set_color = bb_graphics_set_color;
    iface->set_fill_style = bb_graphics_set_fill_style;
    iface->set_line_style = bb_graphics_set_line_style;
}


static void
bb_graphics_render_absolute_line_to(BbItemRenderer *renderer, int x, int y)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    cairo_line_to(graphics->cairo, x, y);
}


static void
bb_graphics_render_absolute_move_to(BbItemRenderer *renderer, int x, int y)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    cairo_move_to(graphics->cairo, x, y);
}


static void
bb_graphics_render_arc(BbItemRenderer *renderer, int x, int y, int radius, int start, int sweep)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    if (sweep > 0)
    {
        cairo_new_sub_path(graphics->cairo);

        cairo_arc(
            graphics->cairo,
            x,
            y,
            radius,
            bb_angle_to_radians(start),
            bb_angle_to_radians(start + sweep)
            );
    }
    else if (sweep < 0)
    {
        cairo_new_sub_path(graphics->cairo);

        cairo_arc_negative(
            graphics->cairo,
            x,
            y,
            radius,
            bb_angle_to_radians(start),
            bb_angle_to_radians(start - sweep)
            );
    }
}


static void
bb_graphics_render_relative_line_to(BbItemRenderer *renderer, int dx, int dy)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    cairo_rel_line_to(graphics->cairo, dx, dy);
}


static void
bb_graphics_render_relative_move_to(BbItemRenderer *renderer, int dx, int dy)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    cairo_rel_move_to(graphics->cairo, dx, dy);
}


static void
bb_graphics_set_color(BbItemRenderer *renderer, int color)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    cairo_set_source_rgb(graphics->cairo, 0.0, 1.0, 0.0);  // TODO
}


static void
bb_graphics_set_fill_style(BbItemRenderer *renderer, BbFillStyle *style)
{

}


static void
bb_graphics_set_line_style(BbItemRenderer *renderer, BbLineStyle *style)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    cairo_set_line_width(graphics->cairo, 10.0);
}


static void
bb_graphics_set_cairo(BbGraphics *graphics, cairo_t* cairo)
{
    g_return_if_fail(graphics != NULL);

    if (graphics->cairo != cairo)
    {
        if (graphics->cairo != NULL)
        {
            cairo_destroy(graphics->cairo);
        }

        graphics->cairo = cairo;

        if (graphics->cairo != NULL)
        {
            cairo_reference(graphics->cairo);
        }
    }
}


static void
bb_graphics_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
            bb_graphics_set_cairo(BB_GRAPHICS(object), g_value_get_pointer(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
