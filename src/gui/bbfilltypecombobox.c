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
#include "bbcellrenderercolorswatch.h"
#include "bbfilltypecombobox.h"


enum
{
    PROP_0,
    PROP_FILL_TYPE
};


struct _BbFillTypeComboBox
{
    BbPropertyComboBox parent;

    gulong apply_handler_id;
    gulong update_handler_id;
};

G_DEFINE_TYPE(BbFillTypeComboBox, bb_fill_type_combo_box, BB_TYPE_PROPERTY_COMBO_BOX);


static void
bb_fill_type_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_fill_type_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_fill_type_combo_box_update_cb(BbPropertyComboBox *unused, GVariant *value, BbFillTypeComboBox *combo);


static void
bb_fill_type_combo_box_class_init(BbFillTypeComboBoxClass *klasse)
{
    g_return_if_fail(G_OBJECT_CLASS(klasse) != NULL);

    G_OBJECT_CLASS(klasse)->get_property = bb_fill_type_combo_box_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_fill_type_combo_box_set_property;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/bbfilltypecombobox.ui"
        );
}


BbFillType
bb_fill_type_combo_box_get_fill_type(BbFillTypeComboBox *combo)
{
    g_return_val_if_fail(BB_IS_FILL_TYPE_COMBO_BOX(combo), BB_FILL_TYPE_HOLLOW);

    BbFillType fill_type = gtk_combo_box_get_active(GTK_COMBO_BOX(combo));

    g_return_val_if_fail(bb_fill_type_is_valid(fill_type), BB_FILL_TYPE_HOLLOW);

    return fill_type;
}


static void
bb_fill_type_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_FILL_TYPE:
            g_value_set_int(value, bb_fill_type_combo_box_get_fill_type(BB_FILL_TYPE_COMBO_BOX(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_fill_type_combo_box_init(BbFillTypeComboBox *combo)
{
    gtk_widget_init_template(GTK_WIDGET(combo));

    combo->update_handler_id = g_signal_connect(
        combo,
        "update",
        G_CALLBACK(bb_fill_type_combo_box_update_cb),
        combo
        );
}


void
bb_fill_type_combo_box_set_fill_type(BbFillTypeComboBox *combo, BbFillType fill_type)
{
    g_return_if_fail(BB_IS_FILL_TYPE_COMBO_BOX(combo));
    g_return_if_fail(bb_fill_type_is_valid(fill_type));

    gtk_combo_box_set_active(GTK_COMBO_BOX(combo), fill_type);
}


static void
bb_fill_type_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_FILL_TYPE:
            bb_fill_type_combo_box_set_fill_type(BB_FILL_TYPE_COMBO_BOX(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_fill_type_combo_box_update_cb(BbPropertyComboBox *property_combo, GVariant *value, BbFillTypeComboBox *fill_type_combo)
{
    g_return_if_fail(BB_IS_FILL_TYPE_COMBO_BOX(fill_type_combo));
    g_return_if_fail(BB_IS_PROPERTY_COMBO_BOX(property_combo));
    g_return_if_fail(g_variant_is_of_type(value, G_VARIANT_TYPE_INT32));

    g_signal_handler_block(
        property_combo,
        fill_type_combo->apply_handler_id
        );

    bb_fill_type_combo_box_set_fill_type(fill_type_combo, g_variant_get_int32(value));

    g_signal_handler_unblock(
        property_combo,
        fill_type_combo->apply_handler_id
        );
}
