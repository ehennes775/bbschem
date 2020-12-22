#ifndef __BBGEDATEXT__
#define __BBGEDATEXT__
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
#include "bbgedaitem.h"
#include "bbparams.h"
#include "bbtextalignment.h"
#include "bbtextpresentation.h"
#include "bbtextvisibility.h"


#define BB_GEDA_TEXT_TOKEN "T"


#define BB_TYPE_GEDA_TEXT bb_geda_text_get_type()
G_DECLARE_FINAL_TYPE(BbGedaText, bb_geda_text, BB, GEDA_TEXT, BbGedaItem)


BbTextAlignment
bb_geda_text_get_alignment(BbGedaText *text);


int
bb_geda_text_get_insert_x(BbGedaText *text);


int
bb_geda_text_get_insert_y(BbGedaText *text);


/**
 * @brief Get the line count from the parameters
 *
 * Returns the number of lines of text following the parameter line inside a gEDA file.
 *
 * @param params The parameters to extract the line count from
 * @param error Any errors encountered
 * @return A valid line count (result &gt; 0) or an error (result &lt;= 0)
 */
int
bb_geda_text_get_line_count(BbParams *params, GError **error);


BbTextPresentation
bb_geda_text_get_presentation(BbGedaText *text);


int
bb_geda_text_get_rotation(BbGedaText *text);


int
bb_geda_text_get_size(BbGedaText *text);


gchar*
bb_geda_text_get_text(BbGedaText *text_item);


BbTextVisibility
bb_geda_text_get_visibility(BbGedaText *text);


BbGedaText*
bb_geda_text_new_with_params(BbParams *params, gchar **lines, GError **error);


void
bb_geda_text_set_alignment(BbGedaText *text_item, BbTextAlignment alignment);


void
bb_geda_text_set_insert_x(BbGedaText *text, int x);


void
bb_geda_text_set_insert_y(BbGedaText *text, int y);


void
bb_geda_text_set_presentation(BbGedaText *text_item, BbTextPresentation presentation);


void
bb_geda_text_set_rotation(BbGedaText *text_item, int rotation);


void
bb_geda_text_set_size(BbGedaText *text_item, int size);


void
bb_geda_text_set_text(BbGedaText *text_item, const gchar *text);


void
bb_geda_text_set_visibility(BbGedaText *text_item, BbTextVisibility visibility);


#endif
