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
#include "bbadjustableitemcolor.h"


G_DEFINE_INTERFACE(BbAdjustableItemColor, bb_adjustable_item_color, G_TYPE_OBJECT)


static void
bb_adjustable_item_color_default_init(BbAdjustableItemColorInterface *class)
{
    g_return_if_fail(class != NULL);

    g_object_interface_install_property(
        class,
        g_param_spec_int(
            "item-color",
            "Item Color",
            "The Color of this Item",
            0,
            G_MAXINT,
            0,
            G_PARAM_READWRITE
            )
        );
}


int
bb_adjustable_item_color_get_color(BbAdjustableItemColor *item)
{
    int color;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "item-color", &color,
        NULL
        );

    return color;
}


void
bb_adjustable_item_color_set_color(BbAdjustableItemColor *item, int color)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "item-color", color,
        NULL
        );
}
