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
#include <bbextensions.h>
#include "bbcoord.h"
#include "bbitemparams.h"
#include "bbgedablock.h"
#include "bbadjustableitemcolor.h"
#include "bbparams.h"
#include "bberror.h"


/**
 * The positions of parameters in the file format
 */
enum
{
    PARAM_TOKEN,
    PARAM_INSERT_X,
    PARAM_INSERT_Y,
    PARAM_SELECTABLE,
    PARAM_ROTATION,
    PARAM_MIRROR,
    PARAM_NAME,
    N_PARAMETERS
};


enum
{
    PROP_0,
    PROP_INSERT_X,
    PROP_INSERT_Y,
    PROP_SELECTABLE,
    PROP_ROTATION,
    PROP_MIRROR,
    PROP_NAME,
    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbGedaBlock
{
    BbGedaItem parent;

    /**
     * The x coordinate of the insertion point
     */
    int insert_x;

    /**
     * The y coordinate of the insertion point
     */
    int insert_y;

    /**
     * Indicates the instantiated symbol is selectable.
     */
    gboolean selectable;

    /**
     * The rotation angle of the symbol, in degrees
     */
    int rotation;

    /**
     * Indicates to mirror the symbol on the x coordinate of the insertion point.
     */
    gboolean mirror;

    /**
     * The basename of the symbol file (e.g. diode-1.sym)
     */
    gchar *name;
};


static BbBounds*
bb_geda_block_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator);

static BbGedaItem*
bb_geda_block_clone(BbGedaItem *item);

static void
bb_geda_block_dispose(GObject *object);

static void
bb_geda_block_finalize(GObject *object);

static GRegex*
bb_geda_block_get_name_regex();

static void
bb_geda_block_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
bb_geda_block_render(BbGedaItem *item, BbItemRenderer *renderer);

static void
bb_geda_block_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);

static void
bb_geda_block_translate(BbGedaItem *item, int dx, int dy);

static gboolean
bb_geda_block_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error);

static void
bb_geda_block_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_block_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE(
    BbGedaBlock,
    bb_geda_block,
    BB_TYPE_GEDA_ITEM
    )


static BbBounds*
bb_geda_block_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaBlock *block = BB_GEDA_BLOCK(item);

    g_return_val_if_fail(block != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        block->insert_x,
        block->insert_y,
        block->insert_x,
        block->insert_y,
        0
        );
}


