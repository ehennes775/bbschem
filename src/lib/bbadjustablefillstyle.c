/*
 * bbsch
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
            G_MAXINT,
            0,
            G_PARAM_READWRITE
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
            G_PARAM_READWRITE
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
            G_PARAM_READWRITE
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
            G_PARAM_READWRITE
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
            G_PARAM_READWRITE
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
            G_PARAM_READWRITE
            )
        );
}
