#ifndef __BBMAINWINDOW__
#define __BBMAINWINDOW__
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
#include "bbapplication.h"
#include "bbdocumentwindow.h"


#define BB_TYPE_MAIN_WINDOW bb_main_window_get_type()
G_DECLARE_FINAL_TYPE(BbMainWindow, bb_main_window, BB, MAIN_WINDOW, GtkApplicationWindow)


struct _BbMainWindowClass
{
    GtkApplicationWindowClass parent;
};


void
bb_main_window_add_page(BbMainWindow *window, BbDocumentWindow *page);

GtkWidget*
bb_main_window_get_current_document_window(BbMainWindow* window);

BbMainWindow*
bb_main_window_new(BbApplication *application);


#endif
