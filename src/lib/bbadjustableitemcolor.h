#ifndef __BBADJUSTABLEITEMCOLOR__
#define __BBADJUSTABLEITEMCOLOR__
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

#define BB_TYPE_ADJUSTABLE_ITEM_COLOR bb_adjustable_item_color_get_type()
G_DECLARE_INTERFACE(BbAdjustableItemColor, bb_adjustable_item_color, BB, ADJUSTABLE_ITEM_COLOR, GObject)


struct _BbAdjustableItemColorInterface
{
    GTypeInterface g_iface;
};


#endif
