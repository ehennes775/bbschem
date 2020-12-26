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
#include "bbangle.h"


void
check_calc_sweep()
{
    for (int count = 0; count < 1000; count++)
    {
        int a0 = g_test_rand_int();
        int a1 = g_test_rand_int();

        int sweep = bb_angle_calculate_sweep(a0, a1);

        g_assert_cmpint(0, <, sweep);
        g_assert_cmpint(360, >=, sweep);
    }
}


void check_convert()
{
    for (int count = 0; count < 10000; count++)
    {
        int original_angle = g_test_rand_int();

        double radians = bb_angle_to_radians(original_angle);
        double calculated_angle = bb_angle_from_radians(radians);

        g_assert_cmpfloat(original_angle, ==, calculated_angle);
    }

    g_assert_cmpfloat(0, ==, bb_angle_from_radians(0.0));

    g_assert_cmpfloat(-180, ==, bb_angle_from_radians(-M_PI));

    g_assert_cmpfloat(180, ==, bb_angle_from_radians(M_PI));

    g_assert_cmpfloat(360, ==, bb_angle_from_radians(2.0 * M_PI));
}


void check_is_normal()
{
    for (int count = 0; count < 10000; count++)
    {
        int angle = g_test_rand_int_range(G_MININT, 0);

        g_assert_false(bb_angle_is_normal(angle));
    }

    for (int count = 0; count < 10000; count++)
    {
        int angle = g_test_rand_int_range(360, G_MAXINT);

        g_assert_false(bb_angle_is_normal(angle));
    }

    for (int count = 0; count < 100; count++)
    {
        int angle = g_test_rand_int_range(0, 360);

        g_assert_true(bb_angle_is_normal(angle));
    }
}


void check_is_orthogonal()
{
    for (int count = 0; count < 1000000; count++)
    {
        int angle = 90 * g_test_rand_int_range(G_MININT / 90, G_MAXINT / 90);
        int noise = g_test_rand_int_range(1, 90);

        g_assert_true(bb_angle_is_orthogonal(angle));
        g_assert_false(bb_angle_is_orthogonal(angle + noise));
    }
}


void check_normalize()
{
    for (int count = 0; count < 1000000; count++)
    {
        int angle = g_test_rand_int_range(0, 360);
        int noise = 360 * g_test_rand_int_range(G_MININT / 360, G_MAXINT / 360);

        int normalized = bb_angle_normalize(angle + noise);

        g_assert_cmpint(0, <=, normalized);
        g_assert_cmpint(360, >, normalized);
        g_assert_cmpint(angle, == ,normalized);
    }
}


void check_make_orthogonal()
{
    for (int count = 0; count < 1000000; count++)
    {
        int angle = 90 * g_test_rand_int_range((G_MININT + 45) / 90, G_MAXINT / 90);
        int noise = g_test_rand_int_range(1, 45);

        int normalized = bb_angle_make_orthogonal(angle + noise);
        g_assert_cmpint(angle, ==, normalized);

        normalized = bb_angle_make_orthogonal(angle - noise);
        g_assert_cmpint(angle, ==, normalized);
    }
}


int
main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);

    g_test_add_func(
        "/bbangletest/checkcalcsweep",
        check_calc_sweep
        );

    g_test_add_func(
        "/bbangletest/check_convert",
        check_convert
        );

    g_test_add_func(
        "/bbangletest/checkisnormal",
        check_is_normal
        );

    g_test_add_func(
        "/bbangletest/checkisorthogonal",
        check_is_orthogonal
        );

    g_test_add_func(
        "/bbangletest/checknormalize",
        check_normalize
        );

    g_test_add_func(
        "/bbangletest/checkmakeorthogonal",
        check_make_orthogonal
        );

    return g_test_run();
}
