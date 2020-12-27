#ifndef __BBATTRIBUTE__
#define __BBATTRIBUTE__
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
 * @file bbattribute.h
 *
 * An interface for schematic items that represent attributes
 */
#include <gtk/gtk.h>

#define BB_TYPE_ATTRIBUTE bb_attribute_get_type()
G_DECLARE_INTERFACE(BbAttribute, bb_attribute, BB, ATTRIBUTE, GObject)

struct _BbAttributeInterface
{
    GTypeInterface g_iface;

    gchar* (*get_name)(BbAttribute *attribute);
    gchar* (*get_value)(BbAttribute *attribute);
    void (*set_name)(BbAttribute *attribute, const gchar *name);
    void (*set_value)(BbAttribute *attribute, const gchar *value);
};


/**
 * @brief Get the name of the attribute
 *
 * If the item does not represent an attribute, this function returns an empty string
 *
 * @param attribute A BbAttribute
 * @return The name of the attribute
 */
gchar*
bb_attribute_get_name(BbAttribute *attribute);


/**
 * @brief Get the value of the attribute
 *
 * If the item does not represent an attribute, this function returns an empty string
 *
 * @param attribute A BbAttribute
 * @return The value of the attribute
 */
gchar*
bb_attribute_get_value(BbAttribute *attribute);


/**
 * @brief Set the name of the attribute
 *
 * If the item does not represent an attribute, this function does nothing.
 *
 * @param attribute A BbAttribute
 * @param value The new name. This value cannot be NULL.
 */
void
bb_attribute_set_name(BbAttribute *attribute, const gchar *name);


/**
 * @brief Set the value of the attribute
 *
 * If the item does not represent an attribute, this function does nothing.
 *
 * @param attribute A BbAttribute
 * @param value The new value. This value cannot be NULL.
 */
void
bb_attribute_set_value(BbAttribute *attribute, const gchar *value);

#endif
