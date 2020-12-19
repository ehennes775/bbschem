#ifndef __BBELECTRICALPIN__
#define __BBELECTRICALPIN__
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
#include "bbparams.h"
#include "bbschematicitem.h"


#define BB_ELECTRICAL_PIN_TOKEN "P"


#define BB_TYPE_ELECTRICAL_PIN bb_electrical_pin_get_type()
G_DECLARE_FINAL_TYPE(BbElectricalPin, bb_electrical_pin, BB, ELECTRICAL_PIN, BbSchematicItem)


int
bb_electrical_pin_get_x0(BbElectricalPin *pin);


int
bb_electrical_pin_get_x1(BbElectricalPin *pin);


int
bb_electrical_pin_get_y0(BbElectricalPin *pin);


int
bb_electrical_pin_get_y1(BbElectricalPin *pin);


BbElectricalPin*
bb_electrical_pin_new();


BbElectricalPin*
bb_electrical_pin_new_with_params(BbParams *params, GError **error);


void
bb_electrical_pin_set_x0(BbElectricalPin *pin, int x);


void
bb_electrical_pin_set_x1(BbElectricalPin *pin, int x);


void
bb_electrical_pin_set_y0(BbElectricalPin *pin, int y);


void
bb_electrical_pin_set_y1(BbElectricalPin *pin, int y);


#endif
