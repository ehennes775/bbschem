#ifndef __BBPATHCOMMAND__
#define __BBPATHCOMMAND__
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
#include "bbitemrenderer.h"

#define BB_TYPE_PATH_COMMAND bb_path_command_get_type()
G_DECLARE_DERIVABLE_TYPE(BbPathCommand, bb_path_command, BB, PATH_COMMAND, GObject)

struct _BbPathCommandClass
{
    GObjectClass parent_class;

    BbPathCommand* (*clone)(BbPathCommand *command);
    void (*mirror_x)(BbPathCommand *command, int cx);
    void (*mirror_y)(BbPathCommand *command, int cy);
    GSList* (*output)(BbPathCommand *command, GSList *collector);
    void (*render)(BbPathCommand *command, BbItemRenderer *renderer);
    void (*rotate)(BbPathCommand *command, int cx, int cy, int angle);
    void (*translate)(BbPathCommand *command, int dx, int dy);
};


/**
 * Create a clone of this path command
 *
 * @param command The path command to clone
 * @return The clone of the path command
 */
BbPathCommand*
bb_path_command_clone(BbPathCommand *command);


/**
 * Mirror this path command horizontally
 *
 * @param command The command to mirror
 * @param cx The x coordinate of the vertical line to mirror across
 */
void
bb_path_command_mirror_x(BbPathCommand *command, int cx);


/**
 * Mirror this path command vertically
 *
 * @param command The command to mirror
 * @param cy The y coordinate of the horizontal line to mirror across
 */
void
bb_path_command_mirror_y(BbPathCommand *command, int cy);


/**
 * Output this path command as a list of strings
 *
 * @param command The command to output
 * @param collector The list of previous output commands
 * @return The new list after prepending the command
 */
GSList*
bb_path_command_output(BbPathCommand *command, GSList *collector);


/**
 * Render this path command
 *
 * @param command The command to render
 * @param renderer
 */
void
bb_path_command_render(BbPathCommand *command, BbItemRenderer *renderer);


/**
 * Rotate the path around a given point
 *
 * Positive rotations are clockwise.
 *
 * @param command The command to rotate
 * @param cx The x coordinate of the center of rotation
 * @param cy The y coordinate of the center of rotation
 * @param angle The angle to rotate in degrees
 */
void
bb_path_command_rotate(BbPathCommand *command, int cx, int cy, int angle);


/**
 * Translate the command by a given displacement
 *
 * @param command The command to translate
 * @param dx The displacement on the x axis
 * @param dy The displacement of the y axis
 */
void
bb_path_command_translate(BbPathCommand *command, int dx, int dy);


#endif
