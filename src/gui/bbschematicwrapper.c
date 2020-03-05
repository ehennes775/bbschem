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
#include "bbschematicwrapper.h"


enum
{
    PROP_0,
    PROP_1,
    PROP_2,
    PROP_3
};

typedef struct _Actions Actions;

struct _Actions
{
    GSimpleAction *apply_fill_angle1;
    GSimpleAction *apply_fill_angle2;
    GSimpleAction *apply_fill_pitch1;
    GSimpleAction *apply_fill_pitch2;
    GSimpleAction *apply_fill_type;
    GSimpleAction *apply_line_cap_type;
    GSimpleAction *apply_line_dash_length;
    GSimpleAction *apply_line_dash_space;
    GSimpleAction *apply_line_type;
    GSimpleAction *apply_line_width;
    GSimpleAction *apply_object_color;
    GSimpleAction *apply_pin_type;
    GSimpleAction *apply_text_alignment;
    GSimpleAction *apply_text_color;
    GSimpleAction *apply_text_rotation;
    GSimpleAction *apply_text_size;
    GSimpleAction *delete;
    GSimpleAction *copy;
    GSimpleAction *cut;
    GSimpleAction *paste;
    GSimpleAction *redo;
    GSimpleAction *select_all;
    GSimpleAction *undo;
};

#define ACTION_COUNT (sizeof(Actions)/sizeof(gpointer))


struct _BbSchematicWrapper
{
    GObject parent;

    union
    {
        Actions action;
        GAction *action_array[ACTION_COUNT];
    };
};

G_DEFINE_TYPE(BbSchematicWrapper, bb_schematic_wrapper, G_TYPE_OBJECT);


static void
bb_schematic_wrapper_apply_fill_angle1_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_fill_angle2_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_fill_pitch1_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_fill_pitch2_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_fill_type_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_line_cap_type_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_line_dash_length_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_line_dash_space_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_line_type_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_line_width_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_object_color_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_pin_type_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_text_alignment_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_text_color_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_text_rotation_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_apply_text_size_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_dispose(GObject *object);

static void
bb_schematic_wrapper_finalize(GObject *object);

static void
bb_schematic_wrapper_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_schematic_wrapper_copy_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_cut_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_delete_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_paste_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_redo_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_select_all_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);

static void
bb_schematic_wrapper_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_schematic_wrapper_undo_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper);


void
bb_schematic_wrapper_attach_actions(BbSchematicWrapper *wrapper, GActionMap *map)
{
    g_return_if_fail(BB_IS_SCHEMATIC_WRAPPER(wrapper));
    g_return_if_fail(G_IS_ACTION_MAP(map));

    for (int index = 0; index < ACTION_COUNT; ++index)
    {
        g_action_map_add_action(map, wrapper->action_array[index]);
    }
}


static void
bb_schematic_wrapper_class_init(BbSchematicWrapperClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = bb_schematic_wrapper_dispose;
    G_OBJECT_CLASS(klasse)->finalize = bb_schematic_wrapper_finalize;
    G_OBJECT_CLASS(klasse)->get_property = bb_schematic_wrapper_get_property;
    G_OBJECT_CLASS(klasse)->set_property = bb_schematic_wrapper_set_property;
}


void
bb_schematic_wrapper_detach_actions(BbSchematicWrapper *wrapper, GActionMap *map)
{
    g_return_if_fail(BB_IS_SCHEMATIC_WRAPPER(wrapper));
    g_return_if_fail(G_IS_ACTION_MAP(map));

    for (int index = 0; index < ACTION_COUNT; ++index)
    {
        g_action_map_remove_action(map, g_action_get_name(wrapper->action_array[index]));
    }
}


static void
bb_schematic_wrapper_dispose(GObject *object)
{
    // BbSchematicWrapper* privat = BBSCHEMATIC_WRAPPER_GET_PRIVATE(object);
}


static void
bb_schematic_wrapper_finalize(GObject *object)
{
    // BbSchematicWrapper* privat = BBSCHEMATIC_WRAPPER_GET_PRIVATE(object);
}


