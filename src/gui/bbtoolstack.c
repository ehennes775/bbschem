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
#include <bbextensions.h>
#include "bbtoolstack.h"
#include "bbtoolchanger.h"
#include "bbtoolfactory.h"
#include "bbgridcontrol.h"


enum
{
    PROP_0,

    /* From BbGridControl */
    PROP_GRID_VISIBLE,
    PROP_SNAP_ACTIVE,

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

    gboolean grid_visible;
    gboolean snap_active;
};


static BbDrawingTool*
bb_tool_stack_create_tool(BbToolChanger *changer, BbToolSubject *subject);

static void
bb_tool_stack_dispose(GObject *object);

static void
bb_tool_stack_finalize(GObject *object);

static gboolean
bb_tool_stack_get_grid_visible(BbGridControl *grid_control);

static void
bb_tool_stack_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static gboolean
bb_tool_stack_get_snap_active(BbGridControl *grid_control);

static void
bb_tool_stack_grid_control_init(BbGridControlInterface *iface);

static void
bb_tool_stack_notify_visible_child_cb(BbToolStack *stack, GParamSpec *psped, gpointer user_data);

static void
bb_tool_stack_set_grid_visible(BbGridControl *grid_control, gboolean visible);

static void
bb_tool_stack_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_tool_stack_set_snap_active(BbGridControl *grid_control, gboolean active);

static void
bb_tool_stack_tool_changer_init(BbToolChangerInterface *iface);


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbToolStack,
    bb_tool_stack,
    GTK_TYPE_STACK,
    G_IMPLEMENT_INTERFACE(BB_TYPE_GRID_CONTROL, bb_tool_stack_grid_control_init)
    G_IMPLEMENT_INTERFACE(BB_TYPE_TOOL_CHANGER, bb_tool_stack_tool_changer_init)
    )


static void
bb_tool_stack_class_init(BbToolStackClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);

    object_class->dispose = bb_tool_stack_dispose;
    object_class->finalize = bb_tool_stack_finalize;
    object_class->get_property = bb_tool_stack_get_property;
    object_class->set_property = bb_tool_stack_set_property;

    //gtk_widget_class_set_template_from_resource(
    //    GTK_WIDGET_CLASS(klasse),
    //    "/com/github/ehennes775/bbsch/gui/bbtoolstack.ui"
    //    );

    /* From BbGridControl */

    properties[PROP_GRID_VISIBLE] = bb_object_class_override_property(
        object_class,
        PROP_GRID_VISIBLE,
        "grid-visible"
        );

    properties[PROP_SNAP_ACTIVE] = bb_object_class_override_property(
        object_class,
        PROP_SNAP_ACTIVE,
        "snap-active"
        );

    /* From BbToolChanger */

    signals[SIG_TOOL_CHANGED] = g_signal_lookup(
        "tool-changed",
        BB_TYPE_TOOL_CHANGER
        );
}


static BbDrawingTool*
bb_tool_stack_select_tool(BbToolChanger *changer, BbDrawingToolSupport *support)
{
    BbDrawingTool *drawing_tool = NULL;
    GtkWidget *widget = gtk_stack_get_visible_child(GTK_STACK(changer));

    if (BB_IS_TOOL_FACTORY(widget))
    {
        drawing_tool = bb_tool_factory_select_tool(
            BB_TOOL_FACTORY(widget),
            support
            );
    }

    return drawing_tool;
}


static void
bb_tool_stack_dispose(GObject *object)
{
}


static void
bb_tool_stack_finalize(GObject *object)
{
}


static gboolean
bb_tool_stack_get_grid_visible(BbGridControl *grid_control)
{
    BbToolStack *tool_stack = BB_TOOL_STACK(grid_control);
    g_return_val_if_fail(BB_IS_TOOL_STACK(tool_stack), TRUE);

    return tool_stack->grid_visible;
}


static void
bb_tool_stack_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_GRID_VISIBLE:
            g_value_set_boolean(value, bb_tool_stack_get_grid_visible(BB_GRID_CONTROL(object)));
            break;

        case PROP_SNAP_ACTIVE:
            g_value_set_boolean(value, bb_tool_stack_get_snap_active(BB_GRID_CONTROL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static gboolean
bb_tool_stack_get_snap_active(BbGridControl *grid_control)
{
    BbToolStack *tool_stack = BB_TOOL_STACK(grid_control);
    g_return_val_if_fail(BB_IS_TOOL_STACK(tool_stack), TRUE);

    return tool_stack->snap_active;
}


static void
bb_tool_stack_grid_control_init(BbGridControlInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_grid_visible = bb_tool_stack_get_grid_visible;
    iface->get_snap_active = bb_tool_stack_get_snap_active;
    iface->set_grid_visible = bb_tool_stack_set_grid_visible;
    iface->set_snap_active = bb_tool_stack_set_snap_active;
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
bb_tool_stack_set_grid_visible(BbGridControl *grid_control, gboolean visible)
{
    BbToolStack *tool_stack = BB_TOOL_STACK(grid_control);
    g_return_if_fail(BB_IS_TOOL_STACK(tool_stack));

    if (tool_stack->grid_visible != visible)
    {
        tool_stack->grid_visible = visible;

        g_object_notify_by_pspec(G_OBJECT(tool_stack), properties[PROP_GRID_VISIBLE]);
    }
}


static void
bb_tool_stack_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_GRID_VISIBLE:
            bb_tool_stack_set_grid_visible(BB_GRID_CONTROL(object), g_value_get_boolean(value));
            break;

        case PROP_SNAP_ACTIVE:
            bb_tool_stack_set_snap_active(BB_GRID_CONTROL(object), g_value_get_boolean(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_tool_stack_set_snap_active(BbGridControl *grid_control, gboolean active)
{
    BbToolStack *tool_stack = BB_TOOL_STACK(grid_control);
    g_return_if_fail(BB_IS_TOOL_STACK(tool_stack));

    if (tool_stack->snap_active != active)
    {
        tool_stack->snap_active = active;

        g_object_notify_by_pspec(G_OBJECT(tool_stack), properties[PROP_SNAP_ACTIVE]);
    }
}


static void
bb_tool_stack_tool_changer_init(BbToolChangerInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->select_tool = bb_tool_stack_select_tool;
}