static void
bb_geda_block_class_init(BbGedaBlockClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_geda_block_dispose;
    object_class->finalize = bb_geda_block_finalize;
    object_class->get_property = bb_geda_block_get_property;
    object_class->set_property = bb_geda_block_set_property;

    BB_GEDA_ITEM_CLASS(klasse)->calculate_bounds = bb_geda_block_calculate_bounds;
    BB_GEDA_ITEM_CLASS(klasse)->clone = bb_geda_block_clone;
    BB_GEDA_ITEM_CLASS(klasse)->render = bb_geda_block_render;
    BB_GEDA_ITEM_CLASS(klasse)->translate = bb_geda_block_translate;
    BB_GEDA_ITEM_CLASS(klasse)->write = bb_geda_block_write;
    BB_GEDA_ITEM_CLASS(klasse)->write_async = bb_geda_block_write_async;
    BB_GEDA_ITEM_CLASS(klasse)->write_finish = bb_geda_block_write_finish;

    bb_object_class_install_property(
        object_class,
        PROP_INSERT_X,
        properties[PROP_INSERT_X] = g_param_spec_int(
            "insert-x",
            "Insertion Point X",
            "The x coordinate of the insertion point",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    bb_object_class_install_property(
        object_class,
        PROP_INSERT_Y,
        properties[PROP_INSERT_Y] = g_param_spec_int(
            "insert-y",
            "Insertion Point Y",
            "The y coordinate of the insertion point",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_SELECTABLE] = bb_object_class_install_property(
        object_class,
        PROP_SELECTABLE,
        g_param_spec_boolean(
            "selectable",
            "",
            "The item is selectable (i.e. not locked)",
            TRUE,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_ROTATION] = bb_object_class_install_property(
        object_class,
        PROP_ROTATION,
        g_param_spec_int(
            "rotation",
            "The rotation angle of the symbol",
            "The rotation angle of the symbol",
            INT_MIN,
            INT_MAX,
            0,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_MIRROR] = bb_object_class_install_property(
        object_class,
        PROP_MIRROR,
        g_param_spec_boolean(
            "mirror",
            "",
            "",
            FALSE,
            G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS
            )
        );

    /*
     * The name is construct only. Changing the symbol after component instantiation will likely have problems with
     * incorrect inherited attributes. The preferred mechanism is to completely delete the component and
     * re-instantiate.
     */
    properties[PROP_NAME] = bb_object_class_install_property(
        object_class,
        PROP_NAME,
        g_param_spec_string(
            "name",
            "The filename of the symbol",
            "",
            "",
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS
            )
        );

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_GEDA_ITEM);
}


static BbGedaItem*
bb_geda_block_clone(BbGedaItem *item)
{
    return BB_GEDA_ITEM(g_object_new(
        BB_TYPE_GEDA_BLOCK,
        "insert-x", bb_geda_block_get_insert_x(BB_GEDA_BLOCK(item)),
        "insert-y", bb_geda_block_get_insert_y(BB_GEDA_BLOCK(item)),

        "selectable", bb_geda_block_get_selectable(BB_GEDA_BLOCK(item)),

        "rotation", bb_geda_block_get_rotation(BB_GEDA_BLOCK(item)),
        "mirror", bb_geda_block_get_mirror(BB_GEDA_BLOCK(item)),

        "name", bb_geda_block_get_name(BB_GEDA_BLOCK(item)),

        NULL
        ));
}


static void
bb_geda_block_dispose(GObject *object)
{
}


static void
bb_geda_block_finalize(GObject *object)
{
}


int
bb_geda_block_get_insert_x(BbGedaBlock *block)
{
    g_return_val_if_fail(block != NULL, 0);

    return block->insert_x;
}


int
bb_geda_block_get_insert_y(BbGedaBlock *block)
{
    g_return_val_if_fail(BB_IS_GEDA_BLOCK(block), 0);

    return block->insert_y;
}


gboolean
bb_geda_block_get_mirror(BbGedaBlock *block)
{
    g_return_val_if_fail(BB_IS_GEDA_BLOCK(block), FALSE);

    return block->mirror;
}


const gchar*
bb_geda_block_get_name(BbGedaBlock *block)
{
    g_return_val_if_fail(BB_IS_GEDA_BLOCK(block), NULL);

    return block->name;
}


/**
 * @brief Provides a regular expression to extract data from the block name
 *
 * The function gets called before the class initializer
 *
 * Group name | Description
 * ---------- | -----------
 * embedded   | The optional EMBEDDED prefix
 * basename   | The basename of the block
 */
static GRegex*
bb_geda_block_get_name_regex()
{
    static gsize done = 0;
    static GRegex *regex = NULL;

    if (g_once_init_enter(&done))
    {
        GError *local_error = NULL;

        regex = g_regex_new(
            "(?<embedded>EMBEDDED)?(?<basename>.+)",
            0,
            0,
            &local_error
            );

        g_assert(regex != NULL);
        g_assert(local_error == NULL);

        g_once_init_leave(&done, 1);
    }

    return regex;
}


static void
bb_geda_block_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_INSERT_X:
            g_value_set_int(value, bb_geda_block_get_insert_x(BB_GEDA_BLOCK(object)));
            break;

        case PROP_INSERT_Y:
            g_value_set_int(value, bb_geda_block_get_insert_y(BB_GEDA_BLOCK(object)));
            break;

        case PROP_SELECTABLE:
            g_value_set_boolean(value, bb_geda_block_get_selectable(BB_GEDA_BLOCK(object)));
            break;

        case PROP_ROTATION:
            g_value_set_int(value, bb_geda_block_get_rotation(BB_GEDA_BLOCK(object)));
            break;

        case PROP_MIRROR:
            g_value_set_boolean(value, bb_geda_block_get_mirror(BB_GEDA_BLOCK(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, bb_geda_block_get_name(BB_GEDA_BLOCK(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


int
bb_geda_block_get_rotation(BbGedaBlock *block)
{
    g_return_val_if_fail(BB_IS_GEDA_BLOCK(block), 0);

    return block->rotation;
}


gboolean
bb_geda_block_get_selectable(BbGedaBlock *block)
{
    g_return_val_if_fail(BB_IS_GEDA_BLOCK(block), TRUE);

    return block->selectable;
}


static void
bb_geda_block_init(BbGedaBlock *block)
{
    g_return_if_fail(BB_IS_GEDA_BLOCK(block));
}


BbGedaBlock*
bb_geda_block_new_with_params(BbParams *params, GError **error)
{
    g_return_val_if_fail(bb_params_token_matches(params, BB_GEDA_BLOCK_TOKEN), NULL);

    const gchar *basename = NULL;
    BbGedaBlock *block = NULL;
    gboolean embedded = FALSE;
    GError *local_error = NULL;
    GMatchInfo *match_info = NULL;

    gboolean success = g_regex_match(
        bb_geda_block_get_name_regex(),
        bb_params_get_string(params, PARAM_NAME, &local_error),
        0,
        &match_info
        );

    if ((local_error == NULL) && !success)
    {
        local_error = g_error_new(BB_ERROR_DOMAIN, 0, "Ouch");
    }

    if (local_error == NULL)
    {
        gint position;
        gboolean success2;

        success2 = g_match_info_fetch_named_pos(match_info, "embedded", &position, NULL);

        embedded = success2 && (position >= 0);

        if (embedded)
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_NOT_SUPPORTED,
                "Embedded blocks are not supported"
                );
        }
    }

    if (local_error == NULL)
    {
        basename = g_match_info_fetch_named(match_info, "basename");

        if (basename == NULL)
        {
            local_error = g_error_new(BB_ERROR_DOMAIN, 0, "Ouch");
        }
    }

    if (local_error == NULL)
    {
        GError *param_error[N_PARAMETERS] = { NULL };

        block = BB_GEDA_BLOCK(g_object_new(
            BB_TYPE_GEDA_BLOCK,
            "insert-x", bb_params_get_int(params, PARAM_INSERT_X, &param_error[PARAM_INSERT_X]),
            "insert-y", bb_params_get_int(params, PARAM_INSERT_Y, &param_error[PARAM_INSERT_Y]),

            "selectable", bb_params_get_int(params, PARAM_SELECTABLE, &param_error[PARAM_SELECTABLE]),

            "rotation", bb_params_get_int(params, PARAM_ROTATION, &param_error[PARAM_ROTATION]),
            "mirror", bb_params_get_int(params, PARAM_MIRROR, &param_error[PARAM_MIRROR]),

            "name", basename,

            NULL
            ));

        for (int index=0; index < N_PARAMETERS; index++)
        {
            if (param_error[index] != NULL)
            {
                g_propagate_error(&local_error, param_error[index]);
                param_error[index] = NULL;
                g_clear_object(&block);
                break;
            }
        }

        for (int index=0; index < N_PARAMETERS; index++)
        {
            g_clear_error(&param_error[index]);
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        g_clear_object(&block);
    }

    return block;
}


static void
bb_geda_block_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaBlock *block = BB_GEDA_BLOCK(item);
    g_return_if_fail(block != NULL);
}


void
bb_geda_block_set_rotation(BbGedaBlock *block, int angle)
{
    g_return_if_fail(BB_IS_GEDA_BLOCK(block));

    if (block->rotation != angle)
    {
        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        block->rotation = angle;

        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_ROTATION]);
    }
}


void
bb_geda_block_set_insert_x(BbGedaBlock *block, int x)
{
    g_return_if_fail(BB_IS_GEDA_BLOCK(block));

    if (block->insert_x != x)
    {
        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        block->insert_x = x;

        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_INSERT_X]);
    }
}


void
bb_geda_block_set_insert_y(BbGedaBlock *block, int y)
{
    g_return_if_fail(BB_IS_GEDA_BLOCK(block));

    if (block->insert_y != y)
    {
        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        block->insert_y = y;

        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_INSERT_Y]);
    }
}


void
bb_geda_block_set_mirror(BbGedaBlock *block, gboolean mirror)
{
    g_return_if_fail(BB_IS_GEDA_BLOCK(block));

    if ((block->mirror && !mirror) || (!block->mirror && mirror))
    {
        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        block->mirror = mirror;

        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_MIRROR]);
    }
}


