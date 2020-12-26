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
#include <bbpathscanner.h>


struct _AbsoluteRelativeTest
{
    char *path;
    char command;
    char original;
    gboolean relative;
};


void
absolute_relative_test()
{
    struct _AbsoluteRelativeTest test[] =
    {
        { "c", 'c', 'c' , TRUE },
        { "C", 'c', 'C' , FALSE },
        { "l", 'l', 'l' , TRUE },
        { "L", 'l', 'L' , FALSE },
        { "m", 'm', 'm' , TRUE },
        { "M", 'm', 'M' , FALSE },
        { "z", 'z', 'z' , TRUE },
        { "Z", 'z', 'Z' , FALSE }
    };

    int count = sizeof(test) / sizeof(struct _AbsoluteRelativeTest);

    for (int index = 0; index < count; index++)
    {
        GError *local_error = NULL;
        BbPathScanner *scanner = bb_path_scanner_new(test[index].path);
        gboolean success;
        BbPathScannerToken token = {.tag = BB_PATH_SCANNER_TOKEN_UNKNOWN};

        success = bb_path_scanner_get_token(scanner, &token, &local_error);

        g_assert_cmpint(token.tag, ==, BB_PATH_SCANNER_TOKEN_COMMAND);
        g_assert_cmpint(token.command, ==, test[index].command);
        g_assert_cmpint(token.original, ==, test[index].original);
        g_assert_cmpint(!token.relative, ==, !test[index].relative);
        g_assert_null(local_error);
        g_assert_true(success);

        bb_path_scanner_free(scanner);
    }
}



void
initial_line_to_test()
{
    const char *path[] =
    {
        "L100,200",
        "L 100,200",
        "L100 200",
        "L 100, 200",
        "L 100 200"
    };

    const int count = sizeof(path) / sizeof (char*);

    for (int index = 0; index < count; index++)
    {
        GError *local_error = NULL;
        BbPathScanner *scanner = bb_path_scanner_new(path[index]);
        BbPathScannerToken token = { .tag = BB_PATH_SCANNER_TOKEN_UNKNOWN };

        bb_path_scanner_get_token(scanner, &token, &local_error);

        g_assert_cmpint(token.tag, ==, BB_PATH_SCANNER_TOKEN_COMMAND);
        g_assert_cmpint(token.command, ==, 'l');
        g_assert_cmpint(token.original, ==, 'L');
        g_assert_cmpint(token.relative, ==, FALSE);
        g_assert_null(local_error);

        bb_path_scanner_get_token(scanner, &token, &local_error);

        g_assert_cmpint(token.tag, ==, BB_PATH_SCANNER_TOKEN_PARAMETER);
        g_assert_cmpint(token.value, ==, 100);
        g_assert_null(local_error);

        bb_path_scanner_get_token(scanner, &token, &local_error);

        g_assert_cmpint(token.tag, ==, BB_PATH_SCANNER_TOKEN_PARAMETER);
        g_assert_cmpint(token.value, ==, 200);
        g_assert_null(local_error);

        bb_path_scanner_get_token(scanner, &token, &local_error);

        g_assert_cmpint(token.tag, ==, BB_PATH_SCANNER_TOKEN_END_OF_INPUT);
        g_assert_null(local_error);
    }
}


void
initial_move_to_test()
{
    const char *path[] =
    {
        "M100,200",
        "M 100,200",
        "M100 200",
        "M 100, 200",
        "M 100 200"
    };

    const int count = sizeof(path) / sizeof (char*);

    for (int index = 0; index < count; index++)
    {
        GError *local_error = NULL;
        BbPathScanner *scanner = bb_path_scanner_new(path[index]);
        BbPathScannerToken token = { .tag = BB_PATH_SCANNER_TOKEN_UNKNOWN };

        bb_path_scanner_get_token(scanner, &token, &local_error);

        g_assert_cmpint(token.tag, ==, BB_PATH_SCANNER_TOKEN_COMMAND);
        g_assert_cmpint(token.command, ==, 'm');
        g_assert_cmpint(token.original, ==, 'M');
        g_assert_cmpint(token.relative, ==, FALSE);
        g_assert_null(local_error);

        bb_path_scanner_get_token(scanner, &token, &local_error);

        g_assert_cmpint(token.tag, ==, BB_PATH_SCANNER_TOKEN_PARAMETER);
        g_assert_cmpint(token.value, ==, 100);
        g_assert_null(local_error);

        bb_path_scanner_get_token(scanner, &token, &local_error);

        g_assert_cmpint(token.tag, ==, BB_PATH_SCANNER_TOKEN_PARAMETER);
        g_assert_cmpint(token.value, ==, 200);
        g_assert_null(local_error);

        bb_path_scanner_get_token(scanner, &token, &local_error);

        g_assert_cmpint(token.tag, ==, BB_PATH_SCANNER_TOKEN_END_OF_INPUT);
        g_assert_null(local_error);
    }
}


int
main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func(
        "/bbpathscannertest/absoluterelativetest",
        absolute_relative_test
        );

    g_test_add_func(
        "/bbpathscannertest/initiallinetotest",
        initial_line_to_test
        );

    g_test_add_func(
        "/bbpathscannertest/initialmovetotest",
        initial_move_to_test
        );

    return g_test_run();
}
