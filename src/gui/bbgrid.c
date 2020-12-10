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
#include "bbgrid.h"

#define BB_GRID_DEFAULT_SIZE (100)
#define BB_GRID_MAXIMUM_SIZE (102400)
#define BB_GRID_MINIMUM_SIZE (25)

enum
{
    PROP_0,
    PROP_CAN_SCALE_DOWN,
    PROP_CAN_SCALE_UP,
    PROP_DRAW_SIZE,
    PROP_SNAP_SIZE,
    PROP_SUBJECT,
    N_PROPERTIES
};


struct _BbGrid
{
    GObject parent;

    int snap_index;

    int draw_index;

    BbToolSubject *subject;
};


static int
bb_grid_calculate_draw_index(BbGrid *grid, int snap_index);

static int
bb_grid_calculate_size(int index);

static void
bb_grid_dispose(GObject *object);

static void
bb_grid_finalize(GObject *object);

static void
bb_grid_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_grid_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_grid_set_subject(BbGrid *grid, BbToolSubject *subject);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE(
    BbGrid,
    bb_grid,
    G_TYPE_OBJECT
    )



static int
bb_grid_calculate_draw_index(BbGrid *grid, int snap_index)
{
    int draw_index = snap_index;
    int draw_size = bb_grid_calculate_size(draw_index);

    // TODO Add condition if grid lines would be too close together to go to next higher grid
    while (draw_size <= BB_GRID_MAXIMUM_SIZE && FALSE)
    {
        draw_index++;

        draw_size = bb_grid_calculate_size(draw_index);
    }

    return draw_index;
}


static int
bb_grid_calculate_size(int index)
{
    return BB_GRID_MINIMUM_SIZE * (1 << index);
}


static void
bb_grid_class_init(BbGridClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_grid_dispose;
    object_class->finalize = bb_grid_finalize;
    object_class->get_property = bb_grid_get_property;
    object_class->set_property = bb_grid_set_property;

    properties[PROP_CAN_SCALE_DOWN] = bb_object_class_install_property(
        object_class,
        PROP_CAN_SCALE_DOWN,
        g_param_spec_boolean(
            "can-scale-down",
            "",
            "",
            FALSE,
            G_PARAM_READABLE
            )
        );

    properties[PROP_CAN_SCALE_UP] = bb_object_class_install_property(
        object_class,
        PROP_CAN_SCALE_UP,
        g_param_spec_boolean(
            "can-scale-up",
            "",
            "",
            FALSE,
            G_PARAM_READABLE
            )
        );

    properties[PROP_DRAW_SIZE] = bb_object_class_install_property(
        object_class,
        PROP_DRAW_SIZE,
        g_param_spec_int(
            "draw-size",
            "",
            "",
            BB_GRID_MINIMUM_SIZE,
            BB_GRID_MAXIMUM_SIZE,
            BB_GRID_DEFAULT_SIZE,
            G_PARAM_READABLE
            )
        );

    properties[PROP_SNAP_SIZE] = bb_object_class_install_property(
        object_class,
        PROP_SNAP_SIZE,
        g_param_spec_int(
            "snap-size",
            "",
            "",
            BB_GRID_MINIMUM_SIZE,
            BB_GRID_MAXIMUM_SIZE,
            BB_GRID_DEFAULT_SIZE,
            G_PARAM_READABLE
        )
    );

    properties[PROP_SUBJECT] = bb_object_class_install_property(
        object_class,
        PROP_SUBJECT,
        g_param_spec_object(
            "subject",
            "",
            "",
            BB_TYPE_TOOL_SUBJECT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY
            )
        );

}


static void
bb_grid_dispose(GObject *object)
{
    BbGrid *grid = BB_GRID(object);
    g_return_if_fail(grid != NULL);

    g_clear_object(&grid->subject);
}


static void
bb_grid_finalize(GObject *object)
{
}


BbGrid*
bb_grid_new(BbToolSubject *tool_subject)
{
    return BB_GRID(g_object_new(
        BB_TYPE_GRID,
        "subject", tool_subject,
        NULL
        ));
}


gboolean
bb_grid_get_can_scale(BbGrid *grid, BbScaleGridDirection direction)
{
    switch (direction)
    {
        case BB_SCALE_GRID_DIRECTION_DOWN:
            return bb_grid_get_can_scale_down(grid);

        case BB_SCALE_GRID_DIRECTION_RESET:
            return bb_grid_get_can_scale_reset(grid);

        case BB_SCALE_GRID_DIRECTION_UP:
            return bb_grid_get_can_scale_up(grid);

        default:
            g_return_val_if_reached(FALSE);
    }
}


gboolean
bb_grid_get_can_scale_down(BbGrid *grid)
{
    g_return_val_if_fail(grid != NULL, FALSE);

    return grid->snap_index > 0;
}


gboolean
bb_grid_get_can_scale_reset(BbGrid *grid)
{
    g_warn_if_fail(grid != NULL);

    return TRUE;
}


