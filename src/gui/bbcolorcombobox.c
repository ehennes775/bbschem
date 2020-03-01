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
#include "bbcolorcombobox.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3,
};


struct _BbColorComboBox
{
    BbPropertyComboBox parent;
};


G_DEFINE_TYPE(BbColorComboBox, bb_color_combo_box, BB_TYPE_PROPERTY_COMBO_BOX);


static void
bb_color_combo_box_dispose(GObject *object);

static void
bb_color_combo_box_finalize(GObject *object);

static void
bb_color_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_color_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static void
bb_color_combo_box_apply(BbPropertyComboBox *unused, BbColorComboBox *combo)
{
    GActionGroup *group = bb_property_combo_box_get_action_group(BB_PROPERTY_COMBO_BOX(combo));

    if (group != NULL)
    {
        GVariant *color = g_variant_new_int32(
            bb_color_combo_box_get_color(combo)
            );

        g_action_group_activate_action(
            group,
            bb_property_combo_box_get_action_name(BB_PROPERTY_COMBO_BOX(combo)),
            color
            );
    }
}


static void
bb_color_combo_box_class_init(BbColorComboBoxClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_color_combo_box_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_color_combo_box_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_color_combo_box_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_color_combo_box_set_property;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/bbcolorcombobox.ui"
        );

    gtk_widget_class_bind_template_callback(
        GTK_WIDGET_CLASS(klasse),
        bb_color_combo_box_apply
        );
}


static void
bb_color_combo_box_dispose(GObject *object)
{
    // BbColorComboBox* privat = BBCOLOR_COMBO_BOX_GET_PRIVATE(object);
}


static void
bb_color_combo_box_finalize(GObject *object)
{
    // BbColorComboBox* privat = BBCOLOR_COMBO_BOX_GET_PRIVATE(object);
}


int
bb_color_combo_box_get_color(BbColorComboBox *combo)
{
    int color = 0;
    GtkTreeIter iter;

    gboolean success = gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo), &iter);
    GtkTreeModel *model = gtk_combo_box_get_model(GTK_COMBO_BOX(combo));

    if (success && (model != NULL))
    {
        GValue value = G_VALUE_INIT;

        gtk_tree_model_get_value(model, &iter, 3, &value);

        color = g_value_get_int(&value);
    }

    return color;
}


static void
bb_color_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
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
bb_color_combo_box_init(BbColorComboBox *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
bb_color_combo_box_register()
{
    bb_color_combo_box_get_type();
}


static void
bb_color_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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
