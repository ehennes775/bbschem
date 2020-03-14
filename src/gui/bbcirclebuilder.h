#ifndef __BBCIRCLEBUILDER__
#define __BBCIRCLEBUILDER__
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
#include "bbitembuilder.h"

#define BB_TYPE_CIRCLE_BUILDER bb_circle_builder_get_type()
G_DECLARE_FINAL_TYPE(BbCircleBuilder, bb_circle_builder, BB, CIRCLE_BUILDER, BbItemBuilder)


int
bb_circle_builder_get_x0(BbCircleBuilder *builder);

int
bb_circle_builder_get_x1(BbCircleBuilder *builder);

int
bb_circle_builder_get_y0(BbCircleBuilder *builder);

int
bb_circle_builder_get_y1(BbCircleBuilder *builder);

void
bb_circle_builder_set_x0(BbCircleBuilder *builder, int x);

void
bb_circle_builder_set_x1(BbCircleBuilder *builder, int x);

void
bb_circle_builder_set_y0(BbCircleBuilder *builder, int y);

void
bb_circle_builder_set_y1(BbCircleBuilder *builder, int y);

#endif