static void
bb_geda_block_set_name(BbGedaBlock *block, const gchar *name)
{
    g_return_if_fail(BB_IS_GEDA_BLOCK(block));

    if (g_strcmp0(block->name, name) != 0)
    {
        g_free(block->name);

        block->name = g_strdup(name);

        g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_NAME]);
    }
}


static void
bb_geda_block_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_INSERT_X:
            bb_geda_block_set_insert_x(BB_GEDA_BLOCK(object), g_value_get_int(value));
            break;

        case PROP_INSERT_Y:
            bb_geda_block_set_insert_y(BB_GEDA_BLOCK(object), g_value_get_int(value));
            break;

        case PROP_SELECTABLE:
            bb_geda_block_set_selectable(BB_GEDA_BLOCK(object), g_value_get_boolean(value));
            break;

        case PROP_ROTATION:
            bb_geda_block_set_rotation(BB_GEDA_BLOCK(object), g_value_get_int(value));
            break;

        case PROP_MIRROR:
            bb_geda_block_set_mirror(BB_GEDA_BLOCK(object), g_value_get_boolean(value));
            break;

        case PROP_NAME:
            bb_geda_block_set_name(BB_GEDA_BLOCK(object), g_value_get_string(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_geda_block_set_selectable(BbGedaBlock *block, gboolean selectable)
{
    g_return_if_fail(BB_IS_GEDA_BLOCK(block));

    if ((block->selectable && !selectable) || (!block->selectable && selectable))
    {
        block->selectable = selectable;

        g_signal_emit(block, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_SELECTABLE]);
    }
}


static void
bb_geda_block_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaBlock *block = BB_GEDA_BLOCK(item);
    g_return_if_fail(block != NULL);

    bb_coord_translate(dx, dy, &block->insert_x, &block->insert_y, 1);

    g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_INSERT_X]);
    g_object_notify_by_pspec(G_OBJECT(block), properties[PROP_INSERT_Y]);
}


static gboolean
bb_geda_block_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error)
{
    g_return_val_if_fail(BB_IS_GEDA_BLOCK(item), FALSE);

    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d %s\n",
        BB_GEDA_BLOCK_TOKEN,
        bb_geda_block_get_insert_x(BB_GEDA_BLOCK(item)),
        bb_geda_block_get_insert_y(BB_GEDA_BLOCK(item)),
        bb_geda_block_get_selectable(BB_GEDA_BLOCK(item)),
        bb_geda_block_get_rotation(BB_GEDA_BLOCK(item)),
        bb_geda_block_get_mirror(BB_GEDA_BLOCK(item)),
        bb_geda_block_get_name(BB_GEDA_BLOCK(item))
        );

    gboolean result = g_output_stream_write_all(
        stream,
        params->str,
        params->len,
        NULL,
        cancellable,
        error
        );

    g_string_free(params, TRUE);

    return result;
}


static void
bb_geda_block_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    BbGedaBlock *block = BB_GEDA_BLOCK(item);
}


static gboolean
bb_geda_block_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    )
{
    return g_output_stream_write_all_finish(
        stream,
        result,
        NULL,
        error
        );
}
