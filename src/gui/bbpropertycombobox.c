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
#include "bbpropertycombobox.h"


struct _BbPropertyComboBox
{
    GtkComboBox parent;

    gboolean changed;
};


G_DEFINE_TYPE(BbPropertyComboBox, bb_property_combo_box, GTK_TYPE_COMBO_BOX);


static void
bb_property_combo_box_add_widget(BbPropertyComboBox *combo, GtkWidget *widget, gpointer unused);

static gboolean
bb_property_combo_box_focus_out_event(GtkEntry *entry, GdkEvent *event, BbPropertyComboBox *combo);

static void
bb_property_combo_box_notify_active(BbPropertyComboBox *combo, GParamSpec *pspec, gpointer unused);

static void
bb_property_combo_box_remove_widget(BbPropertyComboBox *combo, GtkWidget *widget, gpointer unused);

static void
bb_property_combo_box_value_changed(BbPropertyComboBox *combo, gpointer unused);


static void
bb_property_combo_box_add_widget(BbPropertyComboBox *combo, GtkWidget *widget, gpointer unused)
{
    g_signal_connect(
        G_OBJECT(widget),
        "focus-out-event",
        G_CALLBACK(bb_property_combo_box_focus_out_event),
        combo
        );
}


static gboolean
bb_property_combo_box_focus_out_event(GtkEntry *entry, GdkEvent *event, BbPropertyComboBox *combo)
{
    g_return_val_if_fail(combo != NULL, FALSE);

    if (combo->changed)
    {
        g_signal_emit_by_name (combo, "apply");
        combo->changed = FALSE;
    }

    return FALSE;
}


static void
bb_property_combo_box_class_init(BbPropertyComboBoxClass *class)
{
    g_signal_new(
        "apply",
        G_OBJECT_CLASS_TYPE(class),
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
bb_property_combo_box_init(BbPropertyComboBox *combo)
{
    g_signal_connect(
        G_OBJECT(combo),
        "add",
        G_CALLBACK(bb_property_combo_box_add_widget),
        NULL
        );

    g_signal_connect(
        G_OBJECT(combo),
        "notify::active",
        G_CALLBACK(bb_property_combo_box_notify_active),
        NULL
        );

    g_signal_connect(
        G_OBJECT(combo),
        "remove",
        G_CALLBACK(bb_property_combo_box_remove_widget),
        NULL
        );

    g_signal_connect(
        G_OBJECT(combo),
        "changed",
        G_CALLBACK(bb_property_combo_box_value_changed),
        NULL
        );
}


static void
bb_property_combo_box_notify_active(BbPropertyComboBox *combo, GParamSpec *pspec, gpointer unused)
{
    g_return_if_fail(combo != NULL);

    g_signal_emit_by_name (combo, "apply");
    combo->changed = FALSE;
}


__attribute__((constructor)) void
bb_property_combo_box_register()
{
    bb_property_combo_box_get_type();
}


static void
bb_property_combo_box_remove_widget(BbPropertyComboBox *combo, GtkWidget *widget, gpointer unused)
{
    g_signal_handlers_disconnect_by_func(
        G_OBJECT(widget),
        (gpointer) bb_property_combo_box_focus_out_event,
        combo
        );
}


static void
bb_property_combo_box_value_changed(BbPropertyComboBox *combo, gpointer unused)
{
    //if (gtk_widget_is_focus(GTK_WIDGET(gschem_integer_combo_box_get_entry (GTK_WIDGET (combo)))))
    //{
    //    combo->changed = TRUE;
    //}
    //else
    {
        g_signal_emit_by_name(combo, "apply");
        combo->changed = FALSE;
    }
}
