#ifndef __BBDOCUMENTWINDOW__
#define __BBDOCUMENTWINDOW__
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

#define BB_TYPE_DOCUMENT_WINDOW bb_document_window_get_type()
G_DECLARE_DERIVABLE_TYPE(BbDocumentWindow, bb_document_window, BB, DOCUMENT_WINDOW, GtkBox)

struct _BbDocumentWindowClass
{
    GtkBoxClass parent_class;
};


const char*
bb_document_window_get_tab(BbDocumentWindow *window);


#endif
