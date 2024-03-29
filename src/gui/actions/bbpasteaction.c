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
#include "bbextensions.h"
#include "bbpasteaction.h"
#include "bbpastereceiver.h"


enum
{
    PROP_0,

    /* From GAction */
    PROP_ENABLED,
    PROP_NAME,
    PROP_PARAMETER_TYPE,
    PROP_STATE,
    PROP_STATE_HINT,
    PROP_STATE_TYPE,

    /* From BbGenericReceiver */
    PROP_RECEIVER,

    /* From BbPasteAction */
    PROP_CLIPBOARD,

    N_PROPERTIES
};


struct _BbPasteAction
{
    GObject parent;

    GtkClipboard *clipboard;
    GObject* receiver;

    gboolean enabled;
};


static void
bb_paste_action_action_init(GActionInterface *iface);

static void
bb_paste_action_activate(GAction *action, GVariant *parameter);

static void
bb_paste_action_change_state(GAction *action, GVariant *value);

static void
bb_paste_action_dispose(GObject *object);

static void
bb_paste_action_finalize(GObject *object);

static void
bb_paste_action_generic_receiver_init(BbGenericReceiverInterface *iface);

static gboolean
bb_paste_action_get_enabled(GAction *action);

static const gchar *
bb_paste_action_get_name(GAction *action);

static const GVariantType *
bb_paste_action_get_parameter_type(GAction *action);

static void
bb_paste_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant *
bb_paste_action_get_state(GAction *action);

static GVariant *
bb_paste_action_get_state_hint(GAction *action);

static const GVariantType *
bb_paste_action_get_state_type(GAction *action);

static void
bb_paste_action_set_enable(BbPasteAction *action, gboolean enabled);

static void
bb_paste_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_paste_action_update_enabled(BbPasteAction *action);


static GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    BbPasteAction,
    bb_paste_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, bb_paste_action_action_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_GENERIC_RECEIVER, bb_paste_action_generic_receiver_init)
)


static void
bb_paste_action_action_init(GActionInterface *iface)
{
    iface->activate = bb_paste_action_activate;
    iface->change_state = bb_paste_action_change_state;
    iface->get_enabled = bb_paste_action_get_enabled;
    iface->get_name = bb_paste_action_get_name;
    iface->get_parameter_type = bb_paste_action_get_parameter_type;
    iface->get_state = bb_paste_action_get_state;
    iface->get_state_hint = bb_paste_action_get_state_hint;
    iface->get_state_type = bb_paste_action_get_state_type;
}


// region From BbGenericReceiver interface

static GObject *
bb_paste_action_generic_receiver_get_receiver(BbGenericReceiver *object)
{
    return bb_paste_action_get_receiver(BB_PASTE_ACTION(object));
}

static void
bb_paste_action_generic_receiver_set_receiver(BbGenericReceiver *object, GObject *receiver)
{
    bb_paste_action_set_receiver(BB_PASTE_ACTION(object), receiver);
}

static void
bb_paste_action_generic_receiver_init(BbGenericReceiverInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_receiver = bb_paste_action_generic_receiver_get_receiver;
    iface->set_receiver = bb_paste_action_generic_receiver_set_receiver;
}

// endregion


static void
bb_paste_action_activate(GAction *action, GVariant *parameter)
{
    g_return_if_fail(action != NULL);

    GtkClipboard *clipboard = bb_paste_action_get_clipboard(BB_PASTE_ACTION((action)));
    GObject *receiver = bb_paste_action_get_receiver(BB_PASTE_ACTION(action));

    if (BB_IS_PASTE_RECEIVER(receiver))
    {
        bb_paste_receiver_paste(BB_PASTE_RECEIVER(receiver), clipboard);
    }
}


static void
bb_paste_action_change_state(GAction *action, GVariant *value)
{

}


static void
bb_paste_action_class_init(BbPasteActionClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_paste_action_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_paste_action_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_paste_action_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_paste_action_set_property;

    properties[PROP_ENABLED] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_ENABLED,
        "enabled"
        );

    properties[PROP_NAME] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_NAME,
        "name"
        );

    properties[PROP_PARAMETER_TYPE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_PARAMETER_TYPE,
        "parameter-type"
        );

    properties[PROP_STATE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_STATE,
        "state"
        );

    properties[PROP_STATE_TYPE] = bb_object_class_override_property(
        G_OBJECT_CLASS(klasse),
        PROP_STATE_TYPE,
        "state-type"
        );

    bb_object_class_install_property(
            G_OBJECT_CLASS(klasse),
            PROP_CLIPBOARD,
            properties[PROP_CLIPBOARD] = g_param_spec_object(
            "clipboard",
            "",
            "",
            GTK_TYPE_CLIPBOARD,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    bb_object_class_install_property(
        G_OBJECT_CLASS(klasse),
        PROP_RECEIVER,
        properties[PROP_RECEIVER] = g_param_spec_object(
            "receiver",
            "",
            "",
            G_TYPE_OBJECT,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );
}


static void
bb_paste_action_clipboard_owner_change(GtkClipboard *clipboard, GdkEventOwnerChange event, BbPasteAction *action)
{
    g_return_if_fail(BB_IS_PASTE_ACTION(action));

    g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_ENABLED]);
}


