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
#include "bbpropertycombobox.h"

enum
{
    PROP_0,
    PROP_ACTION_GROUP,
    PROP_ACTION_NAME
};


typedef struct _BbPropertyComboBoxPrivate BbPropertyComboBoxPrivate;

struct _BbPropertyComboBoxPrivate
{
    GActionGroup *action_group;
    char *action_name;
    gboolean changed;
};

G_DEFINE_TYPE_WITH_PRIVATE(BbPropertyComboBox, bb_property_combo_box, GTK_TYPE_COMBO_BOX)


static void
bb_property_combo_box_action_added_cb(GActionGroup *action_group, gchar *action_name, BbPropertyComboBox *combo);

static void
bb_property_combo_box_action_removed_cb(GActionGroup *action_group, gchar *action_name, BbPropertyComboBox *combo);

static void
bb_property_combo_box_action_state_changed_cb(GActionGroup *action_group, gchar *action_name, GVariant *value, BbPropertyComboBox *combo);

static void
bb_property_combo_box_action_enabled_changed_cb(GActionGroup *action_group, gchar *action_name, gboolean enabled, BbPropertyComboBox *combo);

static void
bb_property_combo_box_add_widget(BbPropertyComboBox *combo, GtkWidget *widget, gpointer unused);

static void
bb_property_combo_box_emit_update(BbPropertyComboBox *combo, GVariant *state);

static gboolean
bb_property_combo_box_focus_out_event(GtkEntry *entry, GdkEvent *event, BbPropertyComboBox *combo);

static void
bb_property_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_property_combo_box_notify_active(BbPropertyComboBox *combo, GParamSpec *pspec, gpointer unused);

static void
bb_property_combo_box_remove_widget(BbPropertyComboBox *combo, GtkWidget *widget, gpointer unused);

static void
bb_property_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_property_combo_box_value_changed(BbPropertyComboBox *combo, gpointer unused);


static void
bb_property_combo_box_action_added_cb(GActionGroup *action_group, gchar *action_name, BbPropertyComboBox *combo)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_if_fail(privat != NULL);

    if (g_strcmp0(privat->action_name, action_name) == 0)
    {
        gtk_widget_set_sensitive(
            GTK_WIDGET(combo),
            g_action_group_get_action_enabled(action_group, privat->action_name)
            );
    }
}


static void
bb_property_combo_box_action_removed_cb(GActionGroup *action_group, gchar *action_name, BbPropertyComboBox *combo)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_if_fail(privat != NULL);

    if (g_strcmp0(privat->action_name, action_name) == 0)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(combo), FALSE);
    }
}


static void
bb_property_combo_box_action_enabled_changed_cb(GActionGroup *action_group, gchar *action_name, gboolean enabled, BbPropertyComboBox *combo)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_if_fail(privat != NULL);

    if (g_strcmp0(privat->action_name, action_name) == 0)
    {
        gtk_widget_set_sensitive(GTK_WIDGET(combo), enabled);
    }
}


static void
bb_property_combo_box_action_state_changed_cb(GActionGroup *action_group, gchar *action_name, GVariant *value, BbPropertyComboBox *combo)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_if_fail(privat != NULL);

    if (g_strcmp0(privat->action_name, action_name) == 0)
    {
        GVariant *maybe = g_variant_get_maybe(value);

        if (maybe != NULL)
        {
            bb_property_combo_box_emit_update(combo, maybe);
        }
        else
        {
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo), -1);
        }
    }
}


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


static void
bb_property_combo_box_emit_update(BbPropertyComboBox *combo, GVariant *state)
{
    g_return_if_fail(BB_IS_PROPERTY_COMBO_BOX(combo));

    g_signal_emit_by_name(combo, "update", state);
}


static gboolean
bb_property_combo_box_focus_out_event(GtkEntry *entry, GdkEvent *event, BbPropertyComboBox *combo)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_val_if_fail(privat != NULL, FALSE);

    if (privat->changed)
    {
        g_signal_emit_by_name (combo, "apply");
        privat->changed = FALSE;
    }

    return FALSE;
}


