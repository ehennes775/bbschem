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
#include "bbhatch.h"
#include "bbpoint.h"
#include "bbbounds.h"
#include "bbline.h"
#include "bbangle.h"

typedef struct _BbSweepStatus BbSweepStatus;

struct _BbSweepStatus
{
    double x;
    double y1;
    double m1;
    double b1;
};


typedef struct _BbSweepEvent BbSweepEvent;

struct _BbSweepEvent
{
    BbSweepStatus status;
    int y0;
};


void
bb_hatch_box(int x[2], int y[2], gint angle, gint pitch, GArray *lines)
{
    g_return_if_fail(lines != NULL);

    GArray *corners = g_array_sized_new(FALSE, FALSE, sizeof(BbPoint), 4);

    BbPoint point;

    point.x = x[0];
    point.y = y[0];
    g_array_append_val(corners, point);

    point.x = x[1];
    point.y = y[0];
    g_array_append_val(corners, point);

    point.x = x[1];
    point.y = y[1];
    g_array_append_val(corners, point);

    point.x = x[0];
    point.y = y[1];
    g_array_append_val(corners, point);

    bb_hatch_polygon(corners, angle, pitch, lines);

    g_array_free(corners, TRUE);
}


static double
bb_hatch_calculate_initial_sweep(int pitch, double min_y, double max_y)
{
    double delta = max_y - min_y;

    return min_y + ((delta - (floor((delta - pitch) / pitch) * pitch)) / 2.0);
}


static int
bb_hatch_compare_sweep_events(BbSweepEvent *a, BbSweepEvent *b)
{
    return a->y0 - b->y0;
}


static int
bb_hatch_compare_sweep_statuses(BbSweepStatus *a, BbSweepStatus *b)
{
    return a->x - b->x;
}


void
bb_hatch_polygon(GArray *points, gint angle, gint pitch, GArray *lines)
{
    g_return_if_fail(points != NULL);
    g_return_if_fail(pitch > 0);
    g_return_if_fail(lines != NULL);

    GArray *events = g_array_new(FALSE, FALSE, sizeof(BbSweepEvent));
    GArray *points2 = g_array_sized_new(FALSE, FALSE, sizeof(BbPoint), points->len);
    GArray *status = g_array_new(FALSE, FALSE, sizeof(BbSweepStatus));

    BbMatrix transform;

    bb_matrix_init(&transform);
    bb_matrix_rotate(&transform, bb_angle_to_radians(-angle));

    g_array_append_vals(points2, points->data, points->len);
    bb_point_transform_array(points2->len, &g_array_index(points2, BbPoint, 0), &transform);

    if (points2->len > 1)
    {
        BbPoint *p0 = &g_array_index(points2, BbPoint, points2->len-1);

        for (int index = 0; index < points2->len; index++)
        {
            BbPoint *p1 = &g_array_index(points2, BbPoint, index);

            if ( p0->y != p1->y )
            {
                BbSweepEvent event =
                {
                    .y0 = MIN(p0->y, p1->y),
                    .status.y1 = MAX(p0->y, p1->y),
                    .status.m1 = (gdouble)( p1->x - p0->x ) / (gdouble)( p1->y - p0->y ),
                    .status.b1 = p0->x - event.status.m1 * p0->y
                };

                g_array_append_val(events, event);
            }

            p0 = p1;
        }
    }

    g_array_sort(events, (GCompareFunc) bb_hatch_compare_sweep_events);

    BbBounds bounds =
    {
        .min_x = G_MAXINT,
        .min_y = G_MAXINT,
        .max_x = G_MININT,
        .max_y = G_MININT
    };

    BbMatrix inverse;
    bb_matrix_init(&inverse);
    bb_matrix_rotate(&inverse, bb_angle_to_radians(angle));

    bb_points_calculate_bounds(points2->len, (BbPoint*) points2->data, &bounds);
    double sweep_y = bb_hatch_calculate_initial_sweep(pitch, bounds.min_y, bounds.max_y);

    while (events->len > 0 || status->len > 0)
    {
        int index = 0;

        while (index < events->len)
        {
            BbSweepEvent *event = &g_array_index(events, BbSweepEvent , index);

            if (sweep_y >= event->y0)
            {
                BbSweepStatus st = event->status;

                g_array_append_val(status, st);
                g_array_remove_index(events, index);
            }
            else
            {
                index++;
            }
        }

        for (index = status->len - 1; index >= 0; index--)
        {
            BbSweepStatus *st = &g_array_index(status, BbSweepStatus, index);

            if (sweep_y >= st->y1)
            {
                g_array_remove_index_fast(status, index);
            }
        }

        for (index = 0; index < status->len; index++)
        {
            BbSweepStatus *st = &g_array_index(status, BbSweepStatus, index);

            st->x = st->m1 * sweep_y + st->b1;
        }

        g_array_sort(status, (GCompareFunc) bb_hatch_compare_sweep_statuses);

        for (index = 0; index+1 < status->len; index += 2)
        {
            BbLine line =
            {
                .x[0] = g_array_index(status, BbSweepStatus, index).x,
                .y[0] = sweep_y,
                .x[1] = g_array_index(status, BbSweepStatus, index+1).x,
                .y[1] = sweep_y
            };

            bb_line_transform(&line, &inverse);

            g_array_append_val(lines, line);
        }

        sweep_y += pitch;
    }

    g_array_free(events, TRUE);
    g_array_free(points2, TRUE);
    g_array_free(status, TRUE);
}
