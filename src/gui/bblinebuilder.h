#ifndef __BBLINEBUILDER__
#define __BBLINEBUILDER__
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

#define BB_TYPE_LINE_BUILDER bb_line_builder_get_type()
G_DECLARE_FINAL_TYPE(BbLineBuilder, bb_line_builder, BB, LINE_BUILDER, BbItemBuilder)


int
bb_line_builder_get_x0(BbLineBuilder *builder);

int
bb_line_builder_get_x1(BbLineBuilder *builder);

int
bb_line_builder_get_y0(BbLineBuilder *builder);

int
bb_line_builder_get_y1(BbLineBuilder *builder);

void
bb_line_builder_set_x0(BbLineBuilder *builder, int x);

void
bb_line_builder_set_x1(BbLineBuilder *builder, int x);

void
bb_line_builder_set_y0(BbLineBuilder *builder, int y);

void
bb_line_builder_set_y1(BbLineBuilder *builder, int y);

#endif
