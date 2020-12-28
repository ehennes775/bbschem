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
#include "bbadjustablefillstyle.h"
#include "bbadjustableitemcolor.h"
#include "bbadjustablelinestyle.h"
#include "bbschematic.h"
#include "bbgedaitem.h"
#include "bbgedaline.h"
#include "bbapplyfunc.h"
#include "bblibrary.h"
#include "bbattribute.h"
#include "bbelectrical.h"


enum
{
    PROP_0,
    PROP_CAIRO,
    PROP_2,
    PROP_3,
    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE_ITEM,
    N_SIGNALS
};


struct _BbSchematic
{
    GObject parent;

    GSList *items;
};


G_DEFINE_TYPE(BbSchematic, bb_schematic, G_TYPE_OBJECT)

typedef struct _ApplyItemPropertyCapture ApplyItemPropertyCapture;

struct _ApplyItemPropertyCapture
{
    const char *name;
    const GValue *value;
};

typedef struct _AsyncWriteData AsyncWriteData;

struct _AsyncWriteData
{
    GOutputStream *stream;
    int io_priority;
    GSList *item;
};


typedef struct _RenderCapture RenderCapture;

struct _RenderCapture
{
    BbItemRenderer *renderer;
};


typedef struct _WriteCapture WriteCapture;

struct _WriteCapture
{
    GOutputStream *stream;
    GCancellable *cancellable;
    GError **error;
};

static void
bb_schematic_add_items_lambda(BbGedaItem *item, BbSchematic *schematic);

static void
bb_schematic_apply_item_property_lambda(BbGedaItem *item, ApplyItemPropertyCapture *capture);

static void
bb_schematic_dispose(GObject *object);

static void
bb_schematic_finalize(GObject *object);

static void
bb_schematic_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_schematic_invalidate_item_cb(BbGedaItem *item, BbSchematic *schematic);

static void
bb_schematic_render_lambda_1(BbGedaItem *item, RenderCapture *capture);

static void
bb_schematic_render_lambda_2(BbGedaItem *item, RenderCapture *capture);

static void
bb_schematic_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_schematic_write_callback(GObject *source, GAsyncResult *result, gpointer callback_data);

static void
bb_schematic_write_lambda(BbGedaItem *item, WriteCapture *capture);

static AsyncWriteData*
bb_schematic_async_write_data_new();

static void
bb_schematic_async_write_data_free(gpointer slice);


static void
bb_schematic_modify_fill_angle_1_lambda(
    gpointer data,
    gpointer user_data
    );


static void
bb_schematic_modify_fill_angle_2_lambda(
    gpointer data,
    gpointer user_data
    );


static void
bb_schematic_modify_fill_pitch_1_lambda(
    gpointer data,
    gpointer user_data
    );


static void
bb_schematic_modify_fill_pitch_2_lambda(
    gpointer data,
    gpointer user_data
    );


static void
bb_schematic_modify_fill_type_lambda(
    gpointer data,
    gpointer user_data
    );


static void
bb_schematic_modify_fill_width_lambda(
    gpointer data,
    gpointer user_data
    );


