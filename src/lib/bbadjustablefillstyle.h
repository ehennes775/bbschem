#ifndef __BBADJUSTABLEFILLSTYLE__
#define __BBADJUSTABLEFILLSTYLE__
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
#include "bbfilltype.h"

#define BB_TYPE_ADJUSTABLE_FILL_STYLE bb_adjustable_fill_style_get_type()
G_DECLARE_INTERFACE(BbAdjustableFillStyle, bb_adjustable_fill_style, BB, ADJUSTABLE_FILL_STYLE, GObject)

struct _BbAdjustableFillStyleInterface
{
    GTypeInterface parent;
};


int
bb_adjustable_fill_style_get_fill_angle_1(BbAdjustableFillStyle *item);


int
bb_adjustable_fill_style_get_fill_angle_2(BbAdjustableFillStyle *item);


int
bb_adjustable_fill_style_get_fill_pitch_1(BbAdjustableFillStyle *item);


int
bb_adjustable_fill_style_get_fill_pitch_2(BbAdjustableFillStyle *item);


BbFillType
bb_adjustable_fill_style_get_fill_type(BbAdjustableFillStyle *item);


int
bb_adjustable_fill_style_get_fill_width(BbAdjustableFillStyle *item);


void
bb_adjustable_fill_style_set_fill_angle_1(BbAdjustableFillStyle *item, int angle);


void
bb_adjustable_fill_style_set_fill_angle_2(BbAdjustableFillStyle *item, int angle);


void
bb_adjustable_fill_style_set_fill_pitch_1(BbAdjustableFillStyle *item, int pitch);


void
bb_adjustable_fill_style_set_fill_pitch_2(BbAdjustableFillStyle *item, int pitch);


void
bb_adjustable_fill_style_set_fill_type(BbAdjustableFillStyle *item, BbFillType type);


void
bb_adjustable_fill_style_set_fill_width(BbAdjustableFillStyle *item, int width);


#endif
