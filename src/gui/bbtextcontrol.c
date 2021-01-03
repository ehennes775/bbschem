/*
 * bbschem
 * Copyright (C) 2021 Edward C. Hennessy
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
#include <bbcolors.h>
#include <bbtextsize.h>
#include "bbtextcontrol.h"


G_DEFINE_INTERFACE(BbTextControl, bb_text_control, G_TYPE_OBJECT)


BbTextAlignment
bb_text_control_get_alignment(BbTextControl *text_control)
{
    g_return_val_if_fail(BB_IS_TEXT_CONTROL(text_control), BB_TEXT_ALIGNMENT_DEFAULT);

    BbTextControlInterface *iface = BB_TEXT_CONTROL_GET_IFACE(text_control);

    g_return_val_if_fail(iface != NULL, BB_TEXT_ALIGNMENT_DEFAULT);
    g_return_val_if_fail(iface->get_alignment != NULL, BB_TEXT_ALIGNMENT_DEFAULT);

    return iface->get_alignment(text_control);
}


static BbTextAlignment
bb_text_control_get_alignment_missing(BbTextControl *text_control)
{
    g_error("bb_text_control_get_rotation() not overridden");
}


int
bb_text_control_get_color(BbTextControl *text_control)
{
    g_return_val_if_fail(BB_IS_TEXT_CONTROL(text_control), BB_COLOR_GRAPHIC);

    BbTextControlInterface *iface = BB_TEXT_CONTROL_GET_IFACE(text_control);

    g_return_val_if_fail(iface != NULL, BB_COLOR_GRAPHIC);
    g_return_val_if_fail(iface->get_color != NULL, BB_COLOR_GRAPHIC);

    return iface->get_color(text_control);
}


static int
bb_text_control_get_color_missing(BbTextControl *text_control)
{
    g_error("bb_text_control_get_rotation() not overridden");
}


BbTextPresentation
bb_text_control_get_presentation(BbTextControl *text_control)
{
    g_return_val_if_fail(BB_IS_TEXT_CONTROL(text_control), BB_TEXT_PRESENTATION_DEFAULT);

    BbTextControlInterface *iface = BB_TEXT_CONTROL_GET_IFACE(text_control);

    g_return_val_if_fail(iface != NULL, BB_TEXT_PRESENTATION_DEFAULT);
    g_return_val_if_fail(iface->get_presentation != NULL, BB_TEXT_PRESENTATION_DEFAULT);

    return iface->get_presentation(text_control);
}


static BbTextPresentation
bb_text_control_get_presentation_missing(BbTextControl *text_control)
{
    g_error("bb_text_control_get_rotation() not overridden");
}


int
bb_text_control_get_rotation(BbTextControl *text_control)
{
    g_return_val_if_fail(BB_IS_TEXT_CONTROL(text_control), 0);

    BbTextControlInterface *iface = BB_TEXT_CONTROL_GET_IFACE(text_control);

    g_return_val_if_fail(iface != NULL, 0);
    g_return_val_if_fail(iface->get_rotation != NULL, 0);

    return iface->get_rotation(text_control);
}


static int
bb_text_control_get_rotation_missing(BbTextControl *text_control)
{
    g_error("bb_text_control_get_rotation() not overridden");
}


int
bb_text_control_get_size(BbTextControl *text_control)
{
    g_return_val_if_fail(BB_IS_TEXT_CONTROL(text_control), BB_TEXT_SIZE_DEFAULT);

    BbTextControlInterface *iface = BB_TEXT_CONTROL_GET_IFACE(text_control);

    g_return_val_if_fail(iface != NULL, BB_TEXT_SIZE_DEFAULT);
    g_return_val_if_fail(iface->get_size != NULL, BB_TEXT_SIZE_DEFAULT);

    return iface->get_size(text_control);
}


static int
bb_text_control_get_size_missing(BbTextControl *text_control)
{
    g_error("bb_text_control_get_size() not overridden");
}


const gchar*
bb_text_control_get_text(BbTextControl *text_control)
{
    g_return_val_if_fail(BB_IS_TEXT_CONTROL(text_control), NULL);

    BbTextControlInterface *iface = BB_TEXT_CONTROL_GET_IFACE(text_control);

    g_return_val_if_fail(iface != NULL, NULL);
    g_return_val_if_fail(iface->get_text != NULL, NULL);

    return iface->get_text(text_control);
}



static const gchar*
bb_text_control_get_text_missing(BbTextControl *text_control)
{
    g_error("bb_text_control_get_text() not overridden");
}


BbTextVisibility
bb_text_control_get_visibility(BbTextControl *text_control)
{
    g_return_val_if_fail(BB_IS_TEXT_CONTROL(text_control), BB_TEXT_VISIBILITY_DEFAULT);

    BbTextControlInterface *iface = BB_TEXT_CONTROL_GET_IFACE(text_control);

    g_return_val_if_fail(iface != NULL, BB_TEXT_VISIBILITY_DEFAULT);
    g_return_val_if_fail(iface->get_visibility != NULL, BB_TEXT_VISIBILITY_DEFAULT);

    return iface->get_visibility(text_control);
}



static BbTextVisibility
bb_text_control_get_visibility_missing(BbTextControl *text_control)
{
    g_error("bb_text_control_get_visibility() not overridden");
}


static void
bb_text_control_default_init(BbTextControlInterface *iface)
{
    g_return_if_fail(iface != NULL);

    iface->get_alignment = bb_text_control_get_alignment_missing;
    iface->get_color = bb_text_control_get_color_missing;
    iface->get_presentation = bb_text_control_get_presentation_missing;
    iface->get_rotation = bb_text_control_get_rotation_missing;
    iface->get_size = bb_text_control_get_size_missing;
    iface->get_text = bb_text_control_get_text_missing;
    iface->get_visibility = bb_text_control_get_visibility_missing;

    g_object_interface_install_property(
        iface,
        g_param_spec_int(
            "alignment",
            "",
            "",
            0,
            N_TEXT_ALIGNMENT - 1,
            BB_TEXT_ALIGNMENT_DEFAULT,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_int(
            "color",
            "",
            "",
            0,
            G_MAXINT,
            BB_COLOR_GRAPHIC,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_int(
            "presentation",
            "",
            "",
            0,
            N_TEXT_PRESENTATION - 1,
            BB_TEXT_PRESENTATION_DEFAULT,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_int(
            "rotation",
            "",
            "",
            G_MININT,
            G_MAXINT,
            0,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_int(
            "size",
            "",
            "",
            BB_TEXT_SIZE_MIN,
            BB_TEXT_SIZE_MAX,
            BB_TEXT_SIZE_DEFAULT,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_string(
            "text",
            "",
            "",
            NULL,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );

    g_object_interface_install_property(
        iface,
        g_param_spec_int(
            "visibility",
            "",
            "",
            0,
            N_TEXT_VISIBILITY - 1,
            BB_TEXT_VISIBILITY_DEFAULT,
            G_PARAM_READABLE | G_PARAM_STATIC_STRINGS
            )
        );
}
