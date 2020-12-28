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

#include <math.h>
#include <gtk/gtk.h>
#include <bblibrary.h>
#include <bbextensions.h>
#include <bbcolors.h>
#include "bbgraphics.h"


enum
{
    PROP_0,
    PROP_CAIRO,
    PROP_WIDGET_MATRIX,
    PROP_REVEAL,
    PROP_STYLE,
    N_PROPERTIES
};


struct _BbGraphics
{
    GObject parent;

    cairo_t *cairo;

    gboolean reveal;
    
    GtkStyleContext *style;

    /**
     * A matrix for converting widget coordinates to window coordinates
     */
    cairo_matrix_t widget_matrix;
};


typedef gboolean (*GridShowFunc)(int value);


typedef struct _GridGeometry GridGeometry;

struct _GridGeometry
{
    cairo_matrix_t matrix;
    double width;
    double min_x;
    double min_y;
    double max_x;
    double max_y;
};


static void
calculate_text_adjustment(PangoLayout *layout, BbTextAlignment alignment, int *dx, int *dy);

static void
bb_graphics_dispose(GObject *object);

static void
bb_graphics_draw_grid_horizontal(BbGraphics *graphics, GridGeometry *geometry, GridShowFunc show);

static void
bb_graphics_draw_grid_vertical(BbGraphics *graphics, GridGeometry *geometry, GridShowFunc show);

static void
bb_graphics_finalize(GObject *object);

static void
bb_graphics_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static gboolean 
bb_graphics_get_reveal(BbItemRenderer *renderer);

static gboolean
bb_graphics_is_major(int value);

static gboolean
bb_graphics_is_minor(int value);

static gboolean
bb_graphics_is_origin(int value);

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
bb_graphics_render_text(
    BbItemRenderer *renderer,
    int insert_x,
    int insert_y,
    char *text
    );

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

static void
bb_graphics_set_style(BbGraphics *graphics, GtkStyleContext *style);

static void
bb_graphics_set_widget_matrix(BbGraphics *graphics, cairo_matrix_t *widget_matrix);

static void
bb_graphics_set_reveal(BbItemRenderer *renderer, gboolean reveal);


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

    /* From BbItemRenderer */
    properties[PROP_REVEAL] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_REVEAL,
        "reveal"
        );

    properties[PROP_CAIRO] = bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_CAIRO,
        g_param_spec_pointer(
            "cairo",
            "",
            "",
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
            )
        );

    properties[PROP_WIDGET_MATRIX] = bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_WIDGET_MATRIX,
        g_param_spec_pointer(
            "widget-matrix",
            "",
            "",
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
            )
        );

    properties[PROP_STYLE] = bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_STYLE,
        g_param_spec_object(
            "style",
            "",
            "",
            GTK_TYPE_STYLE_CONTEXT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
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


void
bb_graphics_draw_closed_shape(
    BbItemRenderer *renderer,
    int color,
    BbFillStyle *fill_style,
    BbLineStyle *line_style,
    BbClosedShapeDrawer *drawer
    )
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(BB_IS_GRAPHICS(graphics));

    bb_closed_shape_drawer_draw_outline(drawer, renderer);

    cairo_stroke(graphics->cairo);
}


void
bb_graphics_draw_grid(BbGraphics *graphics, int grid_size)
{
    cairo_save(graphics->cairo);

    GridGeometry horizontal;
    GridGeometry vertical;

    cairo_get_matrix(graphics->cairo, &horizontal.matrix);
    horizontal.matrix.xx *= grid_size;
    horizontal.width = ABS(1.0 / horizontal.matrix.xx);

    cairo_get_matrix(graphics->cairo, &vertical.matrix);
    vertical.matrix.yy *= grid_size;
    vertical.width = ABS(1.0 / vertical.matrix.yy);

    double x[2];
    cairo_set_matrix(graphics->cairo, &horizontal.matrix);
    cairo_clip_extents(graphics->cairo, &x[0], &horizontal.min_y, &x[1], &horizontal.max_y);
    horizontal.min_x = ceil(MIN(x[0], x[1]));
    horizontal.max_x = floor(MAX(x[0], x[1]));

    double y[2];
    cairo_set_matrix(graphics->cairo, &vertical.matrix);
    cairo_clip_extents(graphics->cairo, &vertical.min_x, &y[0], &vertical.max_x, &y[1]);
    vertical.min_y = ceil(MIN(y[0], y[1]));
    vertical.max_y = floor(MAX(y[0], y[1]));

    cairo_set_source_rgb(graphics->cairo, 0.09, 0.09, 0.09);
    bb_graphics_draw_grid_horizontal(graphics, &horizontal, bb_graphics_is_minor);
    bb_graphics_draw_grid_vertical(graphics, &vertical, bb_graphics_is_minor);

    cairo_set_source_rgb(graphics->cairo, 0.125, 0.125, 0.125);
    bb_graphics_draw_grid_horizontal(graphics, &horizontal, bb_graphics_is_major);
    bb_graphics_draw_grid_vertical(graphics, &vertical, bb_graphics_is_major);

    cairo_set_source_rgb(graphics->cairo, 0.25, 0.25, 0.25);
    bb_graphics_draw_grid_horizontal(graphics, &horizontal, bb_graphics_is_origin);
    bb_graphics_draw_grid_vertical(graphics, &vertical, bb_graphics_is_origin);

    cairo_restore(graphics->cairo);
}


