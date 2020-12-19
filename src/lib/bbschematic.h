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
#include "bbqueryfunc.h"
#include "bbapplyfunc.h"


#define BB_TYPE_SCHEMATIC bb_schematic_get_type()
G_DECLARE_FINAL_TYPE(BbSchematic, bb_schematic, BB, SCHEMATIC, GObject)

void
bb_schematic_add_item(BbSchematic *schematic, BbSchematicItem *item);


void
bb_schematic_add_items(BbSchematic *schematic, GSList *items);


void
bb_schematic_apply_item_property(BbSchematic *schematic, const char *name, const GValue *value);


void
bb_schematic_foreach(BbSchematic *schematic, GFunc func, gpointer user_data);


void
bb_schematic_calculate_bounds(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    BbBoundsCalculator *calculator,
    BbBounds *bounds
    );


/**
 * Query items in the schematic
 *
 * @param schematic A schematic with items to query
 * @param where_pred A predicate to filter items passed to the query function
 * @param where_user_data A user pointer to pass to the filter predicate
 * @param query_func A function to query data from the schematic items
 * @param query_user_data A user pointer to pass to the query function
 */
void
bb_schematic_foreach_query(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    BbQueryFunc query_func,
    gpointer query_user_data
    );


/**
 * Modify items in the schematic
 *
 * @param schematic A schematic with items to modify
 * @param where_pred A predicate to filter items passed to the modify function
 * @param where_user_data A user pointer to pass to the filter predicate
 * @param modify_func A function to modify items in the schematic
 * @param modify_user_data A user pointer to pass to the modify function
 */
void
bb_schematic_foreach_modify(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    BbApplyFunc modify_func,
    gpointer modify_user_data
    );


/**
 * Remove items from a schematic
 *
 * @param schematic A schematic with items to modify
 * @param where_pred A predicate indicating which items to remove
 * @param where_user_data A user pointer to pass to the predicate
 */
void
bb_schematic_foreach_remove(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data
    );


BbSchematic*
bb_schematic_new();


void
bb_schematic_render(
    BbSchematic *schematic,
    BbItemRenderer *renderer
    );


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
