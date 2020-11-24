#ifndef __BBEXTENSIONS__
#define __BBEXTENSIONS__
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


/**
 * Override a property from a base class or interface
 *
 * Functions as g_object_class_override_property(), but returns the associated GParamSpec.
 *
 * @param oclass The GObjectClass of the derived class
 * @param property_id The property_id within the derived class
 * @param name The name of the property to override
 * @returns The GParamSpec associated with the overridden property.
 */
GParamSpec*
bb_object_class_override_property(GObjectClass *oclass, guint property_id, const gchar *name);


#endif
