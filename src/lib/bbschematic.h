#ifndef __BBSCHEMATIC__
#define __BBSCHEMATIC__
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
#include "bbpred.h"


#define BB_TYPE_SCHEMATIC bb_schematic_get_type()
G_DECLARE_FINAL_TYPE(BbSchematic, bb_schematic, BB, SCHEMATIC, GObject)


void
bb_schematic_add_items(BbSchematic *schematic, GSList *items);

void
bb_schematic_apply_item_property(BbSchematic *schematic, const char *name, const GValue *value);

void
bb_schematic_foreach(BbSchematic *schematic, GFunc func, gpointer user_data);


void
bb_schematic_foreach_modify(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    GFunc modify_func,
    gpointer modify_user_data
    );


/**
 * Modify the first fill angle for a set of items
 *
 * @param schematic A schematic
 * @param where_pred A predicate for filtering items
 * @param where_user_data User data to pass to the filter predicate
 * @param fill_angle The new fill angle for filtered items
 */
void
bb_schematic_modify_fill_angle_1(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    int fill_angle
    );


/**
 * Modify the second fill angle for a set of items
 *
 * @param schematic A schematic
 * @param where_pred A predicate for filtering items
 * @param where_user_data User data to pass to the filter predicate
 * @param fill_angle The new fill angle for filtered items
 */
void
bb_schematic_modify_fill_angle_2(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    int fill_angle
    );


/**
 * Modify the first fill pitch for a set of items
 *
 * @param schematic A schematic
 * @param where_pred A predicate for filtering items
 * @param where_user_data User data to pass to the filter predicate
 * @param fill_pitch The new fill pitch for filtered items
 */
void
bb_schematic_modify_fill_pitch_1(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    int fill_pitch
    );


/**
 * Modify the second fill pitch for a set of items
 *
 * @param schematic A schematic
 * @param where_pred A predicate for filtering items
 * @param where_user_data User data to pass to the filter predicate
 * @param fill_pitch The new fill pitch for filtered items
 */
void
bb_schematic_modify_fill_pitch_2(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    int fill_pitch
    );


void
bb_schematic_modify_fill_type(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    int fill_type
    );


void
bb_schematic_modify_fill_width(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    int fill_width
    );


void
bb_schematic_modify_item_color(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    int color
    );




BbSchematic*
bb_schematic_new();

gboolean
bb_schematic_write(
    BbSchematic *schematic,
    GOutputStream *stream,
    GCancellable *cancellable,
    GError **error
    );

void
bb_schematic_write_async(
    BbSchematic *schematic,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

void
bb_schematic_write_finish(
    BbSchematic *schematic,
    GAsyncResult *result,
    GError **error
    );

#endif
