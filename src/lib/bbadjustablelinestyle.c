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
#include "bbadjustablelinestyle.h"


G_DEFINE_INTERFACE(BbAdjustableLineStyle, bb_adjustable_line_style, G_TYPE_OBJECT)


static void
bb_adjustable_line_style_default_init(BbAdjustableLineStyleInterface *class)
{
    g_return_if_fail(class != NULL);

    g_object_interface_install_property(
        class,
        g_param_spec_int(
            "width",
            "Width",
            "Width",
            0,
            G_MAXINT,
            0,
            G_PARAM_READWRITE
            )
        );

    g_object_interface_install_property(
        class,
        g_param_spec_int(
            "line-type",
            "Line Type",
            "Line Type",
            0,
            G_MAXINT,
            0,
            G_PARAM_READWRITE
            )
        );
}


int
bb_adjustable_line_style_get_cap_type(BbAdjustableLineStyle *item)
{
    int type;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "cap-type", &type,
        NULL
        );

    return type;
}


int
bb_adjustable_line_style_get_dash_length(BbAdjustableLineStyle *item)
{
    int length;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "dash-length", &length,
        NULL
        );

    return length;
}


int
bb_adjustable_line_style_get_dash_space(BbAdjustableLineStyle *item)
{
    int space;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "dash-space", &space,
        NULL
        );

    return space;
}


int
bb_adjustable_line_style_get_dash_type(BbAdjustableLineStyle *item)
{
    int type;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "dash-type", &type,
        NULL
        );

    return type;
}


int
bb_adjustable_line_style_get_line_width(BbAdjustableLineStyle *item)
{
    int width;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "line-width", &width,
        NULL
        );

    return width;
}


void
bb_adjustable_line_style_set_cap_type(BbAdjustableLineStyle *item, int type)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "cap-type", type,
        NULL
        );
}


void
bb_adjustable_line_style_set_dash_length(BbAdjustableLineStyle *item, int length)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "dash-length", length,
        NULL
        );
}


void
bb_adjustable_line_style_set_dash_space(BbAdjustableLineStyle *item, int space)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "dash-space", space,
        NULL
        );
}


void
bb_adjustable_line_style_set_dash_type(BbAdjustableLineStyle *item, int type)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "dash-type", type,
        NULL
        );
}


void
bb_adjustable_line_style_set_line_width(BbAdjustableLineStyle *item, int width)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "line-width", width,
        NULL
        );
}
