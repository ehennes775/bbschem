/*
 * bbschem
 * Copyright (C) 2021 Edward C. Hennessy
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
#include "bbalignmentcombobox.h"


enum
{
    PROP_0,

    PROP_ALIGNMENT,

    N_PROPERTIES
};


struct _BbAlignmentComboBox
{
    BbPropertyComboBox parent;

    gulong apply_handler_id;
    gulong update_handler_id;
};


static void
bb_alignment_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_alignment_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_alignment_combo_box_update_cb(BbPropertyComboBox *unused, GVariant *value, BbAlignmentComboBox *combo);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE(BbAlignmentComboBox, bb_alignment_combo_box, BB_TYPE_PROPERTY_COMBO_BOX);


static void
bb_alignment_combo_box_class_init(BbAlignmentComboBoxClass *klasse)
{
    g_return_if_fail(G_IS_OBJECT_CLASS(klasse));

    G_OBJECT_CLASS(klasse)->get_property = bb_alignment_combo_box_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_alignment_combo_box_set_property;

    BB_TYPE_TEXT_ALIGNMENT;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/bbalignmentcombobox.ui"
        );

    properties[PROP_ALIGNMENT] = bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_ALIGNMENT,
        g_param_spec_enum(
            "alignment",
            "",
            "",
            BB_TYPE_TEXT_ALIGNMENT,
            BB_TEXT_ALIGNMENT_DEFAULT,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );
}


BbTextAlignment
bb_alignment_combo_box_get_alignment(BbAlignmentComboBox *combo)
{
    g_return_val_if_fail(BB_IS_ALIGNMENT_COMBO_BOX(combo), BB_TEXT_ALIGNMENT_DEFAULT);

    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo));
    g_return_val_if_fail(GTK_IS_TREE_MODEL(model), BB_TEXT_ALIGNMENT_DEFAULT);

    BbTextAlignment alignment = BB_TEXT_ALIGNMENT_DEFAULT;
    GtkTreeIter iter;

    gboolean success = gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo), &iter);

    if (success)
    {
        GValue value = G_VALUE_INIT;

        gtk_tree_model_get_value(model, &iter, 1, &value);

        BbTextAlignment temp_alignment = g_value_get_enum(&value);

        if (bb_text_alignment_is_valid(temp_alignment))
        {
            alignment = temp_alignment;
        }

        g_value_reset(&value);
    }

    return alignment;
}


static void
bb_alignment_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_alignment_combo_box_init(BbAlignmentComboBox *combo)
{
    gtk_widget_init_template(GTK_WIDGET(combo));

    gtk_combo_box_set_wrap_width(GTK_COMBO_BOX(combo), 3);

    combo->update_handler_id = g_signal_connect(
        combo,
        "update",
        G_CALLBACK(bb_alignment_combo_box_update_cb),
        combo
        );
}


__attribute__((constructor)) void
bb_alignment_combo_box_register()
{
    bb_alignment_combo_box_get_type();
}


void
bb_alignment_combo_box_set_alignment(BbAlignmentComboBox *combo, BbTextAlignment alignment)
{
    g_return_if_fail(BB_IS_ALIGNMENT_COMBO_BOX(combo));
    g_return_if_fail(bb_text_alignment_is_valid(alignment));

    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo));
    g_return_if_fail(GTK_IS_TREE_MODEL(model));

    GtkTreeIter iter;

    gboolean success = gtk_tree_model_get_iter_first(model, &iter);

    while (success)
    {
        GValue value = G_VALUE_INIT;

        gtk_tree_model_get_value(model, &iter, 1, &value);

        BbTextAlignment temp_alignment = g_value_get_enum(&value);

        g_value_unset(&value);

        if (alignment == temp_alignment)
        {
            gtk_combo_box_set_active_iter(GTK_COMBO_BOX(combo), &iter);

            break;
        }

        success = gtk_tree_model_iter_next(model, &iter);
    }
}


static void
bb_alignment_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ALIGNMENT:
            bb_alignment_combo_box_set_alignment(BB_ALIGNMENT_COMBO_BOX(object), g_value_get_enum(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_alignment_combo_box_update_cb(BbPropertyComboBox *property_combo, GVariant *value, BbAlignmentComboBox *alignment_combo)
{
    g_return_if_fail(BB_IS_ALIGNMENT_COMBO_BOX(alignment_combo));
    g_return_if_fail(BB_IS_PROPERTY_COMBO_BOX(property_combo));
    g_return_if_fail(g_variant_is_of_type(value, G_VARIANT_TYPE_INT32));

    g_signal_handler_block(
        property_combo,
        alignment_combo->apply_handler_id
        );

    bb_alignment_combo_box_set_alignment(alignment_combo, g_variant_get_int32(value));

    g_signal_handler_unblock(
        property_combo,
        alignment_combo->apply_handler_id
        );
}
