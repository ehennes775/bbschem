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
#include "bbattribute.h"


static gchar*
bb_attribute_get_name_missing(BbAttribute *attribute);

static gchar*
bb_attribute_get_value_missing(BbAttribute *attribute);

static void
bb_attribute_set_name_missing(BbAttribute *attribute, const gchar *name);

static void
bb_attribute_set_value_missing(BbAttribute *attribute, const gchar *value);


G_DEFINE_INTERFACE(BbAttribute, bb_attribute, G_TYPE_OBJECT)


static void
bb_attribute_default_init(BbAttributeInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_name = bb_attribute_get_name_missing;
    iface->get_value = bb_attribute_get_value_missing;
    iface->set_name = bb_attribute_set_name_missing;
    iface->set_value = bb_attribute_set_value_missing;

    g_object_interface_install_property(
        iface,
        g_param_spec_string(
            "name",
            "",
            "",
            NULL,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_string(
            "value",
            "",
            "",
            NULL,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );
}


gchar*
bb_attribute_get_name(BbAttribute *attribute)
{
    g_return_val_if_fail(attribute != NULL, NULL);

    BbAttributeInterface *iface = BB_ATTRIBUTE_GET_IFACE(attribute);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->get_name != NULL, NULL);

    return iface->get_name(attribute);
}


static gchar*
bb_attribute_get_name_missing(BbAttribute *attribute)
{
    g_error("bb_attribute_get_name() not overridden");
}


gchar*
bb_attribute_get_value(BbAttribute *attribute)
{
    g_return_val_if_fail(attribute != NULL, NULL);

    BbAttributeInterface *iface = BB_ATTRIBUTE_GET_IFACE(attribute);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->get_value != NULL, NULL);

    return iface->get_value(attribute);
}


static gchar*
bb_attribute_get_value_missing(BbAttribute *attribute)
{
    g_error("bb_attribute_get_value() not overridden");
}


void
bb_attribute_set_name(BbAttribute *attribute, const gchar *name)
{
    g_return_if_fail(attribute != NULL);
    g_return_if_fail(name != NULL);

    BbAttributeInterface *iface = BB_ATTRIBUTE_GET_IFACE(attribute);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->get_name != NULL);

    iface->get_name(attribute);
}


static void
bb_attribute_set_name_missing(BbAttribute *attribute, const gchar *name)
{
    g_error("bb_attribute_get_name() not overridden");
}


void
bb_attribute_set_value(BbAttribute *attribute, const gchar *value)
{
    g_return_if_fail(attribute != NULL);
    g_return_if_fail(value != NULL);

    BbAttributeInterface *iface = BB_ATTRIBUTE_GET_IFACE(attribute);

    g_return_if_fail(iface != NULL);
    g_return_if_fail(iface->set_value != NULL);

    iface->set_value(attribute, value);
}


static void
bb_attribute_set_value_missing(BbAttribute *attribute, const gchar *name)
{
    g_error("bb_attribute_set_value() not overridden");
}
