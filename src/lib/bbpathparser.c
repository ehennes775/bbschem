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
#include "bbpathparser.h"
#include "bbpathscanner.h"
#include "bberror.h"
#include "bbpathcommand.h"
#include "bbabsolutemoveto.h"
#include "bbrelativemoveto.h"
#include "bbabsolutelineto.h"
#include "bbrelativelineto.h"
#include "bbclosepath.h"


typedef struct _BbPathParser BbPathParser;

struct _BbPathParser
{
    /**
     * @brief The path commands, accumulated by parsing, stored in reverse order
     */
    GSList *commands;
    BbPathScanner *scanner;
};


/**
 *
 */
typedef gboolean (*EmitFunc)(BbPathParser *parser, int count, int parameter[count], GError **error);


gboolean
parse_parameters(BbPathScanner *scanner, BbPathScannerToken *token, int count, int parameter[count], GError **error);

static gboolean
parse_initial_command(BbPathParser *parser, BbPathScannerToken *token, GError **error);

static gboolean
parse_close_parameters(BbPathScanner *scanner, BbPathScannerToken *token, GError **error);

static gboolean
parse_parse_and_emit(
    BbPathParser *parser,
    BbPathScannerToken *token,
    gboolean relative,
    int parameter_count,
    EmitFunc emit,
    GError **error
    );

static gboolean
parse_line_to_parameters(BbPathScanner *scanner, BbPathScannerToken *token, GError **error);

static gboolean
parse_move_to_parameters(BbPathScanner *scanner, BbPathScannerToken *token, GError **error);

static gboolean
parse_subsequent_commands(BbPathParser *parser, BbPathScannerToken *token, GError **error);


GSList*
bb_path_parser_parse(const char *input, GError **error)
{
    GError *local_error = NULL;
    BbPathParser parser =
    {
        .commands = NULL,
        .scanner = bb_path_scanner_new(input)
    };
    BbPathScannerToken token = { .tag = BB_PATH_SCANNER_TOKEN_UNKNOWN };

    gboolean success = parse_initial_command(&parser, &token, &local_error);

    if (success && (local_error == NULL))
    {
        success = parse_subsequent_commands(&parser, &token, &local_error);
    }

    if ((local_error == NULL) && !success)
    {
        local_error = g_error_new(BB_ERROR_DOMAIN, 0, "Internal error");
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        g_slist_free_full(g_steal_pointer(&parser.commands), g_object_unref);
    }

    bb_path_scanner_free(g_steal_pointer(&parser.scanner));

    return g_slist_reverse(parser.commands);
}


static gboolean
emit_close_path(BbPathParser *parser, int count, int *parameter, GError **error)
{
    g_return_val_if_fail(count == 0, FALSE);

    parser->commands = g_slist_prepend(
        parser->commands,
        BB_PATH_COMMAND(bb_close_path_new())
        );

    return TRUE;
}


static gboolean
emit_curve_to(BbPathParser *parser, int count, int parameter[count], GError **error)
{
    g_return_val_if_fail(count == 6, FALSE);

    g_message("Curve to");

    return TRUE;
}


static gboolean
emit_line_to(BbPathParser *parser, int count, int parameter[count], GError **error)
{
    g_return_val_if_fail(parser != NULL, FALSE);
    g_return_val_if_fail(count == 2, FALSE);

    gboolean success = TRUE;

    if (parser->commands == NULL)
    {
        g_set_error(
            error,
            BB_ERROR_DOMAIN,
            0,
            "A line to path command cannot be first"
            );

        success = FALSE;
    }
    else if (TRUE)
    {
        parser->commands = g_slist_prepend(
            parser->commands,
            BB_PATH_COMMAND(bb_absolute_line_to_new(parameter[0], parameter[1]))
            );
    }
    else
    {
        parser->commands = g_slist_prepend(
            parser->commands,
            BB_PATH_COMMAND(bb_relative_line_to_new(parameter[0], parameter[1]))
            );
    }

    return success;
}


static gboolean
emit_move_to(BbPathParser *parser, int count, int parameter[count], GError **error)
{
    g_return_val_if_fail(parser != NULL, FALSE);
    g_return_val_if_fail(count == 2, FALSE);

    BbPathCommand *command = NULL;

    if (parser->commands == NULL)
    {
        command = BB_PATH_COMMAND(bb_absolute_move_to_new(parameter[0], parameter[1]));
    }
    else
    {
        command = BB_PATH_COMMAND(bb_relative_move_to_new(parameter[0], parameter[1]));
    }

    parser->commands = g_slist_prepend(
        parser->commands,
        command
        );

    return TRUE;
}



gboolean
get_parameters(BbPathScanner *scanner, int count, int parameter[count], GError **error)
{
    int index;
    GError *local_error = NULL;
    gboolean success = TRUE;
    BbPathScannerToken token;

    for (index = 0; index < count; index++)
    {
        success = bb_path_scanner_get_token(scanner, &token, &local_error);

        if ((local_error != NULL) || !success)
        {
            break;
        }

        if (token.tag != BB_PATH_SCANNER_TOKEN_PARAMETER)
        {
            local_error = g_error_new(BB_ERROR_DOMAIN, 0, "Parameter expected");
        }

        parameter[index] = token.value;
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
    }

    return success;
}


