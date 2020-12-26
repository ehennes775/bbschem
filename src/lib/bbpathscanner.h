#ifndef __BBPATHSCANNER__
#define __BBPATHSCANNER__
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


/**
 * @brief An opaque type representing the scanner
 */
typedef struct _BbPathScanner BbPathScanner;


/**
 * @brief The types of tokens returned from the scanner.
 */
typedef enum _BbPathScannerTokenTag BbPathScannerTokenTag;

enum _BbPathScannerTokenTag
{
    BB_PATH_SCANNER_TOKEN_UNKNOWN,
    BB_PATH_SCANNER_TOKEN_COMMAND,
    BB_PATH_SCANNER_TOKEN_END_OF_INPUT,
    BB_PATH_SCANNER_TOKEN_PARAMETER
};


/**
 * @brief A variant structure for tokens and associated data
 */
typedef struct _BbPathScannerToken BbPathScannerToken;

struct _BbPathScannerToken
{
    /**
     * @brief The type of token
     */
    BbPathScannerTokenTag tag;

    union
    {
        struct
        {
            /**
             * @brief For BB_PATH_SCANNER_TOKEN_COMMAND types: The path command normalized to lower case
             */
            gchar command;

            /**
             * @brief For BB_PATH_SCANNER_TOKEN_COMMAND types: The path command in the original case read from input
             */
            gchar original;

            /**
             * @brief For BB_PATH_SCANNER_TOKEN_COMMAND types: TRUE indicates this command requests relative coordinates
             */
            gboolean relative;
        };

        struct
        {
            /**
             * @brief For BB_PATH_SCANNER_TOKEN_PARAMETER types: The parameter converted to an integer
             */
            int value;
        };
    };
};


/**
 * @param scanner The scanner to free, optionally NULL
 */
void
bb_path_scanner_free(BbPathScanner* scanner);


/**
 * @brief Scan a token from the input stream
 *
 * @param scanner A BbPathScanner
 * @param token The output token, scanned from the stream. Must not be NULL.
 * @param error Any errors encountered
 * @return TRUE on success. FALSE if a GError error was encountered. If the function returns false and no error, then
 * a logic or programming error was encountered.
 */
gboolean
bb_path_scanner_get_token(BbPathScanner* scanner, BbPathScannerToken *token, GError **error);


/**
 * @brief Create a new path scanner
 *
 * @param input The input string to parse the path from.
 * @return A new path scanner for the given input string.
 */
BbPathScanner*
bb_path_scanner_new(const char *input);


#endif