static void
bb_paste_action_content_received(GtkClipboard *clipboard, GtkSelectionData *selection_data, BbPasteAction *action)
{
    g_return_if_fail(GTK_IS_CLIPBOARD(clipboard));
    g_return_if_fail(BB_IS_PASTE_ACTION(action));

    gboolean enabled =
            BB_IS_PASTE_RECEIVER(action->receiver) &&
            bb_paste_receiver_can_paste(BB_PASTE_RECEIVER(action->receiver), selection_data);

    bb_paste_action_set_enable(action, enabled);

    g_object_unref(action);
}


static void
bb_paste_action_dispose(GObject *object)
{
    BbPasteAction *action = BB_PASTE_ACTION(object);

    g_return_if_fail(action != NULL);

    bb_paste_action_set_receiver(action, NULL);

    G_OBJECT_GET_CLASS(bb_paste_action_parent_class)->dispose(object);
}


static void
bb_paste_action_finalize(GObject *object)
{
}

GtkClipboard *
bb_paste_action_get_clipboard(BbPasteAction *action)
{
    g_return_val_if_fail(BB_IS_PASTE_ACTION(action), NULL);

    return action->clipboard;
}


static gboolean
bb_paste_action_get_enabled(GAction *action)
{
    BbPasteAction *paste_action = BB_PASTE_ACTION(action);

    g_return_val_if_fail(paste_action != NULL, FALSE);

    return paste_action->enabled;
}


static const gchar *
bb_paste_action_get_name(GAction *action)
{
    g_warn_if_fail(action != NULL);

    return "edit-paste";
}


static const GVariantType *
bb_paste_action_get_parameter_type(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static void
bb_paste_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, bb_paste_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_paste_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, bb_paste_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, bb_paste_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, bb_paste_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, bb_paste_action_get_state_type(G_ACTION(object)));
            break;

        case PROP_CLIPBOARD:
            g_value_set_object(value, bb_paste_action_get_clipboard(BB_PASTE_ACTION(object)));
            break;

        case PROP_RECEIVER:
            g_value_set_object(value, bb_paste_action_get_receiver(BB_PASTE_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static GVariant *
bb_paste_action_get_state(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static GVariant *
bb_paste_action_get_state_hint(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static const GVariantType*
bb_paste_action_get_state_type(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


GObject*
bb_paste_action_get_receiver(BbPasteAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return action->receiver;
}


static void
bb_paste_action_init(BbPasteAction *action)
{
    action->clipboard = NULL;
    action->receiver = NULL;
}


BbPasteAction *
bb_paste_action_new(GtkClipboard *clipboard)
{
    return BB_PASTE_ACTION(g_object_new(
        BB_TYPE_PASTE_ACTION,
        "clipboard", clipboard,
        NULL
        ));
}


static void
bb_paste_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CLIPBOARD:
            bb_paste_action_set_clipboard(BB_PASTE_ACTION(object), g_value_get_object(value));
            break;

        case PROP_RECEIVER:
            bb_paste_action_set_receiver(BB_PASTE_ACTION(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_paste_action_set_clipboard(BbPasteAction *action, GtkClipboard *clipboard)
{
    g_return_if_fail(BB_IS_PASTE_ACTION(action));

    if (action->clipboard != clipboard)
    {
        if (action->clipboard != NULL)
        {
            g_object_unref(action->clipboard);
        }

        action->clipboard = clipboard;

        if (action->clipboard != NULL)
        {
            g_object_ref(action->clipboard);

            g_signal_connect(
                    action->clipboard,
                    "owner-change",
                    G_CALLBACK(bb_paste_action_clipboard_owner_change),
                    action
                    );
        }

        bb_paste_action_update_enabled(action);
    }
}


static void
bb_paste_action_set_enable(BbPasteAction *action, gboolean enabled)
{
    g_return_if_fail(BB_IS_PASTE_ACTION(action));

    action->enabled = enabled;

    g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_ENABLED]);
}


void
bb_paste_action_set_receiver(BbPasteAction *action, GObject* receiver)
{
    g_return_if_fail(BB_IS_PASTE_ACTION(action));

    if (action->receiver != receiver)
    {
        if (action->receiver != NULL)
        {
            g_object_unref(action->receiver);
        }

        action->receiver = receiver;

        if (action->receiver != NULL)
        {
            g_object_ref(action->receiver);
        }

        bb_paste_action_update_enabled(action);
        g_object_notify_by_pspec(G_OBJECT(action), properties[PROP_RECEIVER]);
    }
}


static void
bb_paste_action_update_enabled(BbPasteAction *action)
{
    g_return_if_fail(BB_IS_PASTE_ACTION(action));

    if (action->clipboard == NULL || action->receiver == NULL)
    {
        bb_paste_action_set_enable(action, FALSE);
    }
    else
    {
        GdkDisplay *display = gtk_clipboard_get_display(action->clipboard);

        gboolean supported = gdk_display_supports_selection_notification(display);

        if (supported)
        {
            gtk_clipboard_request_contents(
                action->clipboard,
                gdk_atom_intern_static_string("TARGETS"),
                (GtkClipboardReceivedFunc) bb_paste_action_content_received,
                g_object_ref(action)
                );
        }
        else
        {
            bb_paste_action_set_enable(action, TRUE);
        }
    }
}
