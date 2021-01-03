#ifndef __BBALIGNMENTCOMBOBOX__
#define __BBALIGNMENTCOMBOBOX__
/*
 * bbschem
 * Copyright (C) 2021 Edward C. Hennessy
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
#include <bbtextalignment.h>
#include "bbpropertycombobox.h"

#define BB_TYPE_ALIGNMENT_COMBO_BOX bb_alignment_combo_box_get_type()
G_DECLARE_FINAL_TYPE(BbAlignmentComboBox, bb_alignment_combo_box, BB, ALIGNMENT_COMBO_BOX, BbPropertyComboBox)


/**
 *
 *
 * @param combo
 * @return
 */
BbTextAlignment
bb_alignment_combo_box_get_alignment(BbAlignmentComboBox *combo);


/**
 *
 *
 * @param combo
 * @param alignment
 */
void
bb_alignment_combo_box_set_alignment(BbAlignmentComboBox *combo, BbTextAlignment alignment);


#endif
