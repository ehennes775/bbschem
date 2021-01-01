#ifndef __BBGENERALOPENER__
#define __BBGENERALOPENER__
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
#include "bbdocumentwindow.h"
#include "bbspecificopener.h"

#define BB_TYPE_GENERAL_OPENER bb_general_opener_get_type()
G_DECLARE_FINAL_TYPE(BbGeneralOpener, bb_general_opener, BB, GENERAL_OPENER, GObject)


/**
 * Add a handler for a specific file format
 *
 * @param general_opener A
 * @param content_type The content type of the specific opener handler (e.g. 'application/x-lepton-symbol')
 * @param specific_opener The handler for the specific content type
 */
G_MODULE_EXPORT void
bb_general_opener_add_specific_opener(
    BbGeneralOpener *general_opener,
    gchar *content_type,
    BbSpecificOpener *specific_opener
    );


/**
 * @brief
 *
 * @return
 */
BbGeneralOpener*
bb_general_opener_new();


/**
 * @brief Begin opening a document window
 *
 * @param opener
 * @param file The file to open
 * @param cancellable
 * @param callback A callback function when the open operation has been started
 * @param user_data User data to pass to the callback function
 */
void
bb_general_opener_read_content_type_async(
    BbGeneralOpener *opener,
    GFile *file,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );


/**
 * @brief
 *
 * @param opener
 * @param result
 * @param error
 * @return TRUE if successful
 * @return FALSE if a logic error or runtime error occurred
 *
 * @see bb_general_opener_read_content_type_async()
 */
gboolean
bb_general_opener_read_content_type_finish(
    BbGeneralOpener *opener,
    GAsyncResult *result,
    GError **error
    );


#endif
