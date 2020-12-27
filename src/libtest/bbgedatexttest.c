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
#include <bbgedatext.h>
#include <bbtextsize.h>


void
check_alignment(BbGedaText *text_item)
{
    for (int count = 0; count < N_TEXT_ALIGNMENT; count++)
    {
        BbTextAlignment set_alignment = (BbTextAlignment) count;

        bb_geda_text_set_alignment(text_item, set_alignment);

        g_assert_cmpint(set_alignment, ==, bb_geda_text_get_alignment(text_item));

        BbTextAlignment get_alignment;
        g_object_get(text_item, "alignment", &get_alignment, NULL);
    }

    for (int count = 0; count < N_TEXT_ALIGNMENT; count++)
    {
        BbTextAlignment set_alignment = (BbTextAlignment) count;

        g_object_set(text_item, "alignment", set_alignment, NULL);

        g_assert_cmpint(set_alignment, ==, bb_geda_text_get_alignment(text_item));

        BbTextAlignment get_alignment;
        g_object_get(text_item, "alignment", &get_alignment, NULL);
    }
}


void
check_insert_x(BbGedaText *text_item)
{
    for (int count = 0; count < 10000; count++)
    {
        int set_insert_x = g_test_rand_int();

        bb_geda_text_set_insert_x(text_item, set_insert_x);

        g_assert_cmpint(set_insert_x, ==, bb_geda_text_get_insert_x(text_item));

        int get_insert_x;
        g_object_get(text_item, "insert-x", &get_insert_x, NULL);
    }

    for (int count = 0; count < 10000; count++)
    {
        int set_insert_x = g_test_rand_int();

        g_object_set(text_item, "insert-x", set_insert_x, NULL);

        g_assert_cmpint(set_insert_x, ==, bb_geda_text_get_insert_x(text_item));

        int get_insert_x;
        g_object_get(text_item, "insert-x", &get_insert_x, NULL);
    }
}


void
check_insert_y(BbGedaText *text_item)
{
    for (int count = 0; count < 10000; count++)
    {
        int set_insert_y = g_test_rand_int();

        bb_geda_text_set_insert_y(text_item, set_insert_y);

        g_assert_cmpint(set_insert_y, ==, bb_geda_text_get_insert_y(text_item));

        int get_insert_y;
        g_object_get(text_item, "insert-y", &get_insert_y, NULL);
    }

    for (int count = 0; count < 10000; count++)
    {
        int set_insert_y = g_test_rand_int();

        g_object_set(text_item, "insert-y", set_insert_y, NULL);

        g_assert_cmpint(set_insert_y, ==, bb_geda_text_get_insert_y(text_item));

        int get_insert_y;
        g_object_get(text_item, "insert-y", &get_insert_y, NULL);
    }
}


void
check_presentation(BbGedaText *text_item)
{
    for (int count = 0; count < N_TEXT_PRESENTATION; count++)
    {
        BbTextPresentation set_presentation = (BbTextPresentation) count;

        bb_geda_text_set_presentation(text_item, set_presentation);

        g_assert_cmpint(set_presentation, ==, bb_geda_text_get_presentation(text_item));

        BbTextPresentation get_presentation;
        g_object_get(text_item, "presentation", &get_presentation, NULL);
    }

    for (int count = 0; count < N_TEXT_PRESENTATION; count++)
    {
        BbTextPresentation set_presentation = (BbTextPresentation) count;

        g_object_set(text_item, "presentation", set_presentation, NULL);

        g_assert_cmpint(set_presentation, ==, bb_geda_text_get_presentation(text_item));

        BbTextPresentation get_presentation;
        g_object_get(text_item, "presentation", &get_presentation, NULL);
    }
}


void
check_rotation(BbGedaText *text_item)
{
    for (int count = 0; count < 10000; count++)
    {
        int set_rotation = g_test_rand_int();

        bb_geda_text_set_rotation(text_item, set_rotation);

        g_assert_cmpint(set_rotation, ==, bb_geda_text_get_rotation(text_item));

        int get_rotation;
        g_object_get(text_item, "rotation", &get_rotation, NULL);
    }

    for (int count = 0; count < 10000; count++)
    {
        int set_rotation = g_test_rand_int();

        g_object_set(text_item, "rotation", set_rotation, NULL);

        g_assert_cmpint(set_rotation, ==, bb_geda_text_get_rotation(text_item));

        int get_rotation;
        g_object_get(text_item, "rotation", &get_rotation, NULL);
    }
}


void
check_size(BbGedaText *text_item)
{
    for (int count = 0; count < 10000; count++)
    {
        int set_size = g_test_rand_int_range(BB_TEXT_SIZE_MIN, BB_TEXT_SIZE_MAX);

        bb_geda_text_set_size(text_item, set_size);

        g_assert_cmpint(set_size, ==, bb_geda_text_get_size(text_item));

        int get_size;
        g_object_get(text_item, "size", &get_size, NULL);
    }

    for (int count = 0; count < 10000; count++)
    {
        int set_size = g_test_rand_int_range(BB_TEXT_SIZE_MIN, BB_TEXT_SIZE_MAX);

        g_object_set(text_item, "size", set_size, NULL);

        g_assert_cmpint(set_size, ==, bb_geda_text_get_size(text_item));

        int get_size;
        g_object_get(text_item, "size", &get_size, NULL);
    }
}


void
check_visibility(BbGedaText *text_item)
{
    for (int count = 0; count < N_TEXT_VISIBILITY; count++)
    {
        BbTextVisibility set_visibility = (BbTextVisibility) count;

        bb_geda_text_set_visibility(text_item, set_visibility);

        g_assert_cmpint(set_visibility, ==, bb_geda_text_get_visibility(text_item));

        BbTextVisibility get_visibility;
        g_object_get(text_item, "visibility", &get_visibility, NULL);
    }

    for (int count = 0; count < N_TEXT_VISIBILITY; count++)
    {
        BbTextVisibility set_visibility = (BbTextVisibility) count;

        g_object_set(text_item, "visibility", set_visibility, NULL);

        g_assert_cmpint(set_visibility, ==, bb_geda_text_get_visibility(text_item));

        BbTextVisibility get_visibility;
        g_object_get(text_item, "visibility", &get_visibility, NULL);
    }
}


int
main(int argc, char *argv[])
{
    g_test_init(&argc, &argv, NULL);

    BbGedaText *text_item = g_object_ref(BB_GEDA_TEXT(g_object_new(
        BB_TYPE_GEDA_TEXT, NULL
        )));

    g_test_add_data_func(
        "/bbgedatexttest/checkalignment",
        text_item,
        (GTestDataFunc) check_alignment
        );

    g_test_add_data_func(
        "/bbgedatexttest/checkinsertx",
        text_item,
        (GTestDataFunc) check_insert_x
        );

    g_test_add_data_func(
        "/bbgedatexttest/checkinserty",
        text_item,
        (GTestDataFunc) check_insert_y
        );

    g_test_add_data_func(
        "/bbgedatexttest/checkpresentation",
        text_item,
        (GTestDataFunc) check_presentation
        );

    g_test_add_data_func(
        "/bbgedatexttest/checkrotation",
        text_item,
        (GTestDataFunc) check_rotation
        );

    g_test_add_data_func(
        "/bbgedatexttest/checksize",
        text_item,
        (GTestDataFunc) check_size
        );

    g_test_add_data_func(
        "/bbgedatexttest/checkvisibility",
        text_item,
        (GTestDataFunc) check_visibility
        );

    g_object_unref(text_item);

    return g_test_run();
}
