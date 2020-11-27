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
#include "bbgraphics.h"


enum {
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


struct _BbGraphics {
    GObject parent;
};


G_DEFINE_TYPE(BbGraphics, bb_graphics, G_OBJECT);


static void
bb_graphics_dispose(GObject *object);

static void
bb_graphics_finalize(GObject *object);

static void
bb_graphics_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_graphics_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


static void
bb_graphics_class_init(BbGraphicsClass *klasse) {
    G_OBJECT_CLASS(klasse)->dispose = bb_graphics_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_graphics_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_graphics_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_graphics_set_property;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/bbgraphics.ui"
    );
}


static void
bb_graphics_dispose(GObject *object) {
    // BbGraphics* privat = BBGRAPHICS_GET_PRIVATE(object);
}


static void
bb_graphics_finalize(GObject *object) {
    // BbGraphics* privat = BBGRAPHICS_GET_PRIVATE(object);
}


static void
bb_graphics_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec) {
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
bb_graphics_init(BbGraphics *window) {
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_graphics_register() {
    bb_graphics_get_type();
}


static void
bb_graphics_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec) {
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