static void
bb_schematic_wrapper_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
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
bb_schematic_wrapper_init(BbSchematicWrapper *wrapper)
{
    g_signal_connect(
        wrapper->action.apply_fill_angle1 = g_simple_action_new_stateful(
            "apply-fill-angle1",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_fill_angle1_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_fill_angle2 = g_simple_action_new_stateful(
            "apply-fill-angle2",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_fill_angle2_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_fill_pitch1 = g_simple_action_new_stateful(
            "apply-fill-pitch1",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_fill_pitch1_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_fill_pitch2 = g_simple_action_new_stateful(
            "apply-fill-pitch2",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_fill_pitch2_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_fill_type = g_simple_action_new(
            "apply-fill-type",
            G_VARIANT_TYPE_INT32
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_fill_type_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_line_cap_type = g_simple_action_new(
            "apply-line-cap-type",
            G_VARIANT_TYPE_INT32
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_line_cap_type_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_line_dash_length = g_simple_action_new(
            "apply-line-dash-length",
            G_VARIANT_TYPE_INT32
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_line_dash_length_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_line_dash_space = g_simple_action_new(
            "apply-line-dash-space",
            G_VARIANT_TYPE_INT32
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_line_dash_space_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_line_type = g_simple_action_new(
            "apply-line-type",
            G_VARIANT_TYPE_INT32
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_line_type_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_line_width = g_simple_action_new(
            "apply-line-width",
            G_VARIANT_TYPE_INT32
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_line_width_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_object_color = g_simple_action_new_stateful(
            "apply-object-color",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_object_color_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_pin_type = g_simple_action_new_stateful(
            "apply-pin-type",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_pin_type_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_text_alignment = g_simple_action_new_stateful(
            "apply-text-alignment",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_text_alignment_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_text_color = g_simple_action_new_stateful(
            "apply-text-color",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_text_color_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_text_rotation = g_simple_action_new_stateful(
            "apply-text-rotation",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_text_rotation_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.apply_text_size = g_simple_action_new_stateful(
            "apply-text-size",
            G_VARIANT_TYPE_INT32,
            g_variant_new_maybe(G_VARIANT_TYPE_INT32, NULL)
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_apply_text_size_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.copy = g_simple_action_new(
            "edit-copy",
            NULL
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_copy_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.cut = g_simple_action_new(
            "edit-cut",
            NULL
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_cut_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.delete = g_simple_action_new(
            "edit-delete",
            NULL
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_delete_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.paste = g_simple_action_new(
            "edit-paste",
            NULL
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_paste_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.redo = g_simple_action_new(
            "edit-redo",
            NULL
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_redo_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.select_all = g_simple_action_new(
            "edit-select-all",
            NULL
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_select_all_action_cb),
        wrapper
        );

    g_signal_connect(
        wrapper->action.undo = g_simple_action_new(
            "edit-undo",
            NULL
            ),
        "activate",
        G_CALLBACK(bb_schematic_wrapper_undo_action_cb),
        wrapper
        );
}


__attribute__((constructor)) void
bb_schematic_wrapper_register()
{
    bb_schematic_wrapper_get_type();
}


static void
bb_schematic_wrapper_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
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
bb_schematic_wrapper_apply_fill_angle1_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_fill_angle1_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_fill_angle1,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_fill_angle2_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_fill_angle2_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_fill_angle2,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_fill_pitch1_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_fill_pitch1_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_fill_pitch1,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_fill_pitch2_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_fill_pitch2_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_fill_pitch2,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_fill_type_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_fill_type_action_cb");
}


static void
bb_schematic_wrapper_apply_line_cap_type_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_line_cap_type_action_cb");
}


static void
bb_schematic_wrapper_apply_line_dash_length_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_line_dash_length_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_line_dash_length,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_line_dash_space_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_line_dash_space_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_line_dash_space,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_line_type_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_line_type_action_cb");
}


static void
bb_schematic_wrapper_apply_line_width_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_line_width_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_line_width,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_object_color_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int index = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_object_color_action_cb (color = %d)", index);

    g_simple_action_set_state(
        wrapper->action.apply_object_color,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_pin_type_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_pin_type_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_pin_type,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_text_alignment_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_text_alignment_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_text_alignment,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_text_color_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_text_color_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_text_color,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_text_rotation_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_text_rotation_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_text_rotation,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_apply_text_size_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(g_variant_is_of_type(parameter, G_VARIANT_TYPE_INT32));
    g_return_if_fail(wrapper != NULL);

    int value = g_variant_get_int32(parameter);

    g_message("bb_schematic_wrapper_apply_text_size_action_cb");

    g_simple_action_set_state(
        wrapper->action.apply_text_size,
        g_variant_new_maybe(G_VARIANT_TYPE_INT32, parameter)
        );
}


static void
bb_schematic_wrapper_copy_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(parameter == NULL);
    g_return_if_fail(wrapper != NULL);

    g_message("bb_schematic_wrapper_copy_action_cb");
}


static void
bb_schematic_wrapper_cut_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(parameter == NULL);
    g_return_if_fail(wrapper != NULL);

    g_message("bb_schematic_wrapper_cut_action_cb");
}


static void
bb_schematic_wrapper_delete_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(parameter == NULL);
    g_return_if_fail(wrapper != NULL);

    g_message("bb_schematic_wrapper_delete_action_cb");
}


static void
bb_schematic_wrapper_paste_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(parameter == NULL);
    g_return_if_fail(wrapper != NULL);

    g_message("bb_schematic_wrapper_paste_action_cb");
}


static void
bb_schematic_wrapper_redo_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(parameter == NULL);
    g_return_if_fail(wrapper != NULL);

    g_message("bb_schematic_wrapper_redo_action_cb");
}


static void
bb_schematic_wrapper_select_all_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(parameter == NULL);
    g_return_if_fail(wrapper != NULL);

    g_message("bb_schematic_wrapper_select_all_action_cb");
}


static void
bb_schematic_wrapper_undo_action_cb(GSimpleAction *simple, GVariant *parameter, BbSchematicWrapper *wrapper)
{
    g_return_if_fail(parameter == NULL);
    g_return_if_fail(wrapper != NULL);

    g_message("bb_schematic_wrapper_undo_action_cb");
}
