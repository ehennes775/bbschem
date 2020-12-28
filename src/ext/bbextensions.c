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

#include "bbextensions.h"


gboolean
bb_boolean_equals(gboolean a, gboolean b)
{
    return (a && b) || (!a && !b);
}


GParamSpec*
bb_object_class_override_property(GObjectClass *object_class, guint property_id, const gchar *name)
{
    g_object_class_override_property(object_class, property_id, name);

    return g_object_class_find_property(object_class, name);
}


GParamSpec*
bb_object_class_install_property(GObjectClass *object_class, guint property_id, GParamSpec *pspec)
{
    g_object_class_install_property(object_class, property_id, pspec);

    return pspec;
}
