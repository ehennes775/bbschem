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
#include <bblibrary.h>
#include "bbtexttool.h"
#include "bbdrawingtool.h"


enum
{
    PROP_0,
    PROP_PROTOTYPE,
    PROP_RECEIVER,
    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE_ITEM,
    N_SIGNALS
};


struct _BbTextTool
{
    GObject parent;

    BbGedaText *prototype;

    BbToolSubject *subject;
};


static gboolean
bb_text_tool_button_pressed(BbDrawingTool *tool, double x, double y);

static gboolean
bb_text_tool_button_released(BbDrawingTool *tool, double x, double y);

static void
bb_text_tool_dispose(GObject *object);

static void
bb_text_tool_draw(BbDrawingTool *tool, BbGraphics *graphics);

static void
bb_text_tool_drawing_tool_init(BbDrawingToolInterface *iface);

static void
bb_text_tool_finalize(GObject *object);

static void
bb_text_tool_finish(BbTextTool *text_tool);

static BbGedaText*
bb_text_tool_get_prototype(BbTextTool *tool);

static void
bb_text_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static BbToolSubject*
bb_text_tool_get_subject(BbTextTool *tool);

static void
bb_text_tool_invalidate_item_cb(BbGedaItem *item, BbTextTool *text_tool);

static void
bb_text_tool_key_pressed(BbDrawingTool *tool);

static void
bb_text_tool_key_released(BbDrawingTool *tool);

static gboolean
bb_text_tool_motion_notify(BbDrawingTool *tool, double x, double y);

static void
bb_text_tool_reset(BbTextTool *text_tool);

static void
bb_text_tool_reset_with_point(BbTextTool *tool, double x, double y);

static void
bb_text_tool_set_prototype(BbTextTool *tool, BbGedaText *item);

static void
bb_text_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_text_tool_set_subject(BbTextTool *tool, BbToolSubject *subject);

static void
bb_text_tool_update_with_point(BbTextTool *text_tool, double x, double y);


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_EXTENDED(
    BbTextTool,
    bb_text_tool,
    G_TYPE_OBJECT,
    0,
    G_IMPLEMENT_INTERFACE(BB_TYPE_DRAWING_TOOL, bb_text_tool_drawing_tool_init)
    )


static gboolean
bb_text_tool_button_pressed(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbTextTool *text_tool = BB_TEXT_TOOL(tool);
    g_return_val_if_fail(text_tool != NULL, FALSE);

    bb_text_tool_update_with_point(text_tool, x, y);
    bb_text_tool_finish(text_tool);

    return TRUE;
}

static gboolean
bb_text_tool_button_released(BbDrawingTool *tool, double x, double y)
{
}


