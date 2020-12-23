#ifndef __BBPROPERTYCOMBOBOX__
#define __BBPROPERTYCOMBOBOX__
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


#define BB_TYPE_PROPERTY_COMBO_BOX bb_property_combo_box_get_type()
G_DECLARE_DERIVABLE_TYPE(BbPropertyComboBox, bb_property_combo_box, BB, PROPERTY_COMBO_BOX, GtkComboBox)

struct _BbPropertyComboBoxClass
{
    GtkComboBoxClass parent_class;

    gint apply_signal_id;
    gint update_signal_id;
};

GActionGroup*
bb_property_combo_box_get_action_group(BbPropertyComboBox *combo);

const char*
bb_property_combo_box_get_action_name(BbPropertyComboBox *combo);

void
bb_property_combo_box_set_action_group(BbPropertyComboBox *combo, GActionGroup *group);

void
bb_property_combo_box_set_action_name(BbPropertyComboBox *combo, const char *name);

#endif
