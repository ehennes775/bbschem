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
#include <bbextensions.h>
#include "bbtoolpalette.h"


enum
{
    PROP_0,
    PROP_MAIN_WINDOW,
    N_PROPERTIES
};


typedef struct _Actions Actions;

struct _Actions
{
    GSimpleAction *select_tool;
};

#define ACTION_COUNT (sizeof(Actions)/sizeof(gpointer))


struct _BbToolPalette
{
    GtkBox parent;

    BbMainWindow *main_window;

    union
    {
        Actions action;
        GAction *action_array[ACTION_COUNT];
    };
};


G_DEFINE_TYPE(BbToolPalette, bb_tool_palette, GTK_TYPE_BOX);


static void
bb_tool_palette_attach_actions(BbToolPalette *palette, GActionMap *map);

static void
bb_tool_palette_detach_actions(BbToolPalette *palette, GActionMap *map);

static void
bb_tool_palette_dispose(GObject *object);

static void
bb_tool_palette_finalize(GObject *object);

static void
bb_tool_palette_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_tool_palette_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static GParamSpec *properties[N_PROPERTIES];


static void
bb_tool_palette_attach_actions(BbToolPalette *palette, GActionMap *map)
{
    g_return_if_fail(G_IS_ACTION_MAP(map));
    g_return_if_fail(BB_IS_TOOL_PALETTE(palette));

    for (int index = 0; index < ACTION_COUNT; ++index)
    {
        g_action_map_add_action(map, palette->action_array[index]);
    }
}


static void
bb_tool_palette_class_init(BbToolPaletteClass *class)
{
    g_return_if_fail(G_OBJECT_CLASS(class) != NULL);

    G_OBJECT_CLASS(class)->dispose = bb_tool_palette_dispose;
    G_OBJECT_CLASS(class)->finalize = bb_tool_palette_finalize;
    G_OBJECT_CLASS(class)->get_property = bb_tool_palette_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_tool_palette_set_property;

    properties[PROP_MAIN_WINDOW] = g_param_spec_object(
        "main-window",
        "",
        "",
        BB_TYPE_MAIN_WINDOW,
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

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(class),
        "/com/github/ehennes775/bbsch/gui/bbtoolpalette.ui"
        );
}


static void
bb_tool_palette_detach_actions(BbToolPalette *palette, GActionMap *map)
{
    g_return_if_fail(G_IS_ACTION_MAP(map));
    g_return_if_fail(BB_IS_TOOL_PALETTE(palette));

    for (int index = 0; index < ACTION_COUNT; ++index)
    {
        g_action_map_remove_action(map, g_action_get_name(palette->action_array[index]));
    }
}


static void
bb_tool_palette_dispose(GObject *object)
{
    BbToolPalette *palette = BB_TOOL_PALETTE(object);
    g_return_if_fail(palette != NULL);

    g_clear_object(&palette->main_window);
}


static void
bb_tool_palette_finalize(GObject *object)
{
}


BbMainWindow*
bb_tool_palette_get_main_window(BbToolPalette *palette)
{
    g_return_val_if_fail(palette != NULL, NULL);

    return palette->main_window;
}


static void
bb_tool_palette_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_MAIN_WINDOW:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_tool_palette_init(BbToolPalette *palette)
{
    gtk_widget_init_template(GTK_WIDGET(palette));

    palette->action.select_tool = g_simple_action_new_stateful(
        "select-tool",
        NULL,
        g_variant_new_string("hello")
        );
}


__attribute__((constructor)) void
bb_tool_palette_register()
{
    bb_tool_palette_get_type();
}


void
bb_tool_palette_set_main_window(BbToolPalette *palette, BbMainWindow *window)
{
    g_return_if_fail(palette != NULL);

    if (palette->main_window != window)
    {
        if (palette->main_window != NULL)
        {
            bb_tool_palette_detach_actions(palette, G_ACTION_MAP(window));
        }

        g_set_object(&palette->main_window, window);

        if (palette->main_window != NULL)
        {
            bb_tool_palette_attach_actions(palette, G_ACTION_MAP(window));
        }

        g_object_notify_by_pspec(G_OBJECT(palette), properties[PROP_MAIN_WINDOW]);
    }
}


static void
bb_tool_palette_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_MAIN_WINDOW:
            bb_tool_palette_set_main_window(BB_TOOL_PALETTE(object), BB_MAIN_WINDOW(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
