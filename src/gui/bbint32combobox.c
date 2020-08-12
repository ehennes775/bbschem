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
#include <errno.h>
#include "bbint32combobox.h"


enum
{
    PROP_0,
    PROP_VALUE
};


struct _BbInt32ComboBox
{
    BbPropertyComboBox parent;

    GtkEntry *entry;

    gulong apply_handler_id;
    gulong update_handler_id;
};

G_DEFINE_TYPE(BbInt32ComboBox, bb_int32_combo_box, BB_TYPE_PROPERTY_COMBO_BOX);


static void
bb_int32_combo_box_apply_cb(BbPropertyComboBox *unused, BbInt32ComboBox *combo);

static void
bb_int32_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_int32_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_int32_combo_box_update_cb(BbPropertyComboBox *unused, GVariant *value, BbInt32ComboBox *combo);


static void
bb_int32_combo_box_apply_cb(BbPropertyComboBox *property_combo, BbInt32ComboBox *int32_combo)
{
    g_return_if_fail(BB_IS_INT32_COMBO_BOX(int32_combo));
    g_return_if_fail(BB_IS_PROPERTY_COMBO_BOX(property_combo));

    GActionGroup *group = bb_property_combo_box_get_action_group(property_combo);

    if (group != NULL)
    {
        GError *error = NULL;
        int value = bb_int32_combo_box_parse_value(int32_combo, &error);

        if (error == NULL)
        {
            g_action_group_activate_action(
                group,
                bb_property_combo_box_get_action_name(property_combo),
                g_variant_new_int32(value)
                );
        }

        g_error_free(error);
    }
}


static void
bb_int32_combo_box_class_init(BbInt32ComboBoxClass *klasse)
{
    g_return_if_fail(G_OBJECT_CLASS(klasse) != NULL);

    G_OBJECT_CLASS(klasse)->get_property = bb_int32_combo_box_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_int32_combo_box_set_property;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/bbint32combobox.ui"
        );

    //gtk_widget_class_bind_template_child(
    //    GTK_WIDGET_CLASS(klasse),
    //    BbInt32ComboBox,
    //    entry
    //    );
}


int
bb_int32_combo_box_get_value(BbInt32ComboBox *combo)
{
    GError *error = NULL;
    int value = bb_int32_combo_box_parse_value(combo, &error);

    if (error != NULL)
    {
        g_error_free(error);
        value = -1;
    }

    return value;
}


static void
bb_int32_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_VALUE:
            g_value_set_int(value, bb_int32_combo_box_get_value(BB_INT32_COMBO_BOX(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_int32_combo_box_init(BbInt32ComboBox *combo)
{
    gtk_widget_init_template(GTK_WIDGET(combo));

    combo->apply_handler_id = g_signal_connect(
        combo,
        "apply",
        G_CALLBACK(bb_int32_combo_box_apply_cb),
        combo
    );

    combo->update_handler_id = g_signal_connect(
        combo,
        "update",
        G_CALLBACK(bb_int32_combo_box_update_cb),
        combo
    );
}


int
bb_int32_combo_box_parse_value(BbInt32ComboBox *combo, GError **error)
{
    GError *temp_error = NULL;
    GtkWidget *entry = GTK_WIDGET(gtk_bin_get_child(GTK_BIN(combo)));
    int value = -1;
    const char *text0 = gtk_entry_get_text(GTK_ENTRY(entry));

    if (text0 != NULL)
    {
        long temp;
        char *text1;

        errno = 0;

        temp = strtol (text0, &text1, 0);

        if ((errno == 0) && (text1 != NULL) && (*text1 == '\0') && (temp >= 0))
        {
            value = temp;
        }
        else
        {
            temp_error = g_error_new(
                g_quark_from_string("bbsch"),
                0,
                "TODO"
                );
        }
    }
    else
    {
        temp_error = g_error_new(
            g_quark_from_string("bbsch"),
            0,
            "TODO"
            );
    }

    if (temp_error != NULL)
    {
        g_propagate_error (error, temp_error);
    }

    return value;
}


__attribute__((constructor)) void
bb_int32_combo_box_register()
{
    bb_int32_combo_box_get_type();
}


void
bb_int32_combo_box_set_value(BbInt32ComboBox *combo, int value)
{
    g_message("bb_int32_combo_box_set_value (value = %d)", value);
}


static void
bb_int32_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_VALUE:
            bb_int32_combo_box_set_value(BB_INT32_COMBO_BOX(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_int32_combo_box_update_cb(BbPropertyComboBox *property_combo, GVariant *value, BbInt32ComboBox *int32_combo)
{
    g_return_if_fail(BB_IS_INT32_COMBO_BOX(int32_combo));
    g_return_if_fail(BB_IS_PROPERTY_COMBO_BOX(property_combo));
    g_return_if_fail(g_variant_is_of_type(value, G_VARIANT_TYPE_INT32));

    g_signal_handler_block(
        property_combo,
        int32_combo->apply_handler_id
        );

    bb_int32_combo_box_set_value(int32_combo, g_variant_get_int32(value));

    g_signal_handler_unblock(
        property_combo,
        int32_combo->apply_handler_id
        );
}
