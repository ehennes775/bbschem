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
#include "bbelectrical.h"


static void
bb_electrical_add_attribute_missing(BbElectrical *electrical, BbAttribute *attribute);

static void
bb_electrical_foreach_missing(BbElectrical *electrical, GFunc func, gpointer user_data);


G_DEFINE_INTERFACE(BbElectrical, bb_electrical, G_TYPE_OBJECT)


static void
bb_electrical_default_init(BbElectricalInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->add_attribute = bb_electrical_add_attribute_missing;
    iface->foreach = bb_electrical_foreach_missing;
}


void
bb_electrical_add_attribute(BbElectrical *electrical, BbAttribute *attribute)
{
    g_return_if_fail(BB_IS_ELECTRICAL(electrical));
    g_return_if_fail(BB_IS_ATTRIBUTE(attribute));

    BbElectricalInterface *iface = BB_ELECTRICAL_GET_IFACE(electrical);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->add_attribute != NULL);

    iface->add_attribute(electrical, attribute);
}


static void
bb_electrical_add_attribute_missing(BbElectrical *electrical, BbAttribute *attribute)
{
    g_error("bb_electrical_add_attribute() not overridden");
}


void
bb_electrical_foreach(BbElectrical *electrical, GFunc func, gpointer user_data)
{
    g_return_if_fail(BB_IS_ELECTRICAL(electrical));
    g_return_if_fail(func != NULL);

    BbElectricalInterface *iface = BB_ELECTRICAL_GET_IFACE(electrical);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->foreach != NULL);

    iface->foreach(electrical, func, user_data);
}


static void
bb_electrical_foreach_missing(BbElectrical *electrical, GFunc func, gpointer user_data)
{
    g_error("bb_electrical_foreach() not overridden");
}