gboolean
bb_grid_get_can_scale_up(BbGrid *grid)
{
    g_return_val_if_fail(grid != NULL, FALSE);

    int next_size_up = bb_grid_calculate_size(grid->snap_index + 1);

    return next_size_up <= BB_GRID_MAXIMUM_SIZE;
}

int
bb_grid_get_draw_size(BbGrid *grid)
{
    g_return_val_if_fail(grid != NULL, BB_GRID_DEFAULT_SIZE);

    return bb_grid_calculate_size(grid->draw_index);
}


static void
bb_grid_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAN_SCALE_DOWN:
            g_value_set_boolean(value, bb_grid_get_can_scale_down(BB_GRID(object)));
            break;

        case PROP_CAN_SCALE_UP:
            g_value_set_boolean(value, bb_grid_get_can_scale_up(BB_GRID(object)));
            break;

        case PROP_SUBJECT:
            g_value_set_object(value, bb_grid_get_subject(BB_GRID(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_grid_get_snap_size(BbGrid *grid)
{
    g_return_val_if_fail(grid != NULL, BB_GRID_DEFAULT_SIZE);

    return grid->snap_index;
}


BbToolSubject*
bb_grid_get_subject(BbGrid *grid)
{
    g_return_val_if_fail(grid != NULL, NULL);

    return grid->subject;
}


static void
bb_grid_init(BbGrid *grid)
{
    grid->snap_index = 3;
    grid->draw_index = 3;
}

void
bb_grid_scale(BbGrid *grid, BbScaleGridDirection direction)
{
    switch (direction)
    {
        case BB_SCALE_GRID_DIRECTION_DOWN:
            bb_grid_scale_down(grid);
            break;

        case BB_SCALE_GRID_DIRECTION_RESET:
            bb_grid_scale_reset(grid);
            break;

        case BB_SCALE_GRID_DIRECTION_UP:
            bb_grid_scale_up(grid);
            break;

        default:
            g_warn_if_reached();
    }
}


void
bb_grid_scale_down(BbGrid *grid)
{
    g_return_if_fail(grid != NULL);
    g_return_if_fail(bb_grid_get_can_scale_down(grid));

    grid->snap_index--;

    grid->draw_index = bb_grid_calculate_draw_index(grid, grid->snap_index);

    bb_tool_subject_invalidate_all(grid->subject);

    g_object_notify(G_OBJECT(grid), properties[PROP_CAN_SCALE_UP]);
    g_object_notify(G_OBJECT(grid), properties[PROP_CAN_SCALE_DOWN]);
    g_object_notify(G_OBJECT(grid), properties[PROP_DRAW_SIZE]);
    g_object_notify(G_OBJECT(grid), properties[PROP_SNAP_SIZE]);
}


void
bb_grid_draw(BbGrid *grid, BbGraphics *graphics)
{
    g_return_if_fail(BB_IS_GRID(grid));

    bb_graphics_draw_grid(graphics, bb_grid_get_draw_size(grid));
}


void
bb_grid_scale_reset(BbGrid *grid)
{
    g_return_if_fail(grid != NULL);
    g_return_if_fail(bb_grid_get_can_scale_down(grid));

    grid->snap_index = 3;

    grid->draw_index = bb_grid_calculate_draw_index(grid, grid->snap_index);

    bb_tool_subject_invalidate_all(grid->subject);

    g_object_notify(G_OBJECT(grid), properties[PROP_CAN_SCALE_UP]);
    g_object_notify(G_OBJECT(grid), properties[PROP_CAN_SCALE_DOWN]);
    g_object_notify(G_OBJECT(grid), properties[PROP_DRAW_SIZE]);
    g_object_notify(G_OBJECT(grid), properties[PROP_SNAP_SIZE]);
}


void
bb_grid_scale_up(BbGrid *grid)
{

    g_return_if_fail(grid != NULL);
    g_return_if_fail(bb_grid_get_can_scale_up(grid));

    grid->snap_index++;

    grid->draw_index = bb_grid_calculate_draw_index(grid, grid->snap_index);

    bb_tool_subject_invalidate_all(grid->subject);

    g_object_notify(G_OBJECT(grid), properties[PROP_CAN_SCALE_UP]);
    g_object_notify(G_OBJECT(grid), properties[PROP_CAN_SCALE_DOWN]);
    g_object_notify(G_OBJECT(grid), properties[PROP_DRAW_SIZE]);
    g_object_notify(G_OBJECT(grid), properties[PROP_SNAP_SIZE]);
}


static void
bb_grid_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_SUBJECT:
            bb_grid_set_subject(BB_GRID(object), g_value_get_object(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_grid_set_subject(BbGrid *grid, BbToolSubject *subject)
{
    g_return_if_fail(BB_IS_GRID(grid));

    if (grid->subject != subject)
    {
        if (grid->subject != NULL)
        {
            g_object_unref(grid->subject);
        }

        grid->subject = subject;

        if (grid->subject != NULL)
        {
            g_object_ref(grid->subject);
        }

        g_object_notify_by_pspec(G_OBJECT(grid), properties[PROP_SUBJECT]);
    }
}