static void
bb_graphics_draw_grid_horizontal(BbGraphics *graphics, GridGeometry *geometry, GridShowFunc show)
{
    cairo_set_matrix(graphics->cairo, &geometry->matrix);
    cairo_set_line_width(graphics->cairo, geometry->width);

    for (int x = geometry->min_x; x <= geometry->max_x; x++)
    {
        if (show(x))
        {
            cairo_move_to(graphics->cairo, x, geometry->min_y);
            cairo_line_to(graphics->cairo, x, geometry->max_y);
        }
    }

    cairo_stroke(graphics->cairo);
}


static void
bb_graphics_draw_grid_vertical(BbGraphics *graphics, GridGeometry *geometry, GridShowFunc show)
{
    cairo_set_matrix(graphics->cairo, &geometry->matrix);
    cairo_set_line_width(graphics->cairo, geometry->width);

    for (int y = geometry->min_y; y <= geometry->max_y; y++)
    {
        if (show(y))
        {
            cairo_move_to(graphics->cairo, geometry->min_x, y);
            cairo_line_to(graphics->cairo, geometry->max_x, y);
        }
    }

    cairo_stroke(graphics->cairo);
}


void
bb_graphics_draw_select_box(BbGraphics *graphics, int x0, int y0, int x1, int y1)
{
    g_return_if_fail(BB_IS_GRAPHICS(graphics));

    gtk_style_context_save(graphics->style);
    gtk_style_context_add_class(graphics->style, "schematicselectrubber");

    cairo_save(graphics->cairo);
    cairo_set_matrix(graphics->cairo, &graphics->widget_matrix);

    int x = MIN(x0, x1);
    int y = MIN(y0, y1);
    int w = abs(x1 - x0);
    int h = abs(y1 - y0);

    gtk_render_background(graphics->style, graphics->cairo, x, y, w, h);
    gtk_render_frame(graphics->style, graphics->cairo, x, y, w, h);

    gtk_style_context_restore(graphics->style);
    cairo_restore(graphics->cairo);
}


