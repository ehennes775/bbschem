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
#include <math.h>
#include "bberror.h"
#include "bbpathscanner.h"


enum _BbPathScannerState
{
    STATE_INITIAL_COMMAND,
    STATE_INITIAL_PARAMETER,
    STATE_PARAMETER
};


struct _BbPathScanner
{
    /**
     * The NUL terminated input string, dynamically allocated
     */
    gchar *input;

    /**
     * Points to the current character in the input
     */
    gchar *iter;

    /**
     * The current state of the scanner
     */
    enum _BbPathScannerState state;
};


static gboolean
bb_path_scanner_scan_initial_command(BbPathScanner* scanner, BbPathScannerToken *token, GError **error);

static gboolean
bb_path_scanner_scan_initial_parameter(BbPathScanner* scanner, BbPathScannerToken *token, GError **error);

static void
bb_path_scanner_scan_delimiter(BbPathScanner* scanner);

static gboolean
bb_path_scanner_scan_parameter(BbPathScanner* scanner, BbPathScannerToken *token, GError **error);

static void
bb_path_scanner_scan_whitespace(BbPathScanner* scanner);


void
bb_path_scanner_free(BbPathScanner* scanner)
{
    if (scanner != NULL)
    {
        g_free(scanner->input);
    }

    g_free(scanner);
}


gboolean
bb_path_scanner_get_token(BbPathScanner* scanner, BbPathScannerToken *token, GError **error)
{
    g_return_val_if_fail(scanner != NULL, FALSE);
    g_return_val_if_fail(token != NULL, FALSE);

    GError *local_error = NULL;
    gboolean success = FALSE;

    switch(scanner->state)
    {
        case STATE_INITIAL_COMMAND:
            success = bb_path_scanner_scan_initial_command(scanner, token, &local_error);
            break;

        case STATE_INITIAL_PARAMETER:
            success = bb_path_scanner_scan_initial_parameter(scanner, token, &local_error);
            break;

        case STATE_PARAMETER:
            success = bb_path_scanner_scan_parameter(scanner, token, &local_error);
            break;

        default:
            g_warn_if_reached();
            break;
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        success = FALSE;
    }

    return success;
}


BbPathScanner*
bb_path_scanner_new(const char *input)
{
    BbPathScanner *scanner = g_new0(BbPathScanner, 1);

    scanner->input = g_strdup(input);
    scanner->iter = scanner->input;
    scanner->state = STATE_INITIAL_COMMAND;

    return scanner;
}


/**
 *
 *
 * @param scanner
 * @param token
 * @param error
 * @return
 */
static gboolean
bb_path_scanner_scan_initial_command(BbPathScanner* scanner, BbPathScannerToken *token, GError **error)
{
    bb_path_scanner_scan_whitespace(scanner);

    gboolean success = TRUE;

    if (*scanner->iter == '\0')
    {
        token->tag = BB_PATH_SCANNER_TOKEN_END_OF_INPUT;
        scanner->state = STATE_INITIAL_PARAMETER;
    }
    else
    {
        gboolean success = strchr("CcLlMmZz", *scanner->iter) != NULL;

        if (success)
        {
            token->tag = BB_PATH_SCANNER_TOKEN_COMMAND;
            token->command = g_ascii_tolower(*scanner->iter);
            token->original = *scanner->iter;
            token->relative = g_ascii_islower(*scanner->iter);

            scanner->iter++;
            scanner->state = STATE_INITIAL_PARAMETER;
        }
        else
        {
            g_set_error(
                error,
                BB_ERROR_DOMAIN,
                ERROR_UNKNOWN_PATH_COMMAND,
                "Unknown path command: '%c'",
                *scanner->iter
                );
        }
    }

    return success;
}


/**
 * Scan the first parameter
 *
 *
 */
static gboolean
bb_path_scanner_scan_initial_parameter(BbPathScanner* scanner, BbPathScannerToken *token, GError **error)
{
    g_return_val_if_fail(scanner != NULL, FALSE);

    bb_path_scanner_scan_whitespace(scanner);

    if (*scanner->iter == '\0')
    {
        token->tag = BB_PATH_SCANNER_TOKEN_END_OF_INPUT;
        scanner->state = STATE_INITIAL_PARAMETER;
    }
    else
    {
        gboolean success = strchr("CcLlMmZz", *scanner->iter) != NULL;
        if (success)
        {
            token->tag = BB_PATH_SCANNER_TOKEN_COMMAND;
            token->command = g_ascii_tolower(*scanner->iter);
            token->original = *scanner->iter;
            token->relative = g_ascii_islower(*scanner->iter);

            scanner->iter++;
            scanner->state = STATE_INITIAL_PARAMETER;
        }
        else
        {
            gchar *iter0 = scanner->iter;
            gchar *iter1 = scanner->iter;

            if (*scanner->iter == '-')
            {
                scanner->iter++;
            }
            else if (*scanner->iter == '+')
            {
                scanner->iter++;
            }

            if (!g_ascii_isdigit(*scanner->iter))
            {
                // TODO error
            }

            while (g_ascii_isdigit(*scanner->iter))
            {
                iter1 = scanner->iter;
                scanner->iter++;
            }

            if (*scanner->iter == '.')
            {
                scanner->iter++;

                while (g_ascii_isdigit(*scanner->iter))
                {
                    iter1 = scanner->iter;
                    scanner->iter++;
                }
            }

            double value = g_ascii_strtod(iter0, NULL);

            token->tag = BB_PATH_SCANNER_TOKEN_PARAMETER;
            token->value = round(value);

            scanner->state = STATE_PARAMETER;
        }
    }

    return TRUE;
}


/**
 * Scan past the whitespace and an optional single comma
 *
 * Leaves the input iterator on the next non-whitespace character or at the ending NUL.
 *
 * @param scanner The scanner
 */
static void
bb_path_scanner_scan_delimiter(BbPathScanner* scanner)
{
    g_return_if_fail(scanner != NULL);

    bb_path_scanner_scan_whitespace(scanner);

    g_return_if_fail(scanner->iter != NULL);

    if (*scanner->iter == ',')
    {
        scanner->iter++;

        bb_path_scanner_scan_whitespace(scanner);
    }
}


/**
 * @brief Scan a subsequent parameter
 *
 * As scanning an initial parameter, but skips past an optional, single comma
 *
 * @param scanner
 * @param token
 * @param error
 * @return
 */
static gboolean
bb_path_scanner_scan_parameter(BbPathScanner* scanner, BbPathScannerToken *token, GError **error)
{
    g_return_val_if_fail(scanner != NULL, FALSE);

    bb_path_scanner_scan_delimiter(scanner);

    return bb_path_scanner_scan_initial_parameter(scanner, token, error);
}


/**
 * @brief Scan past the whitespace
 *
 * Leaves the input iterator on the next non-whitespace character or at the ending NUL.
 *
 * @param scanner The scanner
 */
static void
bb_path_scanner_scan_whitespace(BbPathScanner* scanner)
{
    g_return_if_fail(scanner != NULL);
    g_return_if_fail(scanner->iter != NULL);

    while (g_ascii_isspace(*scanner->iter))
    {
        scanner->iter++;
    }
}