static void
bb_text_tool_class_init(BbTextToolClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_text_tool_dispose;
    object_class->finalize = bb_text_tool_finalize;
    object_class->get_property = bb_text_tool_get_property;
    object_class->set_property = bb_text_tool_set_property;

    properties[PROP_PROTOTYPE] = bb_object_class_install_property(
        object_class,
        PROP_PROTOTYPE,
        g_param_spec_object(
            "prototype",
            "",
            "",
            BB_TYPE_GEDA_TEXT,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_RECEIVER] = bb_object_class_install_property(
            object_class,
            PROP_RECEIVER,
            g_param_spec_object(
            "subject",
            "",
            "",
            BB_TYPE_TOOL_SUBJECT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );

    signals[SIG_INVALIDATE_ITEM] = g_signal_lookup(
        "invalidate-item",
        BB_TYPE_DRAWING_TOOL
        );
}


static void
bb_text_tool_class_finalize(BbTextToolClass *klasse)
{
}


static void
bb_text_tool_dispose(GObject *object)
{
    BbTextTool *text_tool = BB_TEXT_TOOL(object);
    g_return_if_fail(text_tool != NULL);

    bb_text_tool_set_prototype(text_tool, NULL);
    bb_text_tool_set_subject(text_tool, NULL);
}


static void
bb_text_tool_draw(BbDrawingTool *tool, BbGraphics *graphics)
{
    BbTextTool *text_tool = BB_TEXT_TOOL(tool);
    g_return_if_fail(BB_IS_TEXT_TOOL(text_tool));
    g_return_if_fail(BB_IS_GRAPHICS(graphics));

    bb_geda_item_render(
        BB_GEDA_ITEM(text_tool->prototype),
        BB_ITEM_RENDERER(graphics)
        );
}


static void
bb_text_tool_drawing_tool_init(BbDrawingToolInterface *iface)
{
    g_return_if_fail(iface != NULL);
    
    iface->button_pressed = bb_text_tool_button_pressed;
    iface->button_released = bb_text_tool_button_released;
    iface->draw = bb_text_tool_draw;
    iface->key_pressed = bb_text_tool_key_pressed;
    iface->key_released = bb_text_tool_key_released;
    iface->motion_notify = bb_text_tool_motion_notify;
}


static void
bb_text_tool_finalize(GObject *object)
{
}


static void
bb_text_tool_finish(BbTextTool *text_tool)
{
    g_return_if_fail(text_tool != NULL);

    BbGedaItem *item = bb_geda_item_clone(BB_GEDA_ITEM(text_tool->prototype));

    bb_tool_subject_add_item(text_tool->subject, item);

    g_clear_object(&item);

    bb_text_tool_reset(text_tool);
}


static BbGedaText*
bb_text_tool_get_prototype(BbTextTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->prototype;
}


static void
bb_text_tool_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_PROTOTYPE:
            g_value_set_object(value, bb_text_tool_get_prototype(BB_TEXT_TOOL(object)));
            break;

        case PROP_RECEIVER:
            g_value_set_object(value, bb_text_tool_get_subject(BB_TEXT_TOOL(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static BbToolSubject*
bb_text_tool_get_subject(BbTextTool *tool)
{
    g_return_val_if_fail(tool != NULL, NULL);

    return tool->subject;
}


static void
bb_text_tool_init(BbTextTool *text_tool)
{
    bb_text_tool_reset(text_tool);
}


static void
bb_text_tool_invalidate_item_cb(BbGedaItem *item, BbTextTool *text_tool)
{
    g_return_if_fail(text_tool != NULL);
    g_return_if_fail(text_tool->prototype != NULL);
    g_return_if_fail(text_tool->prototype == BB_GEDA_TEXT(item));

    g_signal_emit(text_tool, signals[SIG_INVALIDATE_ITEM], 0, item);
}


static void
bb_text_tool_key_pressed(BbDrawingTool *tool)
{
    g_message("bb_text_tool_key_pressed");
}


static void
bb_text_tool_key_released(BbDrawingTool *tool)
{
    g_message("bb_text_tool_key_released");
}


BbTextTool*
bb_text_tool_new(BbToolSubject *subject, BbTextControl *text_control)
{
    BbGedaText *prototype = BB_GEDA_TEXT(g_object_new(BB_TYPE_GEDA_TEXT,
         "alignment", bb_text_control_get_alignment(text_control),
         "item-color", bb_text_control_get_color(text_control),
         "presentation", bb_text_control_get_presentation(text_control),
         "rotation", bb_text_control_get_rotation(text_control),
         "size", bb_text_control_get_size(text_control),
         "text", bb_text_control_get_text(text_control),
         "visibility", bb_text_control_get_visibility(text_control),
         NULL
         ));

    g_object_bind_property(
        text_control, "alignment",
        prototype, "alignment",
        G_BINDING_DEFAULT
        );

    g_object_bind_property(
        text_control, "color",
        prototype, "item-color",
        G_BINDING_DEFAULT
        );

    g_object_bind_property(
        text_control, "presentation",
        prototype, "presentation",
        G_BINDING_DEFAULT
        );

    g_object_bind_property(
        text_control, "rotation",
        prototype, "rotation",
        G_BINDING_DEFAULT
        );

    g_object_bind_property(
        text_control, "size",
        prototype, "size",
        G_BINDING_DEFAULT
        );

    g_object_bind_property(
        text_control, "text",
        prototype, "text",
        G_BINDING_DEFAULT
        );

    g_object_bind_property(
        text_control, "visibility",
        prototype, "visibility",
        G_BINDING_DEFAULT
        );

    return BB_TEXT_TOOL(g_object_new(
        BB_TYPE_TEXT_TOOL,
        "prototype", prototype,
        "subject", subject,
        NULL
        ));
}


//void
//bb_text_tool_register(GTypeModule *module)
//{
//    bb_text_tool_register_type(module);
//}


static void
bb_text_tool_reset(BbTextTool *text_tool)
{
    g_return_if_fail(text_tool != NULL);
}


static void
bb_text_tool_reset_with_point(BbTextTool *text_tool, gdouble x, gdouble y)
{
    g_return_if_fail(text_tool != NULL);
    g_return_if_fail(text_tool->prototype != NULL);

    double ux;
    double uy;

    gboolean success = bb_tool_subject_widget_to_user(text_tool->subject, x, y, &ux, &uy);
    g_return_if_fail(success);

    int sx;
    int sy;

    bb_geda_text_set_insert_x(text_tool->prototype, sx);
    bb_geda_text_set_insert_y(text_tool->prototype, sy);
}


static gboolean
bb_text_tool_motion_notify(BbDrawingTool *tool, gdouble x, gdouble y)
{
    BbTextTool *text_tool = BB_TEXT_TOOL(tool);
    g_return_val_if_fail(text_tool != NULL, FALSE);

    bb_text_tool_update_with_point(text_tool, x, y);

    return TRUE;
}


static void
bb_text_tool_set_prototype(BbTextTool *tool, BbGedaText *prototype)
{
    g_return_if_fail(tool != NULL);

    if (tool->prototype != prototype)
    {
        if (tool->prototype != NULL)
        {
            g_signal_handlers_disconnect_by_data(prototype, tool);

            g_object_unref(tool->prototype);
        }

        tool->prototype = prototype;

        if (tool->prototype != NULL)
        {
            g_object_ref(tool->prototype);

            g_signal_connect_object(
                prototype,
                "invalidate-item",
                G_CALLBACK(bb_text_tool_invalidate_item_cb),
                tool,
                0
                );
        }

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_PROTOTYPE]);
    }
}


static void
bb_text_tool_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_PROTOTYPE:
            bb_text_tool_set_prototype(BB_TEXT_TOOL(object), BB_GEDA_TEXT(g_value_get_object(value)));
            break;

        case PROP_RECEIVER:
            bb_text_tool_set_subject(BB_TEXT_TOOL(object), BB_TOOL_SUBJECT(g_value_get_object(value)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_text_tool_set_subject(BbTextTool *tool, BbToolSubject *subject)
{
    g_return_if_fail(tool != NULL);

    if (tool->subject != subject)
    {
        if (tool->subject != NULL)
        {
            g_object_unref(tool->subject);
        }

        tool->subject = subject;

        if (tool->subject != NULL)
        {
            g_object_ref(tool->subject);
        }

        g_object_notify_by_pspec(G_OBJECT(tool), properties[PROP_RECEIVER]);
    }
}


static void
bb_text_tool_update_with_point(BbTextTool *text_tool, gdouble x, gdouble y)
{
    g_return_if_fail(text_tool != NULL);
    g_return_if_fail(text_tool->prototype != NULL);

    double ux;
    double uy;

    gboolean success = bb_tool_subject_widget_to_user(text_tool->subject, x, y, &ux, &uy);
    g_return_if_fail(success);

    int sx;
    int sy;

    bb_tool_subject_snap_coordinate(text_tool->subject, bb_coord_round(ux), bb_coord_round(uy), &sx, &sy);

    bb_geda_text_set_insert_x(text_tool->prototype, sx);
    bb_geda_text_set_insert_y(text_tool->prototype, sy);
}
