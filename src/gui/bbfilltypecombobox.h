#ifndef __BBFILLTYPECOMBOBOX__
#define __BBFILLTYPECOMBOBOX__
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
#include "bbpropertycombobox.h"

#define BB_TYPE_FILL_TYPE_COMBO_BOX bb_fill_type_combo_box_get_type()
G_DECLARE_FINAL_TYPE(BbFillTypeComboBox, bb_fill_type_combo_box, BB, FILL_TYPE_COMBO_BOX, BbPropertyComboBox)


/**
 *
 *
 * @param combo
 * @return
 */
BbFillType
bb_fill_type_combo_box_get_fill_type(BbFillTypeComboBox *combo);


/**
 *
 *
 * @param combo
 * @param index
 */
void
bb_fill_type_combo_box_set_fill_type(BbFillTypeComboBox *combo, BbFillType index);


#endif
