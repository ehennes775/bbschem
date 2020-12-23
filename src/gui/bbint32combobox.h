#ifndef __BBCOLORCOMBOBOX__
#define __BBCOLORCOMBOBOX__
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
#include "bbpropertycombobox.h"

#define BB_TYPE_INT32_COMBO_BOX bb_int32_combo_box_get_type()
G_DECLARE_FINAL_TYPE(BbInt32ComboBox, bb_int32_combo_box, BB, INT32_COMBO_BOX, BbPropertyComboBox)

int
bb_int32_combo_box_get_value(BbInt32ComboBox *combo);

int
bb_int32_combo_box_parse_value(BbInt32ComboBox *combo, GError **error);

void
bb_int32_combo_box_set_value(BbInt32ComboBox *combo, int value);

#endif
