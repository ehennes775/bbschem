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
#include "bbdocumentwindow.h"


enum
{
    PROP_0,
    PROP_CAIRO,
    PROP_TAB,
    N_PROPERTIES
};

typedef struct _BbDocumentWindowPrivate BbDocumentWindowPrivate;

struct _BbDocumentWindowPrivate
{
    int dummy;
};

G_DEFINE_TYPE_WITH_PRIVATE(BbDocumentWindow, bb_document_window, GTK_TYPE_BOX);


static void
bb_document_window_dispose(GObject *object);

static void
bb_document_window_finalize(GObject *object);

static void
bb_document_window_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static const char*
bb_document_window_get_tab_missing(BbDocumentWindow *window);

static void
bb_document_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static GParamSpec *properties[N_PROPERTIES];


static void
bb_document_window_class_init(BbDocumentWindowClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);

    G_OBJECT_CLASS(klasse)->dispose = bb_document_window_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_document_window_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_document_window_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_document_window_set_property;

    klasse->get_tab = bb_document_window_get_tab_missing;

    properties[PROP_TAB] = bb_object_class_install_property(
        object_class,
        PROP_TAB,
        g_param_spec_string(
            "tab",
            "",
            "",
            "None",
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_signal_new(
        "update",
        G_OBJECT_CLASS_TYPE(klasse),
        (GSignalFlags) 0,
        0,
        NULL,
        NULL,
        g_cclosure_marshal_VOID__VOID,
        G_TYPE_NONE,
        0
    );

}


static void
bb_document_window_dispose(GObject *object)
{
    //BbDocumentWindow* privat = BB_DOCUMENT_WINDOW_GET_PRIVATE(object);
}


static void
bb_document_window_finalize(GObject *object)
{
    //BbDocumentWindow* privat = BB_DOCUMENT_WINDOW_GET_PRIVATE(object);
}


static void
bb_document_window_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
            break;

        case PROP_TAB:
            g_value_set_string(value, bb_document_window_get_tab(BB_DOCUMENT_WINDOW(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


const char*
bb_document_window_get_tab(BbDocumentWindow *window)
{
    g_return_val_if_fail(BB_IS_DOCUMENT_WINDOW(window), "None");

    BbDocumentWindowClass *klasse = BB_DOCUMENT_WINDOW_GET_CLASS(window);

    g_return_val_if_fail(klasse != NULL, "None");
    g_return_val_if_fail(klasse->get_tab != NULL, "None");

    return klasse->get_tab(window);
}


static const char*
bb_document_window_get_tab_missing(BbDocumentWindow *window)
{
    g_warn_if_reached();

    return "None";
}


static void
bb_document_window_init(BbDocumentWindow *window)
{
}


static void
bb_document_window_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
