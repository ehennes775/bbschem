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
#include <bblibrary.h>
#include "bbtoolstack.h"
#include "bbtoolchanger.h"
#include "bbtoolfactory.h"


enum
{
    PROP_0,
    PROP_CAIRO,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


enum
{
    SIG_TOOL_CHANGED,
    N_SIGNALS
};


struct _BbToolStack
{
    GtkStack parent;
};


static BbDrawingTool*
bb_tool_stack_create_tool(BbToolChanger *changer, BbToolSubject *subject);

static void
bb_tool_stack_dispose(GObject *object);

static void
bb_tool_stack_finalize(GObject *object);

static void
bb_tool_stack_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_tool_stack_notify_visible_child_cb(BbToolStack *stack, GParamSpec *psped, gpointer user_data);

static void
bb_tool_stack_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_tool_stack_tool_changer_init(BbToolChangerInterface *iface);


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbToolStack,
    bb_tool_stack,
    GTK_TYPE_STACK,
    G_IMPLEMENT_INTERFACE(BB_TYPE_TOOL_CHANGER, bb_tool_stack_tool_changer_init)
    )


static void
bb_tool_stack_class_init(BbToolStackClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_tool_stack_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_tool_stack_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_tool_stack_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_tool_stack_set_property;

    //gtk_widget_class_set_template_from_resource(
    //    GTK_WIDGET_CLASS(klasse),
    //    "/com/github/ehennes775/bbsch/gui/bbtoolstack.ui"
    //    );

    signals[SIG_TOOL_CHANGED] = g_signal_lookup(
        "tool-changed",
        BB_TYPE_TOOL_CHANGER
        );
}


static BbDrawingTool*
bb_tool_stack_create_tool(BbToolChanger *changer, BbToolSubject *subject)
{
    BbDrawingTool *drawing_tool = NULL;
    GtkWidget *widget = gtk_stack_get_visible_child(GTK_STACK(changer));

    if (BB_IS_TOOL_FACTORY(widget))
    {
        drawing_tool = bb_tool_factory_create_tool(
            BB_TOOL_FACTORY(widget),
            subject
            );
    }

    return drawing_tool;
}


static void
bb_tool_stack_dispose(GObject *object)
{
    // BbToolStack* privat = BBTOOL_STACK_GET_PRIVATE(object);
}


static void
bb_tool_stack_finalize(GObject *object)
{
    // BbToolStack* privat = BBTOOL_STACK_GET_PRIVATE(object);
}


static void
bb_tool_stack_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
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
bb_tool_stack_init(BbToolStack *window)
{
    // gtk_widget_init_template(GTK_WIDGET(window));

    g_signal_connect(
        window,
        "notify::visible-child",
        G_CALLBACK(bb_tool_stack_notify_visible_child_cb),
        NULL
        );
}

static void
bb_tool_stack_notify_visible_child_cb(BbToolStack *stack, GParamSpec *psped, gpointer user_data)
{
    g_signal_emit(stack, signals[SIG_TOOL_CHANGED], 0);
}


__attribute__((constructor)) void
bb_tool_stack_register()
{
    bb_tool_stack_get_type();
}


static void
bb_tool_stack_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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
bb_tool_stack_tool_changer_init(BbToolChangerInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->create_tool = bb_tool_stack_create_tool;
}