/**
 * Parse the initial command
 *
 * @param scanner
 * @param token
 * @param error
 * @return TRUE if successful
 */
static gboolean
parse_initial_command(BbPathParser *parser, BbPathScannerToken *token, GError **error)
{
    g_return_val_if_fail(parser != NULL, FALSE);
    g_return_val_if_fail(parser->scanner != NULL, FALSE);
    g_return_val_if_fail(token != NULL, FALSE);

    GError *local_error = NULL;

    gboolean success = bb_path_scanner_get_token(parser->scanner, token, &local_error);

    if (success && (local_error == NULL))
    {
        if (token->tag != BB_PATH_SCANNER_TOKEN_COMMAND || token->command != 'm')
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_EXPECTED_MOVETO,
                "Expected an initial move to command in path"
                );
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        success = FALSE;
    }

    return success;
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
parse_subsequent_commands(BbPathParser *parser, BbPathScannerToken *token, GError **error)
{
    g_return_val_if_fail(parser != NULL, FALSE);
    g_return_val_if_fail(parser->scanner != NULL, FALSE);
    g_return_val_if_fail(token != NULL, FALSE);
    g_return_val_if_fail(token->tag == BB_PATH_SCANNER_TOKEN_COMMAND, FALSE);

    GError *local_error = NULL;
    gboolean success;

    while (token->tag == BB_PATH_SCANNER_TOKEN_COMMAND)
    {
        BbPathScannerToken command_token = *token;
        EmitFunc repeat_emit_func;
        int parameter_count = 0;

        success = bb_path_scanner_get_token(parser->scanner, token, &local_error);

        if (!success || (local_error != NULL))
        {
            break;
        }

        switch (command_token.command)
        {
            case 'c':
                parameter_count = 6;
                repeat_emit_func = emit_curve_to;
                success = parse_parse_and_emit(
                    parser,
                    token,
                    command_token.relative,
                    parameter_count,
                    repeat_emit_func,
                    &local_error
                    );
                break;

            case 'l':
                parameter_count = 2;
                repeat_emit_func = emit_line_to;
                success = parse_parse_and_emit(
                    parser,
                    token,
                    command_token.relative,
                    parameter_count,
                    repeat_emit_func,
                    &local_error
                    );
                break;

            case 'm':
                parameter_count = 2;
                success = parse_parse_and_emit(
                    parser,
                    token,
                    command_token.relative,
                    parameter_count,
                    emit_move_to,
                    &local_error
                    );
                repeat_emit_func = emit_line_to;
                break;

            case 'z':
                parameter_count = 0;
                success = parse_parse_and_emit(
                    parser,
                    token,
                    command_token.relative,
                    parameter_count,
                    emit_close_path,
                    &local_error
                    );
                repeat_emit_func = NULL;
                break;

            default:
                local_error = g_error_new(
                    BB_ERROR_DOMAIN,
                    ERROR_UNKNOWN_PATH_COMMAND,
                    "Unknown path command '%c'",
                    command_token.original
                    );
        }

        if (!success || (local_error != NULL))
        {
            break;
        }

        if (parameter_count > 0)
        {
            while (token->tag == BB_PATH_SCANNER_TOKEN_PARAMETER)
            {
                success = parse_parse_and_emit(
                    parser,
                    token,
                    command_token.relative,
                    parameter_count,
                    repeat_emit_func,
                    &local_error
                    );

                if (!success || (local_error != NULL))
                {
                    break;
                }
            }
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        success = FALSE;
    }

    return success;
}


/**
 *
 *
 * @param scanner
 * @param token
 * @param parameter_count
 * @param emit
 * @param error
 * @return
 */
static gboolean
parse_parse_and_emit(
    BbPathParser *parser,
    BbPathScannerToken *token,
    gboolean relative,
    int parameter_count,
    EmitFunc emit,
    GError **error
    )
{
    g_return_val_if_fail(parser != NULL, FALSE);
    g_return_val_if_fail(parser->scanner != NULL, FALSE);
    g_return_val_if_fail(token != NULL, FALSE);
    g_return_val_if_fail(parameter_count == 0 || token->tag == BB_PATH_SCANNER_TOKEN_PARAMETER, FALSE);
    g_return_val_if_fail(emit != NULL, FALSE);

    GError *local_error = NULL;
    int parameter[parameter_count];
    gboolean success;

    if (parameter_count == 0)
    {
        success = emit(parser, parameter_count, parameter, &local_error);
    }
    else
    {
        success = parse_parameters(parser->scanner, token, parameter_count, parameter, &local_error);

        if (success && (local_error == NULL))
        {
            emit(parser, parameter_count, parameter, &local_error);
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        success = FALSE;
    }

    return success;

}


gboolean
parse_parameters(BbPathScanner *scanner, BbPathScannerToken *token, int count, int parameter[count], GError **error)
{
    int index = 0;
    GError *local_error = NULL;
    gboolean success = TRUE;

    while (index < count)
    {
        if (token->tag != BB_PATH_SCANNER_TOKEN_PARAMETER)
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                0,
                "Expected parameter"
                );

            break;
        }

        parameter[index++] = token->value;

        success = bb_path_scanner_get_token(scanner, token, &local_error);

        if (!success || (local_error != NULL))
        {
            break;
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        success = FALSE;
    }

    return success;
}