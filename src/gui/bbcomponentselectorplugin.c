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
#include "bbcomponentselectorplugin.h"
#include "bbextensions.h"


enum
{
    PROP_0,
    PROP_OBJECT,
    N_PROPERTIES
};


struct _BbComponentSelectorPlugin
{
    PeasExtensionBase parent;

    GObject *object;
};


static void
bb_component_selector_plugin_activate(PeasActivatable *activatable);

static void
bb_component_selector_plugin_activatable_init(PeasActivatableInterface *iface);

static void
bb_component_selector_plugin_deactivate(PeasActivatable *activatable);

static void
bb_component_selector_plugin_dispose(GObject *object);

static void
bb_component_selector_plugin_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_component_selector_plugin_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


G_DEFINE_TYPE_EXTENDED(
    BbComponentSelectorPlugin,
    bb_component_selector_plugin,
    PEAS_TYPE_EXTENSION_BASE,
    0,
    G_IMPLEMENT_INTERFACE(PEAS_TYPE_ACTIVATABLE, bb_component_selector_plugin_activatable_init)
    )


static GParamSpec *properties[N_PROPERTIES];


static void
bb_component_selector_plugin_activate(PeasActivatable *activatable)
{
    g_return_if_fail(activatable != NULL);
}


static void
bb_component_selector_plugin_activatable_init(PeasActivatableInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->activate = bb_component_selector_plugin_activate;
    iface->deactivate = bb_component_selector_plugin_deactivate;
}


static void
bb_component_selector_plugin_class_init(BbComponentSelectorPluginClass *class)
{
    g_return_if_fail(G_OBJECT_CLASS(class) != NULL);

    G_OBJECT_CLASS(class)->dispose = bb_component_selector_plugin_dispose;
    G_OBJECT_CLASS(class)->get_property = bb_component_selector_plugin_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_component_selector_plugin_set_property;

    properties[PROP_OBJECT] = g_param_spec_object(
        "object",
        "",
        "",
        G_TYPE_OBJECT,
        G_PARAM_READWRITE
        );

    for (int index = PROP_0 + 1; index < N_PROPERTIES; ++index)
    {
        bb_object_class_install_property(
            G_OBJECT_CLASS(class),
            index,
            properties[index]
            );
    }
}


static void
bb_component_selector_plugin_deactivate(PeasActivatable *activatable)
{
    g_return_if_fail(activatable != NULL);
}


static void
bb_component_selector_plugin_dispose(GObject *object)
{
    g_return_if_fail(object != NULL);
}


GObject*
bb_component_selector_plugin_get_object(BbComponentSelectorPlugin *plugin)
{
    g_return_val_if_fail(plugin != NULL, NULL);

    return plugin->object;
}


static void
bb_component_selector_plugin_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_OBJECT:
            g_value_set_object(value, bb_component_selector_plugin_get_object(BB_COMPONENT_SELECTOR_PLUGIN(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_component_selector_plugin_init(BbComponentSelectorPlugin *plugin)
{
    g_return_if_fail(plugin != NULL);
}


__attribute__((constructor)) void
bb_component_selector_plugin_register()
{
}


void
bb_component_selector_plugin_set_object(BbComponentSelectorPlugin *plugin, GObject *object)
{
    g_return_if_fail(plugin != NULL);

    if (plugin->object != object)
    {
        g_set_object(&plugin->object, object);

        g_object_notify_by_pspec(G_OBJECT(plugin), properties[PROP_OBJECT]);
    }
}


static void
bb_component_selector_plugin_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_OBJECT:
            bb_component_selector_plugin_set_object(BB_COMPONENT_SELECTOR_PLUGIN(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