static void
bb_schematic_modify_item_color_lambda(
    gpointer data,
    gpointer user_data
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


void
bb_schematic_add_item(BbSchematic *schematic, BbGedaItem *item)
{
    g_return_if_fail(BB_IS_SCHEMATIC(schematic));
    g_return_if_fail(BB_IS_GEDA_ITEM(item));

    GSList *items = g_slist_append(NULL, item);

    bb_schematic_add_items(schematic, items);
}


void
bb_schematic_add_items(BbSchematic *schematic, GSList *items)
{
    g_return_if_fail(BB_IS_SCHEMATIC(schematic));

    g_slist_foreach(items, (GFunc) bb_schematic_add_items_lambda, schematic);

    schematic->items = g_slist_concat(schematic->items, items);
}


static void
bb_schematic_add_items_lambda(BbGedaItem *item, BbSchematic *schematic)
{
    g_return_if_fail(BB_IS_SCHEMATIC(schematic));
    g_return_if_fail(BB_IS_GEDA_ITEM(item));

    g_object_ref(item);

    g_signal_connect(
        item,
        "invalidate-item",
        G_CALLBACK(bb_schematic_invalidate_item_cb),
        schematic
        );
}


void
bb_schematic_apply_item_property(BbSchematic *schematic, const char *name, const GValue *value)
{
    ApplyItemPropertyCapture capture;

    capture.name = name;
    capture.value = value;

    g_message("ApplyProperty = %s", capture.name);

    bb_schematic_foreach(schematic, (GFunc) bb_schematic_apply_item_property_lambda, &capture);
}


static void
bb_schematic_apply_item_property_lambda(BbGedaItem *item, ApplyItemPropertyCapture *capture)
{
    BbGedaItemClass *class = BB_GEDA_ITEM_GET_CLASS(item);
    g_return_if_fail(class != NULL);

    GParamSpec *param = g_object_class_find_property(G_OBJECT_CLASS(class), capture->name);

    if (param != NULL)
    {
        g_object_set_property(G_OBJECT(item), capture->name, capture->value);
    }
}


void
bb_schematic_calculate_bounds(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    BbBoundsCalculator *calculator,
    BbBounds *bounds
    )
{
    GSList *iter = schematic->items;

    while (iter != NULL)
    {
        if (where_pred(iter->data , where_user_data))
        {
            BbBounds *temp = bb_geda_item_calculate_bounds(iter->data, calculator);

            bb_bounds_union(bounds, bounds, temp);

            bb_bounds_free(temp);
        }

        iter = g_slist_next(iter);
    }
}

static void
bb_schematic_class_init(BbSchematicClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(G_IS_OBJECT_CLASS(object_class));

    object_class->dispose = bb_schematic_dispose;
    object_class->finalize = bb_schematic_finalize;
    object_class->get_property = bb_schematic_get_property;
    object_class->set_property = bb_schematic_set_property;

    signals[SIG_INVALIDATE_ITEM] = g_signal_new(
        "invalidate-item",
        BB_TYPE_SCHEMATIC,
        0,
        0,
        NULL,
        NULL,
        g_cclosure_marshal_VOID__OBJECT,
        G_TYPE_NONE,
        1,
        BB_TYPE_GEDA_ITEM
        );
}


static void
bb_schematic_dispose(GObject *object)
{
}


static void
bb_schematic_finalize(GObject *object)
{
}


void
bb_schematic_foreach(BbSchematic *schematic, GFunc func, gpointer user_data)
{
    g_return_if_fail(schematic != NULL);

    g_slist_foreach(schematic->items, func, user_data);
}


void
bb_schematic_foreach_query(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    BbQueryFunc query_func,
    gpointer query_user_data
    )
{
    GSList *iter;

    g_return_if_fail(schematic != NULL);
    g_return_if_fail(where_pred != NULL);
    g_return_if_fail(query_func != NULL);

    iter = schematic->items;

    while (iter != NULL)
    {
        if (where_pred(iter->data , where_user_data))
        {
            if (!query_func(iter->data , query_user_data))
            {
                break;
            }
        }

        iter = g_slist_next(iter);
    }
}


void
bb_schematic_foreach_modify(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data,
    BbApplyFunc modify_func,
    gpointer modify_user_data
    )
{
    GSList *iter;

    g_return_if_fail(schematic != NULL);
    g_return_if_fail(where_pred != NULL);
    g_return_if_fail(modify_func != NULL);

    iter = schematic->items;

    while (iter != NULL)
    {
        if (where_pred(iter->data , where_user_data))
        {
            modify_func(iter->data , modify_user_data);
        }

        iter = g_slist_next(iter);
    }
}


void
bb_schematic_foreach_remove(
    BbSchematic *schematic,
    BbPred where_pred,
    gpointer where_user_data
    )
{
    // TODO
}


static void
bb_schematic_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
bb_schematic_init(BbSchematic *schematic)
{
    BbGedaBox *box = g_object_new(
        BB_TYPE_GEDA_BOX,
        "x0", 0,
        "y0", 0,
        "x1", 100,
        "y1", 100,
        NULL
        );

    schematic->items = g_slist_append(NULL, box);
}


static void
bb_schematic_invalidate_item_cb(BbGedaItem *item, BbSchematic *schematic)
{
    g_return_if_fail(BB_IS_SCHEMATIC(schematic));

    g_message("invalidate-item");

    g_signal_emit(schematic, signals[SIG_INVALIDATE_ITEM], 0, item);
}


BbSchematic*
bb_schematic_new()
{
    return g_object_new(BB_TYPE_SCHEMATIC, NULL);
}


void
bb_schematic_render(
    BbSchematic *schematic,
    BbItemRenderer *renderer
    )
{
    RenderCapture capture;

    capture.renderer = renderer;

    g_slist_foreach(
        schematic->items,
        (GFunc) bb_schematic_render_lambda_1,
        &capture
        );

    g_slist_foreach(
        schematic->items,
        (GFunc) bb_schematic_render_lambda_2,
        &capture
        );
}


static void
bb_schematic_render_lambda_1(BbGedaItem *item, RenderCapture *capture)
{
    g_return_if_fail(BB_IS_GEDA_ITEM(item));
    g_return_if_fail(capture != NULL);

    if (BB_IS_ELECTRICAL(item))
    {
        bb_electrical_foreach(
            BB_ELECTRICAL(item),
            bb_schematic_render_lambda_2,
            capture
            );
    }
}


static void
bb_schematic_render_lambda_2(BbGedaItem *item, RenderCapture *capture)
{
    g_return_if_fail(BB_IS_GEDA_ITEM(item));
    g_return_if_fail(capture != NULL);

    bb_geda_item_render(item, BB_ITEM_RENDERER(capture->renderer));
}


static void
bb_schematic_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_CAIRO:
            break;

        case PROP_2:
            break;

        case PROP_3:
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


gboolean
bb_schematic_write(
    BbSchematic *schematic,
    GOutputStream *stream,
    GCancellable *cancellable,
    GError **error
    )
{
    WriteCapture capture;

    capture.stream = stream;
    capture.cancellable = cancellable;
    capture.error = error;

    g_slist_foreach(
        schematic->items,
        (GFunc) bb_schematic_write_lambda,
        &capture
        );
}


static void
bb_schematic_write_lambda(BbGedaItem *item, WriteCapture *capture)
{
    bb_geda_item_write(
        item,
        capture->stream,
        capture->cancellable,
        capture->error
    );
}


void
bb_schematic_write_async(
    BbSchematic *schematic,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    GTask *task = g_task_new(schematic, cancellable, callback, callback_data);

    if (schematic->items != NULL)
    {
        AsyncWriteData *data = bb_schematic_async_write_data_new();
        g_task_set_task_data(task, data, bb_schematic_async_write_data_free);

        data->stream = stream;
        data->io_priority = io_priority;
        data->item = schematic->items;

        bb_geda_item_write_async(
            BB_GEDA_ITEM(data->item->data),
            stream,
            io_priority,
            g_task_get_cancellable(task),
            bb_schematic_write_callback,
            task
        );
    }
    else
    {
        g_task_return_boolean(task, TRUE);
    }
}


void
bb_schematic_write_finish(
    BbSchematic *schematic,
    GAsyncResult *result,
    GError **error
    )
{
    g_return_if_fail(g_task_is_valid(result, schematic));

    g_task_propagate_pointer(G_TASK(result), error);
}


static void
bb_schematic_write_callback(GObject *source, GAsyncResult *result, gpointer callback_data)
{
    GError *error = NULL;
    GTask *task = G_TASK(callback_data);
    AsyncWriteData *data = g_task_get_task_data(task);

    bb_geda_item_write_finish(
        BB_GEDA_ITEM(data->item->data),
        data->stream,
        result,
        &error
    );

    if (error != NULL)
    {
        data->item = data->item->next;

        if (data->item != NULL)
        {
            bb_geda_item_write_async(
                BB_GEDA_ITEM(data->item->data),
                data->stream,
                data->io_priority,
                g_task_get_cancellable(G_TASK(result)),
                bb_schematic_write_callback,
                task
            );
        }
        else
        {
            g_task_return_boolean(task, TRUE);
        }
    }
    else
    {
        g_task_return_error(G_TASK(result), error);
    }
}


static AsyncWriteData*
bb_schematic_async_write_data_new()
{
    return g_slice_new(AsyncWriteData);
}


static void
bb_schematic_async_write_data_free(gpointer slice)
{
    g_slice_free(AsyncWriteData, slice);
}