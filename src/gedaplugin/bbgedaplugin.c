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

#include <libpeas/peas.h>
#include <bbextensions.h>
#include <bbgeneralopener.h>
#include "bbgedaplugin.h"
#include "bbgedaopener.h"


enum
{
    PROP_0,

    /* From PeasActivatable */
    PROP_OBJECT,

    N_PROPERTIES
};


struct _BbGedaPlugin
{
    PeasExtensionBase parent;

    /**
     * @brief
     */
    GObject *object;

    BbGedaOpener *opener;
};


static void
bb_geda_plugin_dispose(GObject *object);

static void
bb_geda_plugin_finalize(GObject *object);

static void
bb_geda_plugin_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_plugin_peas_activatable_init(PeasActivatableInterface *iface);

static void
bb_geda_plugin_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_DYNAMIC_TYPE_EXTENDED(
    BbGedaPlugin,
    bb_geda_plugin,
    PEAS_TYPE_EXTENSION_BASE,
    0,
    G_IMPLEMENT_INTERFACE_DYNAMIC(PEAS_TYPE_ACTIVATABLE, bb_geda_plugin_peas_activatable_init)
    )


// region From PeasActivatable Interface

static void
bb_geda_plugin_activate(PeasActivatable *activatable)
{
    BbGedaPlugin *plugin = BB_GEDA_PLUGIN(activatable);
    g_return_if_fail(BB_IS_GEDA_PLUGIN(plugin));

    BbGeneralOpener *general_opener = bb_main_window_get_opener(BB_MAIN_WINDOW(plugin->object));
    plugin->opener = bb_geda_opener_new(BB_MAIN_WINDOW(plugin->object));

    bb_general_opener_add_specific_opener(
        general_opener,
        "application/x-geda-schematic",
        BB_SPECIFIC_OPENER(plugin->opener)
        );

    bb_general_opener_add_specific_opener(
        general_opener,
        "application/x-geda-symbol",
        BB_SPECIFIC_OPENER(plugin->opener)
        );

    bb_general_opener_add_specific_opener(
        general_opener,
        "application/x-lepton-schematic",
        BB_SPECIFIC_OPENER(plugin->opener)
        );

    bb_general_opener_add_specific_opener(
        general_opener,
        "application/x-lepton-symbol",
        BB_SPECIFIC_OPENER(plugin->opener)
        );
}


static GObject*
bb_geda_plugin_get_object(BbGedaPlugin *plugin)
{
    g_return_val_if_fail(BB_IS_GEDA_PLUGIN(plugin), NULL);

    return plugin->object;
}


static void
bb_geda_plugin_deactivate(PeasActivatable *activatable)
{
    BbGedaPlugin *plugin = BB_GEDA_PLUGIN(activatable);
    g_return_if_fail(BB_IS_GEDA_PLUGIN(plugin));

    // TODO Remove openers
}

void
bb_geda_plugin_register(GTypeModule *type_module)
{
    bb_geda_plugin_register_type(type_module);
}


static void
bb_geda_plugin_set_object(BbGedaPlugin *plugin, GObject *object)
{
    g_return_if_fail(BB_IS_GEDA_PLUGIN(plugin));
    g_return_if_fail(G_IS_OBJECT(object));

    if (plugin->object != object)
    {
        g_clear_object(&plugin->object);

        plugin->object = g_object_ref(object);
    }
}


static void
bb_geda_plugin_update_state(PeasActivatable *activatable)
{
    g_message("%s", __func__);
}


static void
bb_geda_plugin_peas_activatable_init(PeasActivatableInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->activate = bb_geda_plugin_activate;
    iface->deactivate = bb_geda_plugin_deactivate;
    iface->update_state = bb_geda_plugin_update_state;
}

// endregion

static void
bb_geda_plugin_class_init(BbGedaPluginClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);

    object_class->dispose = bb_geda_plugin_dispose;
    object_class->finalize = bb_geda_plugin_finalize;
    object_class->get_property = bb_geda_plugin_get_property;
    object_class->set_property = bb_geda_plugin_set_property;

    properties[PROP_OBJECT] = bb_object_class_override_property(
        object_class,
        PROP_OBJECT,
        "object"
        );
}


static void
bb_geda_plugin_class_finalize(BbGedaPluginClass *klass)
{
}


static void
bb_geda_plugin_dispose(GObject *object)
{
    G_OBJECT_CLASS(bb_geda_plugin_parent_class)->dispose(object);
}


static void
bb_geda_plugin_finalize(GObject *object)
{
    G_OBJECT_CLASS(bb_geda_plugin_parent_class)->finalize(object);
}


static void
bb_geda_plugin_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_OBJECT:
            g_value_set_object(value, bb_geda_plugin_get_object(BB_GEDA_PLUGIN(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_geda_plugin_init(BbGedaPlugin *activatable)
{
}


static void
bb_geda_plugin_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_OBJECT:
            bb_geda_plugin_set_object(BB_GEDA_PLUGIN(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