void
bb_graphics_draw_zoom_box(BbGraphics *graphics, int x0, int y0, int x1, int y1)
{
    g_return_if_fail(BB_IS_GRAPHICS(graphics));

    gtk_style_context_save(graphics->style);
    gtk_style_context_add_class(graphics->style, "schematiczoomrubber");

    cairo_save(graphics->cairo);
    cairo_set_matrix(graphics->cairo, &graphics->widget_matrix);

    int x = MIN(x0, x1);
    int y = MIN(y0, y1);
    int w = abs(x1 - x0) + 1;
    int h = abs(y1 - y0) + 1;

    gtk_render_background(graphics->style, graphics->cairo, x, y, w, h);
    gtk_render_frame(graphics->style, graphics->cairo, x, y, w, h);

    gtk_style_context_restore(graphics->style);
    cairo_restore(graphics->cairo);
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

        case PROP_REVEAL:
            g_value_set_boolean(value, bb_graphics_get_reveal(BB_ITEM_RENDERER(object)));
            break;

        case PROP_STYLE:
            g_value_set_object(value, bb_graphics_get_style(BB_GRAPHICS(object)));
            break;

        case PROP_WIDGET_MATRIX:
            g_value_set_pointer(value, bb_graphics_get_widget_matrix(BB_GRAPHICS(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static gboolean
bb_graphics_get_reveal(BbItemRenderer *renderer)
{
   BbGraphics *graphics = BB_GRAPHICS(renderer);
   g_return_val_if_fail(BB_IS_GRAPHICS(graphics), FALSE);
   
   return graphics->reveal;
}


GtkStyleContext*
bb_graphics_get_style(BbGraphics *graphics)
{
    g_return_val_if_fail(BB_IS_GRAPHICS(graphics), NULL);

    return graphics->style;
}


cairo_matrix_t*
bb_graphics_get_widget_matrix(BbGraphics *graphics)
{
    g_return_val_if_fail(BB_IS_GRAPHICS(graphics), NULL);

    return &graphics->widget_matrix;
}


static void
bb_graphics_init(BbGraphics *graphics)
{
}


static gboolean
bb_graphics_is_major(int value)
{
    return !bb_graphics_is_origin(value) && ((ABS(value) % 5) == 0);
}


static gboolean
bb_graphics_is_minor(int value)
{
    return !bb_graphics_is_major(value);
}


static gboolean
bb_graphics_is_origin(int value)
{
    return (value == 0);
}


BbGraphics*
bb_graphics_new(cairo_t *cairo, cairo_matrix_t *widget_matrix, gboolean reveal, GtkStyleContext *style)
{
    return BB_GRAPHICS(g_object_new(
        BB_TYPE_GRAPHICS,
        "cairo", cairo,
        "widget-matrix", widget_matrix,
        "reveal", reveal,
        "style", style,
        NULL
        ));
}


static void
bb_graphics_item_renderer_init(BbItemRendererInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->draw_closed_shape = bb_graphics_draw_closed_shape;
    iface->close_path = bb_graphics_close_path;
    iface->get_reveal = bb_graphics_get_reveal;
    iface->render_absolute_line_to = bb_graphics_render_absolute_line_to;
    iface->render_absolute_move_to = bb_graphics_render_absolute_move_to;
    iface->render_arc = bb_graphics_render_arc;
    iface->render_relative_line_to = bb_graphics_render_relative_line_to;
    iface->render_relative_move_to = bb_graphics_render_relative_move_to;
    iface->render_text = bb_graphics_render_text;
    iface->set_color = bb_graphics_set_color;
    iface->set_fill_style = bb_graphics_set_fill_style;
    iface->set_line_style = bb_graphics_set_line_style;
    iface->set_reveal = bb_graphics_set_reveal;
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
bb_graphics_render_text(
    BbItemRenderer *renderer,
    int insert_x,
    int insert_y,
    char *text
    )
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);

    cairo_save(graphics->cairo);

    PangoFontDescription *font_description = pango_font_description_from_string("Sans");

    PangoLayout *layout = pango_cairo_create_layout(graphics->cairo);

    pango_font_description_set_size(font_description, 100 * PANGO_SCALE);
    pango_layout_set_font_description(layout, font_description);
    pango_layout_set_spacing(layout, 40000);
    pango_layout_set_markup(layout, text, -1);

    cairo_move_to(graphics->cairo, insert_x, insert_y);
    cairo_scale(graphics->cairo, 1.0, -1.0);

    double rotation = bb_angle_to_radians(0);
    cairo_rotate(graphics->cairo, -rotation);

    int dx = 0;
    int dy = 0;

    calculate_text_adjustment(layout, BB_TEXT_ALIGNMENT_LOWER_LEFT, &dx, &dy);

    cairo_rel_move_to(
        graphics->cairo,
        (double) dx / (double) PANGO_SCALE,
        (double) dy / (double) PANGO_SCALE
        );

    pango_cairo_show_layout(graphics->cairo, layout);

    pango_font_description_free(font_description);
    g_clear_object(&layout);

    cairo_restore(graphics->cairo);
}


static void
calculate_text_adjustment(PangoLayout *layout, BbTextAlignment alignment, int *dx, int *dy)
{
    *dx = 0;

    double alignment_x = bb_text_alignment_get_alignment_x(alignment);

    if (alignment_x > 0.0)
    {
        int height;
        int width;

        pango_layout_get_size(layout, &width, &height);

        *dx = - (int) round(alignment_x * width);
    }

    *dy = 0;

    double alignment_y = bb_text_alignment_get_alignment_y(alignment);

    if (alignment_y > 0.0)
    {
        PangoLayoutIter *iter = pango_layout_get_iter(layout);

        while (!pango_layout_iter_at_last_line(iter))
        {
            pango_layout_iter_next_line(iter);
        }

        *dy = - (int) round(alignment_y * pango_layout_iter_get_baseline(iter));
    }
}


static void
bb_graphics_render_relative_move_to(BbItemRenderer *renderer, int dx, int dy)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    cairo_rel_move_to(graphics->cairo, dx, dy);
}


/*
 * Temporary until color schemes are implemented
 */
static void
bb_graphics_set_color(BbItemRenderer *renderer, int color)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(graphics != NULL);
    g_return_if_fail(graphics->cairo != NULL);

    switch (color)
    {
        case BB_COLOR_ATTRIBUTE:
            cairo_set_source_rgb(graphics->cairo, 1.0, 1.0, 0.0);
            break;

        case BB_COLOR_BACKGROUND:
            cairo_set_source_rgb(graphics->cairo, 0.0, 0.0, 0.0);
            break;

        case BB_COLOR_BOUNDING_BOX:
            cairo_set_source_rgb(graphics->cairo, 1.0, 0.65, 0.0);
            break;

        case BB_COLOR_BUS:
            cairo_set_source_rgb(graphics->cairo, 0.0, 1.0, 0.0);
            break;

        case BB_COLOR_DETACHED_ATTRIBUTE:
            cairo_set_source_rgb(graphics->cairo, 1.0, 0.0, 0.0);
            break;

        case BB_COLOR_GRAPHIC:
            cairo_set_source_rgb(graphics->cairo, 0.0, 1.0, 0.0);
            break;

        case BB_COLOR_JUNCTION:
            cairo_set_source_rgb(graphics->cairo, 0.0, 1.0, 1.0);  // TODO
            break;

        case BB_COLOR_LOGIC_BUBBLE:
            cairo_set_source_rgb(graphics->cairo, 0.0, 1.0, 1.0);
            break;

        case BB_COLOR_MAJOR_GRID:
            cairo_set_source_rgb(graphics->cairo, 0.125, 0.125, 0.125);
            break;

        case BB_COLOR_MINOR_GRID:
            cairo_set_source_rgb(graphics->cairo, 0.09, 0.09, 0.09);
            break;

        case BB_COLOR_NET:
            cairo_set_source_rgb(graphics->cairo, 0.0, 0.0, 1.0);
            break;

        case BB_COLOR_PIN:
            cairo_set_source_rgb(graphics->cairo, 1.0, 1.0, 1.0);
            break;

        case BB_COLOR_ZOOM_BOX:
            cairo_set_source_rgb(graphics->cairo, 0.0, 1.0, 1.0);
            break;

        case BB_COLOR_SELECT:
            cairo_set_source_rgb(graphics->cairo, 1.0, 0.65, 0.0);
            break;

        case BB_COLOR_TEXT:
            cairo_set_source_rgb(graphics->cairo, 0.0, 1.0, 0.0);
            break;

        default:
            cairo_set_source_rgb(graphics->cairo, 0.0, 1.0, 0.0);
            break;
    }
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

        g_object_notify_by_pspec(G_OBJECT(graphics), properties[PROP_CAIRO]);
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

        case PROP_REVEAL:
            bb_graphics_set_reveal(BB_ITEM_RENDERER(object), g_value_get_boolean(value));
            break;

        case PROP_STYLE:
            bb_graphics_set_style(BB_GRAPHICS(object), g_value_get_object(value));
            break;

        case PROP_WIDGET_MATRIX:
            bb_graphics_set_widget_matrix(BB_GRAPHICS(object), g_value_get_pointer(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_graphics_set_reveal(BbItemRenderer *renderer, gboolean reveal)
{
    BbGraphics *graphics = BB_GRAPHICS(renderer);
    g_return_if_fail(BB_IS_GRAPHICS(graphics));

    if (!!graphics->reveal != !!reveal)
    {
        graphics->reveal = reveal;

        g_object_notify_by_pspec(G_OBJECT(graphics), properties[PROP_REVEAL]);
    }
}


static void
bb_graphics_set_style(BbGraphics *graphics, GtkStyleContext *style)
{
    g_return_if_fail(graphics != NULL);

    if (graphics->style != style)
    {
        if (graphics->style != NULL)
        {
            g_object_unref(graphics->style);
        }

        graphics->style = style;

        if (graphics->style != NULL)
        {
            g_object_ref(graphics->style);
        }

        g_object_notify_by_pspec(G_OBJECT(graphics), properties[PROP_STYLE]);
    }
}


static void
bb_graphics_set_widget_matrix(BbGraphics *graphics, cairo_matrix_t *widget_matrix)
{
    g_return_if_fail(BB_IS_GRAPHICS(graphics));
    g_return_if_fail(widget_matrix != NULL);

    graphics->widget_matrix = *widget_matrix;

    g_object_notify_by_pspec(G_OBJECT(graphics), properties[PROP_WIDGET_MATRIX]);
}
