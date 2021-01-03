#ifndef __BBTEXTCONTROL__
#define __BBTEXTCONTROL__
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
#include "bbtextalignment.h"
#include "bbtextpresentation.h"
#include "bbtextvisibility.h"


#define BB_TYPE_TEXT_CONTROL bb_text_control_get_type()
G_DECLARE_INTERFACE(BbTextControl, bb_text_control, BB, TEXT_CONTROL, GObject)

struct _BbTextControlInterface
{
    GTypeInterface g_iface;

    BbTextAlignment (*get_alignment)(BbTextControl *text_control);
    int (*get_color)(BbTextControl *text_control);
    BbTextPresentation (*get_presentation)(BbTextControl *text_control);
    int (*get_rotation)(BbTextControl *text_control);
    int (*get_size)(BbTextControl *text_control);
    const gchar*(*get_text)(BbTextControl *text_control);
    BbTextVisibility (*get_visibility)(BbTextControl *text_control);
};


/**
 * @return The text alignment
 */
BbTextAlignment
bb_text_control_get_alignment(BbTextControl *text_control);


/**
 * @return The color index
 */
int
bb_text_control_get_color(BbTextControl *text_control);


/**
 * @return The presentation, for attributes
 * @return BB_TEXT_PRESENTATION_BOTH, for text
 */
BbTextPresentation
bb_text_control_get_presentation(BbTextControl *text_control);


/**
 * @return The rotation in degrees
 */
int
bb_text_control_get_rotation(BbTextControl *text_control);


/**
 * @return The text size in points
 */
int
bb_text_control_get_size(BbTextControl *text_control);


/**
 * @return The entire contents of the text or attribute
 */
const gchar*
bb_text_control_get_text(BbTextControl *text_control);


/**
 * @return The visibility, for attributes
 * @return BB_TEXT_VISIBLE, for text
 */
BbTextVisibility
bb_text_control_get_visibility(BbTextControl *text_control);


#endif
