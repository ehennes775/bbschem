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
#include "bbschematicwindow.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3
};


struct _BbSchematicWindow
{
    BbDocumentWindow parent;
};


G_DEFINE_TYPE(BbSchematicWindow, bb_schematic_window, BB_TYPE_DOCUMENT_WINDOW);


static void
bb_schematic_window_dispose(GObject *object);

static void
bb_schematic_window_finalize(GObject *object);

static void
bb_schematic_window_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_schematic_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


void
bb_schematic_window_apply_property(BbSchematicWindow *window, const char *name)
{
    g_message("apply-property = \"%s\"", name);

    g_signal_emit_by_name(window, "update");
}


static void
bb_schematic_window_class_init(BbSchematicWindowClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_schematic_window_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_schematic_window_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_schematic_window_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_schematic_window_set_property;

    //gtk_widget_class_set_template_from_resource(
    //    GTK_WIDGET_CLASS(klasse),
    //    "/com/github/ehennes775/bbsch/gui/bbschematicwindow.ui"
    //    );
}


static void
bb_schematic_window_dispose(GObject *object)
{
    //BbSchematicWindow* privat = BBSCHEMATIC_WINDOW_GET_PRIVATE(object);
}


static void
bb_schematic_window_finalize(GObject *object)
{
    //BbSchematicWindow* privat = BBSCHEMATIC_WINDOW_GET_PRIVATE(object);
}


static void
bb_schematic_window_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_1:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_schematic_window_init(BbSchematicWindow *window)
{
    //gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_schematic_window_register()
{
    bb_schematic_window_get_type();
}


static void
bb_schematic_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_1:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
