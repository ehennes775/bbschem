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
#include <bbextensions.h>
#include "bbcellrenderercolorswatch.h"


enum
{
    PROP_0,
    PROP_FILL_TYPE
};


struct _BbCellRendererColorSwatch
{
    GtkCellRendererText parent;

    GdkRGBA color;
};


G_DEFINE_TYPE(BbCellRendererColorSwatch, bb_cell_renderer_color_swatch, GTK_TYPE_CELL_RENDERER);


static void
bb_cell_renderer_color_swatch_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_cell_renderer_color_swatch_render(
    GtkCellRenderer *cell,
    cairo_t *cr,
    GtkWidget *widget,
    const GdkRectangle *background_area,
    const GdkRectangle *cell_area,
    GtkCellRendererState flags
    );

static void
bb_cell_renderer_color_swatch_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static void
bb_cell_renderer_color_swatch_class_init(BbCellRendererColorSwatchClass *klasse)
{
    G_OBJECT_CLASS(klasse)->get_property = bb_cell_renderer_color_swatch_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_cell_renderer_color_swatch_set_property;

    GTK_CELL_RENDERER_CLASS(klasse)->render = bb_cell_renderer_color_swatch_render;

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_FILL_TYPE,
        g_param_spec_boxed(
            "color",
            "",
            "",
            GDK_TYPE_RGBA,
            G_PARAM_READWRITE
            )
        );
}


const GdkRGBA*
bb_cell_renderer_color_swatch_get_color(BbCellRendererColorSwatch *renderer)
{
    g_return_val_if_fail(BB_IS_CELL_RENDERER_COLOR_SWATCH(renderer), NULL);

    return &renderer->color;
}


static void
bb_cell_renderer_color_swatch_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_FILL_TYPE:
            g_value_set_boxed(
                value,
                bb_cell_renderer_color_swatch_get_color(BB_CELL_RENDERER_COLOR_SWATCH(object))
                );
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_cell_renderer_color_swatch_init(BbCellRendererColorSwatch *renderer)
{
}


__attribute__((constructor)) void
bb_cell_renderer_color_swatch_register()
{
    bb_cell_renderer_color_swatch_get_type();
}


static void
bb_cell_renderer_color_swatch_render(
    GtkCellRenderer *cell,
    cairo_t *context,
    GtkWidget *widget,
    const GdkRectangle *background_area,
    const GdkRectangle *cell_area,
    GtkCellRendererState flags
    )
{
    GtkStateFlags state = gtk_widget_get_state_flags(widget);

    GtkStyleContext *style = gtk_widget_get_style_context(widget);

    GdkRGBA text_color;
    gtk_style_context_get_color(style, state, &text_color);

    cairo_set_source_rgba(
        context,
        text_color.red,
        text_color.green,
        text_color.blue,
        text_color.alpha
        );

    cairo_set_line_width(context, 1);

    gdouble offset = 0.5;

    cairo_move_to(
        context,
        (double) cell_area->x + offset,
        (double) cell_area->y + offset
        );

    cairo_line_to(
        context,
        (double) cell_area->x + (double) cell_area->width - offset,
        (double) cell_area->y + offset
        );

    cairo_line_to(
        context,
        (double) cell_area->x + (double) cell_area->width - offset,
        (double) cell_area->y + (double) cell_area->height - offset
        );

    cairo_line_to(
        context,
        (double) cell_area->x + offset,
        (double) cell_area->y + (double) cell_area->height - offset
        );

    cairo_close_path(context);

    cairo_set_source_rgba(
        context,
        BB_CELL_RENDERER_COLOR_SWATCH(cell)->color.red,
        BB_CELL_RENDERER_COLOR_SWATCH(cell)->color.green,
        BB_CELL_RENDERER_COLOR_SWATCH(cell)->color.blue,
        BB_CELL_RENDERER_COLOR_SWATCH(cell)->color.alpha
        );

    cairo_fill_preserve(context);

    cairo_set_source_rgba(
        context,
        text_color.red,
        text_color.green,
        text_color.blue,
        text_color.alpha
        );

    cairo_stroke(context);
}


void
bb_cell_renderer_color_swatch_set_color(BbCellRendererColorSwatch *renderer, GdkRGBA *color)
{
    g_return_if_fail(color != NULL);
    g_return_if_fail(BB_IS_CELL_RENDERER_COLOR_SWATCH(renderer));

    renderer->color = *color;
}


static void
bb_cell_renderer_color_swatch_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_FILL_TYPE:
            bb_cell_renderer_color_swatch_set_color(
                BB_CELL_RENDERER_COLOR_SWATCH(object),
                g_value_get_boxed(value)
                );
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
