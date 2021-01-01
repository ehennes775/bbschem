#ifndef __BBGEDAREADER__
#define __BBGEDAREADER__
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
/**
 * @file bbgedareader.h
 *
 * A class for reading schematics and symbols in the gEDA file format.
 *
 * Unlike most "reader" type classes, there is only one instance of this reader and functions are reentrant.
 *
 * Programming errors in the asynchronous functions are fatal.
 */

#include <gtk/gtk.h>
#include <bblibrary.h>


#define BB_TYPE_GEDA_READER bb_geda_reader_get_type()
G_DECLARE_FINAL_TYPE(BbGedaReader, bb_geda_reader, BB, GEDA_READER, GObject)


/**
 * @brief Begin reading a gEDA schematic or symbol file asynchronously
 *
 * This function is reentrant.
 *
 * @param reader A BbGedaReader to perform the read operation
 * @param stream A GDataInputStream to read the schematic from
 * @param cancellable A token to cancel the asynchronous operation
 * @param callback A callback function when the asynchronous operation is complete (i.e. ready)
 * @param user_data Generic data to pass to the GAsyncReadyCallback callback
 */
void
bb_geda_reader_read_async(
    BbGedaReader *reader,
    GDataInputStream *stream,
    BbSchematic *schematic,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );


/**
 * Obtain results from reading a gEDA schematic or symbol file asynchronously
 *
 * This function is reentrant.
 *
 * @param reader The BbGedaReader passed to bb_geda_reader_read_async()
 * @param result
 * @param error
 * @return
 */
void*
bb_geda_reader_read_finish(
    BbGedaReader *reader,
    GAsyncResult *result,
    GError **error
    );


#endif
