#ifndef __BBGEDASCHEMATICREADER__
#define __BBGEDASCHEMATICREADER__
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
 * @file bbgedaschematicreader.h
 *
 * A class for reading schematics and symbols in the gEDA file format.
 *
 * Unlike most "reader" type classes, there is only one instance of this reader and functions are reentrant.
 *
 * Programming errors in the asynchronous functions are fatal.
 */

#include <gtk/gtk.h>


#define BB_TYPE_GEDA_SCHEMATIC_READER bb_geda_schematic_reader_get_type()
G_DECLARE_FINAL_TYPE(BbGedaSchematicReader, bb_geda_schematic_reader, BB, GEDA_SCHEMATIC_READER, GObject)


/**
 * @brief Begin reading a gEDA schematic or symbol file asynchronously
 *
 * This function is reentrant.
 *
 * @param reader A BbGedaSchematicReader
 * @param stream The GDataInputStream to read the schematic from
 * @param cancellable A token to cancel the asynchronous operation
 * @param callback The callback function when the asynchronous operation is complete (i.e. ready)
 * @param user_data Generic data to pass to the GAsyncReadyCallback callback
 */
void
bb_geda_schematic_reader_read_async(
    BbGedaSchematicReader *reader,
    GDataInputStream *stream,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer user_data
    );


/**
 * Obtain the results from reading a gEDA schematic or symbol file asynchronously
 *
 * This function is reentrant.
 *
 * @param reader A BbGedaSchematicReader
 * @param result
 * @param error
 * @return An unused pointer
 */
void*
bb_geda_schematic_reader_read_finish(
    BbGedaSchematicReader *reader,
    GAsyncResult *result,
    GError **error
    );


#endif
