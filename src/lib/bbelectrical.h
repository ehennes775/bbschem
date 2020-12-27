#ifndef __BBELECTRICAL__
#define __BBELECTRICAL__
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
/**
 * @file bbelectrical.h
 *
 * @brief An interface for schematic items that can represent electrical entities (i.e. not graphical)
 */
#include <gtk/gtk.h>
#include "bbattribute.h"


#define BB_TYPE_ELECTRICAL bb_electrical_get_type()
G_DECLARE_INTERFACE(BbElectrical, bb_electrical, BB, ELECTRICAL, GObject)


struct _BbElectricalInterface
{
    GTypeInterface g_iface;

    void (*add_attribute)(BbElectrical *electrical, BbAttribute *attribute);
    void (*foreach)(BbElectrical *electrical, GFunc func, gpointer user_data);
};


/**
 * Add an attribute to the electrical item
 *
 * @param electrical The electrical item -- must not be NULL.
 * @param attribute The attribute -- must not be NULL.
 */
void
bb_electrical_add_attribute(BbElectrical *electrical, BbAttribute *attribute);


/**
 * Iterate over all attributes in the electrical item
 *
 * @param electrical The electrical item -- must not be NULL.
 * @param func The function to call for each attribute -- must not be NULL.
 * @param user_data The data to pass to the function.
 */
void
bb_electrical_foreach(BbElectrical *electrical, GFunc func, gpointer user_data);


#endif
