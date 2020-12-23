/*
 * bschem
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
#include "bbadjustablefillstyle.h"


G_DEFINE_INTERFACE(BbAdjustableFillStyle, bb_adjustable_fill_style, G_TYPE_OBJECT)


static void
bb_adjustable_fill_style_default_init(BbAdjustableFillStyleInterface *class)
{
    g_return_if_fail(class != NULL);

    g_object_interface_install_property(
        class,
        g_param_spec_int(
            "fill-type",
            "Fill Type",
            "Fill Type",
            0,
            N_FILL_TYPES - 1,
            BB_FILL_TYPE_DEFAULT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        class,
        g_param_spec_int(
            "fill-width",
            "Fill Width",
            "Fill Width",
            10,
            G_MAXINT,
            10,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        class,
        g_param_spec_int(
            "angle-1",
            "First Fill Angle",
            "First Fill Angle",
            G_MININT,
            G_MAXINT,
            45,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        class,
        g_param_spec_int(
            "pitch-1",
            "First Fill Pitch",
            "First Fill Pitch",
            10,
            G_MAXINT,
            100,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        class,
        g_param_spec_int(
            "angle-2",
            "Second Fill Angle",
            "Second Fill Angle",
            G_MININT,
            G_MAXINT,
            135,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        class,
        g_param_spec_int(
            "pitch-2",
            "Second Fill Pitch",
            "Second Fill Pitch",
            10,
            G_MAXINT,
            100,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );
}


int
bb_adjustable_fill_style_get_fill_angle_1(BbAdjustableFillStyle *item)
{
    int angle;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "angle-1", &angle,
        NULL
        );

    return angle;
}


int
bb_adjustable_fill_style_get_fill_angle_2(BbAdjustableFillStyle *item)
{
    int angle;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "angle-2", &angle,
        NULL
        );

    return angle;
}


int
bb_adjustable_fill_style_get_fill_pitch_1(BbAdjustableFillStyle *item)
{
    int pitch;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "pitch-1", &pitch,
        NULL
        );

    return pitch;
}


int
bb_adjustable_fill_style_get_fill_pitch_2(BbAdjustableFillStyle *item)
{
    int pitch;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "pitch-2", &pitch,
        NULL
        );

    return pitch;
}


BbFillType
bb_adjustable_fill_style_get_fill_type(BbAdjustableFillStyle *item)
{
    BbFillType type;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "fill-type", &type,
        NULL
        );

    return type;
}


int
bb_adjustable_fill_style_get_fill_width(BbAdjustableFillStyle *item)
{
    int width;

    g_return_val_if_fail(item != NULL, 0);

    g_object_get(
        G_OBJECT(item),
        "fill-width", &width,
        NULL
        );

    return width;
}


void
bb_adjustable_fill_style_set_fill_angle_1(BbAdjustableFillStyle *item, int angle)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "angle-1", angle,
        NULL
        );
}


void
bb_adjustable_fill_style_set_fill_angle_2(BbAdjustableFillStyle *item, int angle)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "angle-2", angle,
        NULL
        );
}


void
bb_adjustable_fill_style_set_fill_pitch_1(BbAdjustableFillStyle *item, int pitch)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "pitch-1", pitch,
        NULL
        );
}


void
bb_adjustable_fill_style_set_fill_pitch_2(BbAdjustableFillStyle *item, int pitch)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "pitch-2", pitch,
        NULL
        );
}


void
bb_adjustable_fill_style_set_fill_type(BbAdjustableFillStyle *item, BbFillType type)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "fill-type", type,
        NULL
        );
}


void
bb_adjustable_fill_style_set_fill_width(BbAdjustableFillStyle *item, int width)
{
    g_return_if_fail(item != NULL);

    g_object_set(
        G_OBJECT(item),
        "fill-width", width,
        NULL
        );
}
