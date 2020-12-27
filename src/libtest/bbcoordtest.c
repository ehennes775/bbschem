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
#include <bbcoord.h>


void
check_rounding(void)
{
    for (int count = 0; count < 10000; count++)
    {
        double input = g_test_rand_double_range(G_MININT, G_MAXINT);

        int rounded = bb_coord_round(input);

        double delta = input - rounded;

        g_assert_cmpfloat(0.5, >=, (delta));
    }
}


void check_snap()
{
    for (int count = 0; count < 1000000; count++)
    {
        int coord = g_test_rand_int();
        int snapped = bb_coord_snap(coord, 1);

        g_assert_cmpint(coord, ==, snapped);
    }

    for (int count = 0; count < 1000000; count++)
    {
        int grid = g_test_rand_int_range(4, 10001);
        int coord = grid * g_test_rand_int_range((G_MININT + (grid / 2)) / grid, (G_MAXINT - (grid / 2)) / grid);
        int noise = g_test_rand_int_range(1, grid / 2);

        int snapped = bb_coord_snap(coord + noise, grid);
        g_assert_cmpint(coord ,==, snapped);

        snapped = bb_coord_snap(coord - noise, grid);
        g_assert_cmpint(coord, ==, snapped);
    }
}


int
main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func(
        "/bbcoordtest/checkrounding",
        check_rounding
        );

    g_test_add_func(
        "/bbcoordtest/checksnap",
        check_snap
        );

    return g_test_run();
}
