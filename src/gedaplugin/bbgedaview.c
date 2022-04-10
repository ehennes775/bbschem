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
#include "bbgedaview.h"


enum
{
    PROP_0,
    PROP_CAIRO,
    PROP_2,
    PROP_3
};


struct _BbGedaView
{
    GtkDrawingArea parent;
};


static void
bb_geda_view_dispose(GObject *object);

static void
bb_geda_view_finalize(GObject *object);

static void
bb_geda_view_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_view_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


G_DEFINE_TYPE_EXTENDED(
    BbGedaView,
    bb_geda_view,
    GTK_TYPE_DRAWING_AREA,
    0,

    )


static void
bb_geda_view_class_init(BbGedaViewClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_geda_view_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_geda_view_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_geda_view_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_geda_view_set_property;
}


static void
bb_geda_view_class_finalize(BbGedaViewClass *klasse)
{
}


static void
bb_geda_view_dispose(GObject *object)
{
}


static void
bb_geda_view_finalize(GObject *object)
{
}


static void
bb_geda_view_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
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
bb_geda_view_init(BbGedaView *window)
{
}


//void
//bb_geda_view_register(GTypeModule *module)
//{
//    bb_geda_view_register_type(module);
//}


static void
bb_geda_view_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