static void
bb_property_combo_box_class_init(BbPropertyComboBoxClass *class)
{
    G_OBJECT_CLASS(class)->get_property = bb_property_combo_box_get_property;
    G_OBJECT_CLASS(class)->set_property = bb_property_combo_box_set_property;

    bb_object_class_install_property(
        G_OBJECT_CLASS(class),
        PROP_ACTION_GROUP,
        g_param_spec_object(
            "action-group",
            "",
            "",
            G_TYPE_ACTION_GROUP,
            G_PARAM_READWRITE
            )
        );

    bb_object_class_install_property(
        G_OBJECT_CLASS(class),
        PROP_ACTION_NAME,
        g_param_spec_string(
            "action-name",
            "",
            "",
            "",
            G_PARAM_READWRITE
            )
        );

    class->apply_signal_id = g_signal_new(
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

    class->update_signal_id = g_signal_new(
        "update",
        G_OBJECT_CLASS_TYPE(class),
        (GSignalFlags) 0,
        0,
        NULL,
        NULL,
        g_cclosure_marshal_VOID__VARIANT,
        G_TYPE_NONE,
        1,
        G_TYPE_VARIANT
        );
}


GActionGroup*
bb_property_combo_box_get_action_group(BbPropertyComboBox *combo)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_val_if_fail(privat != NULL, NULL);

    return privat->action_group;
}


const char*
bb_property_combo_box_get_action_name(BbPropertyComboBox *combo)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_val_if_fail(privat != NULL, NULL);

    return privat->action_name;
}


static void
bb_property_combo_box_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ACTION_GROUP:
            g_value_set_object(
                value,
                bb_property_combo_box_get_action_group(BB_PROPERTY_COMBO_BOX(object))
                );
            break;

        case PROP_ACTION_NAME:
            g_value_set_string(
                value,
                bb_property_combo_box_get_action_name(BB_PROPERTY_COMBO_BOX(object))
                );
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
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
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_if_fail(privat != NULL);

    g_signal_emit_by_name (combo, "apply");
    privat->changed = FALSE;
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


void
bb_property_combo_box_set_action_group(BbPropertyComboBox *combo, GActionGroup *group)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_if_fail(privat != NULL);

    if (privat->action_group != NULL)
    {
        g_signal_handlers_disconnect_by_func(
            privat->action_group,
            G_CALLBACK(bb_property_combo_box_action_state_changed_cb),
            combo
            );

        g_signal_handlers_disconnect_by_func(
            privat->action_group,
            G_CALLBACK(bb_property_combo_box_action_enabled_changed_cb),
            combo
            );

        g_signal_handlers_disconnect_by_func(
            privat->action_group,
            G_CALLBACK(bb_property_combo_box_action_removed_cb),
            combo
            );

        g_signal_handlers_disconnect_by_func(
            privat->action_group,
            G_CALLBACK(bb_property_combo_box_action_added_cb),
            combo
            );
    }

    g_set_object(&privat->action_group, group);

    if (privat->action_group != NULL)
    {
        g_signal_connect(
            privat->action_group,
            "action-added",
            G_CALLBACK(bb_property_combo_box_action_added_cb),
            combo
            );

        g_signal_connect(
            privat->action_group,
            "action-removed",
            G_CALLBACK(bb_property_combo_box_action_removed_cb),
            combo
            );

        g_signal_connect(
            privat->action_group,
            "action-enabled-changed",
            G_CALLBACK(bb_property_combo_box_action_enabled_changed_cb),
            combo
            );

        g_signal_connect(
            privat->action_group,
            "action-state-changed",
            G_CALLBACK(bb_property_combo_box_action_state_changed_cb),
            combo
            );
    }
}


void
bb_property_combo_box_set_action_name(BbPropertyComboBox *combo, const char *name)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_if_fail(privat != NULL);

    g_free(privat->action_name);
    privat->action_name = g_strdup(name);
}


static void
bb_property_combo_box_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ACTION_GROUP:
            bb_property_combo_box_set_action_group(
                BB_PROPERTY_COMBO_BOX(object),
                g_value_get_object(value)
                );
            break;

        case PROP_ACTION_NAME:
            bb_property_combo_box_set_action_name(
                BB_PROPERTY_COMBO_BOX(object),
                g_value_get_string(value)
                );
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_property_combo_box_value_changed(BbPropertyComboBox *combo, gpointer unused)
{
    BbPropertyComboBoxPrivate *privat = bb_property_combo_box_get_instance_private(combo);
    g_return_if_fail(privat != NULL);

    GtkWidget *entry = gtk_bin_get_child(GTK_BIN(combo));

    if ((entry != NULL) && gtk_widget_is_focus(entry))
    {
        privat->changed = TRUE;
    }
    else
    {
        g_signal_emit_by_name(combo, "apply");
        privat->changed = FALSE;
    }
}
