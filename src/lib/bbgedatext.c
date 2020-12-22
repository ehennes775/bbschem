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
#include <bbextensions.h>
#include "bbcoord.h"
#include "bbgedatext.h"
#include "bbadjustableitemcolor.h"
#include "bberror.h"
#include "bbtextalignment.h"
#include "bbtextpresentation.h"
#include "bbtextvisibility.h"
#include "bbtextsize.h"
#include "bbcolors.h"
#include "bbcolor.h"


/**
 * The line break sequence used to separate lines inside multiline strings. This same sequence is also written between
 * multiline strings in the output files.
 */
#define LINE_BREAK "\n"


/**
 * The positions of parameters in the file format
 */
enum
{
    PARAM_TOKEN,
    PARAM_INSERT_X,
    PARAM_INSERT_Y,
    PARAM_COLOR,
    PARAM_SIZE,
    PARAM_VISIBILITY,
    PARAM_PRESENTATION,
    PARAM_ROTATION,
    PARAM_ALIGNMENT,
    PARAM_LINE_COUNT,
    N_PARAMETERS
};


enum
{
    PROP_0,

    /* From AdjustableItemColor */
    PROP_ITEM_COLOR,

    /* From GedaText */
    PROP_ALIGNMENT,
    PROP_INSERT_X,
    PROP_INSERT_Y,
    PROP_PRESENTATION,
    PROP_ROTATION,
    PROP_SIZE,
    PROP_TEXT,
    PROP_VISIBILITY,

    N_PROPERTIES
};


enum
{
    SIG_INVALIDATE,
    N_SIGNALS
};


struct _BbGedaText
{
    BbGedaItem parent;

    BbTextAlignment alignment;

    /**
     * @brief Contains the value of the text as attributes
     *
     * When the text does not represent an attribute, the 'name' and 'both' contain empty strings. The 'value' of the
     * attribute could contain an empty string normally. These values should not be NULL.
     *
     * @see _BbGedaText.text
     */
    gchar *attributes[N_TEXT_PRESENTATION];

    int color;

    int insert_x;

    int insert_y;

    /**
     * @brief Indicates which part of the attribute to present: name, value, or both.
     *
     * This value is ignored when the text does not represent an attribute.
     *
     * @see _BbGedaText.attributes
     */
    BbTextPresentation presentation;

    /**
     * The rotation angle of the text in degrees.
     */
    int rotation;

    /**
     * The size of the text in points.
     */
    int size;

    /**
     * @brief The entire text as a multiline string
     *
     * Contains a multiline string with lines separated by LINE_BREAK with no LINE_BREAK at the end. This value should
     * not contain NULL.
     */
    gchar* text;

    /**
     * @brief Indicates if the text should be shown or hidden.
     */
    BbTextVisibility visibility;
};


static void
bb_geda_text_adjustable_item_color_init(
    BbAdjustableItemColorInterface *iface
    );

static BbBounds*
bb_geda_text_calculate_bounds(
    BbGedaItem *item,
    BbBoundsCalculator *calculator
    );

static BbGedaItem*
bb_geda_text_clone(
    BbGedaItem *item
    );

static int
bb_geda_text_count_lines(
    BbGedaText *text_item
    );

static void
bb_geda_text_dispose(
    GObject *object
    );

static void
bb_geda_text_finalize(
    GObject *object
    );

static GRegex*
bb_geda_text_get_attribute_regex();

static int
bb_geda_text_get_item_color(
    BbGedaText *text
    );

static void
bb_geda_text_get_property(
    GObject *object,
    guint property_id,
    GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_text_render(
    BbGedaItem *item,
    BbItemRenderer *renderer
    );

static void
bb_geda_text_set_item_color(
    BbGedaText *text,
    int color
    );

static void
bb_geda_text_set_property(
    GObject *object,
    guint property_id,
    const GValue *value,
    GParamSpec *pspec
    );

static void
bb_geda_text_translate(
    BbGedaItem *item,
    int dx,
    int dy
    );

static gboolean
bb_geda_text_write(
    BbGedaItem *item,
    GOutputStream *stream,
    GCancellable *cancellable,
    GError **error
    );

static void
bb_geda_text_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    );

static gboolean
bb_geda_text_write_finish(
    BbGedaItem *item,
    GOutputStream *stream,
    GAsyncResult *result,
    GError **error
    );


static GParamSpec *properties[N_PROPERTIES];
static guint signals[N_SIGNALS];


G_DEFINE_TYPE_WITH_CODE(
    BbGedaText,
    bb_geda_text,
    BB_TYPE_GEDA_ITEM,
    G_IMPLEMENT_INTERFACE(BB_TYPE_ADJUSTABLE_ITEM_COLOR, bb_geda_text_adjustable_item_color_init)
    )


static void
bb_geda_text_adjustable_item_color_init(BbAdjustableItemColorInterface *iface)
{
    g_return_if_fail(iface != NULL);
}


static BbBounds*
bb_geda_text_calculate_bounds(BbGedaItem *item, BbBoundsCalculator *calculator)
{
    BbGedaText *text = BB_GEDA_TEXT(item);

    g_return_val_if_fail(text != NULL, NULL);

    return bb_bounds_calculator_calculate_from_corners(
        calculator,
        text->insert_x,
        text->insert_y,
        text->insert_x,
        text->insert_y,
        0
        );
}


static void
bb_geda_text_class_init(BbGedaTextClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(object_class != NULL);

    object_class->dispose = bb_geda_text_dispose;
    object_class->finalize = bb_geda_text_finalize;
    object_class->get_property = bb_geda_text_get_property;
    object_class->set_property = bb_geda_text_set_property;

    BB_GEDA_ITEM_CLASS(klasse)->calculate_bounds = bb_geda_text_calculate_bounds;
    BB_GEDA_ITEM_CLASS(klasse)->clone = bb_geda_text_clone;
    BB_GEDA_ITEM_CLASS(klasse)->render = bb_geda_text_render;
    BB_GEDA_ITEM_CLASS(klasse)->translate = bb_geda_text_translate;
    BB_GEDA_ITEM_CLASS(klasse)->write = bb_geda_text_write;
    BB_GEDA_ITEM_CLASS(klasse)->write_async = bb_geda_text_write_async;
    BB_GEDA_ITEM_CLASS(klasse)->write_finish = bb_geda_text_write_finish;

    /* Properties from BbAdjustableItemColor */
    properties[PROP_ITEM_COLOR] = bb_object_class_override_property(
        object_class,
        PROP_ITEM_COLOR,
        "item-color"
        );

    properties[PROP_ALIGNMENT] = bb_object_class_install_property(
        object_class,
        PROP_ALIGNMENT,
        g_param_spec_int(
            "alignment",
            "",
            "",
            0,
            N_TEXT_ALIGNMENT - 1,
            BB_TEXT_ALIGNMENT_LOWER_LEFT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

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
            G_PARAM_READWRITE
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
            G_PARAM_READWRITE
            )
        );

    properties[PROP_PRESENTATION] = bb_object_class_install_property(
        object_class,
        PROP_PRESENTATION,
        g_param_spec_int(
            "presentation",
            "",
            "",
            0,
            N_TEXT_PRESENTATION - 1,
            BB_TEXT_PRESENTATION_BOTH,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_ROTATION] = bb_object_class_install_property(
        object_class,
        PROP_ROTATION,
        g_param_spec_int(
            "rotation",
            "",
            "",
            G_MININT,
            G_MAXINT,
            0,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_SIZE] = bb_object_class_install_property(
        object_class,
        PROP_SIZE,
        g_param_spec_int(
            "size",
            "",
            "",
            BB_TEXT_SIZE_MIN,
            BB_TEXT_SIZE_MAX,
            BB_TEXT_SIZE_DEFAULT,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    properties[PROP_TEXT] = bb_object_class_install_property(
        object_class,
        PROP_TEXT,
        g_param_spec_string(
            "text",
            "",
            "",
            "Empty",
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );


    properties[PROP_VISIBILITY] = bb_object_class_install_property(
        object_class,
        PROP_VISIBILITY,
        g_param_spec_int(
            "visibility",
            "",
            "",
            0,
            N_TEXT_VISIBILITY - 1,
            BB_TEXT_VISIBILITY_VISIBLE,
            G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS
            )
        );

    signals[SIG_INVALIDATE] = g_signal_lookup("invalidate-item", BB_TYPE_GEDA_ITEM);
}


static BbGedaItem*
bb_geda_text_clone(BbGedaItem *item)
{
    return BB_GEDA_ITEM(g_object_new(
        BB_TYPE_GEDA_TEXT,

        /* From AdjustableItemColor */
        "item-color", bb_geda_text_get_item_color(BB_GEDA_TEXT(item)),

        /* From GedaText */
        "alignment", bb_geda_text_get_alignment(BB_GEDA_TEXT(item)),
        "insert-x", bb_geda_text_get_insert_x(BB_GEDA_TEXT(item)),
        "insert-y", bb_geda_text_get_insert_y(BB_GEDA_TEXT(item)),
        "presentation", bb_geda_text_get_presentation(BB_GEDA_TEXT(item)),
        "rotation", bb_geda_text_get_rotation(BB_GEDA_TEXT(item)),
        "size", bb_geda_text_get_size(BB_GEDA_TEXT(item)),
        "text", bb_geda_text_get_text(BB_GEDA_TEXT(item)),
        "visibility", bb_geda_text_get_visibility(BB_GEDA_TEXT(item)),

        NULL
        ));
}


static int
bb_geda_text_count_lines(BbGedaText *text_item)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text_item), 1);
    g_return_val_if_fail(text_item->text != NULL, 1);

    int count = 1;
    int length = strlen(LINE_BREAK);

    char *iter = strstr(LINE_BREAK, text_item->text);

    while (iter != NULL)
    {
        iter += length;

        count++;

        iter = strstr(LINE_BREAK, iter);
    }

    return count;
}


static void
bb_geda_text_dispose(GObject *object)
{
}


static void
bb_geda_text_finalize(GObject *object)
{
    BbGedaText *text_item = BB_GEDA_TEXT(object);
    g_return_if_fail(text_item != NULL);

    for (int index = 0; index < N_TEXT_PRESENTATION; index++)
    {
        g_free(text_item->attributes[index]);
    }

    g_free(text_item->text);
}


BbTextAlignment
bb_geda_text_get_alignment(BbGedaText *text)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text), BB_TEXT_ALIGNMENT_LOWER_LEFT);

    return text->alignment;
}


/**
 * @brief Provides a regular expression to extract attribute data from the text
 *
 * <table>
 * <tr>
 * <th>Group Name</th>
 * <th>Description</th>
 * </tr>
 * <tr>
 * <td>name</td>
 * <td>The name of the attribute, or if no match, the text does not represent an attribute</td>
 * </tr>
 * <tr>
 * <td>value</td>
 * <td>The value of the attribute, or if no match, the text does not represent an attribute</td>
 * </tr>
 * <tr>
 * <td>text</td>
 * <td>The text, or if no match, the text represents an attribute</td>
 * </tr>
 * </table>
 */
static GRegex*
bb_geda_text_get_attribute_regex()
{
    static gsize done = 0;
    static GRegex *regex = NULL;

    if (g_once_init_enter(&done))
    {
        GError *local_error = NULL;

        regex = g_regex_new(
            "(?<both>(?<name>.+?)=(?<value>.*))|(?<text>.+)",
            G_REGEX_MULTILINE,
            0,
            &local_error
            );

        g_assert(regex != NULL);
        g_assert(local_error == NULL);

        g_once_init_leave(&done, 1);
    }

    return regex;
}


int
bb_geda_text_get_insert_x(BbGedaText *text)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text), 0);

    return text->insert_x;
}


int
bb_geda_text_get_insert_y(BbGedaText *text)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text), 0);

    return text->insert_y;
}


static int
bb_geda_text_get_item_color(BbGedaText *text)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text), BB_COLOR_GRAPHIC);

    return text->color;
}


int
bb_geda_text_get_line_count(BbParams *params, GError **error)
{
    g_return_val_if_fail(params != NULL, 0);

    GError *local_error = NULL;

    int line_count = bb_params_get_int(params, PARAM_LINE_COUNT, &local_error);

    if (local_error == NULL)
    {
        if (line_count <= 0)
        {
            local_error = g_error_new(
                BB_ERROR_DOMAIN,
                ERROR_VALUE_OUT_OF_RANGE,
                "Line count of %d is out of range",
                line_count
                );
        }
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        line_count = 0;
    }

    return line_count;
}


static void
bb_geda_text_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ALIGNMENT:
            g_value_set_int(value, bb_geda_text_get_alignment(BB_GEDA_TEXT(object)));
            break;

        case PROP_INSERT_X:
            g_value_set_int(value, bb_geda_text_get_insert_x(BB_GEDA_TEXT(object)));
            break;

        case PROP_INSERT_Y:
            g_value_set_int(value, bb_geda_text_get_insert_y(BB_GEDA_TEXT(object)));
            break;

        case PROP_ITEM_COLOR:
            g_value_set_int(value, bb_geda_text_get_item_color(BB_GEDA_TEXT(object)));
            break;

        case PROP_PRESENTATION:
            g_value_set_int(value, bb_geda_text_get_presentation(BB_GEDA_TEXT(object)));
            break;

        case PROP_ROTATION:
            g_value_set_int(value, bb_geda_text_get_rotation(BB_GEDA_TEXT(object)));
            break;

        case PROP_SIZE:
            g_value_set_int(value, bb_geda_text_get_size(BB_GEDA_TEXT(object)));
            break;

        case PROP_TEXT:
            g_value_set_string(value, bb_geda_text_get_text(BB_GEDA_TEXT(object)));
            break;

        case PROP_VISIBILITY:
            g_value_set_int(value, bb_geda_text_get_visibility(BB_GEDA_TEXT(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


BbTextPresentation
bb_geda_text_get_presentation(BbGedaText *text)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text), BB_TEXT_PRESENTATION_BOTH);

    return text->presentation;
}


int
bb_geda_text_get_rotation(BbGedaText *text)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text), 0);

    return text->rotation;
}


int
bb_geda_text_get_size(BbGedaText *text)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text), 10);

    return text->size;
}


gchar*
bb_geda_text_get_text(BbGedaText *text_item)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text_item), NULL);

    return text_item->text;
}


BbTextVisibility
bb_geda_text_get_visibility(BbGedaText *text)
{
    g_return_val_if_fail(BB_IS_GEDA_TEXT(text), BB_TEXT_VISIBILITY_VISIBLE);

    return text->visibility;
}


static void
bb_geda_text_init(BbGedaText *text)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text));
}


BbGedaText*
bb_geda_text_new_with_params(BbParams *params, gchar **lines, GError **error)
{
    g_return_val_if_fail(params != NULL, NULL);
    g_return_val_if_fail(lines != NULL, NULL);
    g_return_val_if_fail(bb_params_token_matches(params, BB_GEDA_TEXT_TOKEN), NULL);

    gint alignment;
    gint color;
    gint insert_x;
    gint insert_y;
    GError *local_error = NULL;
    gint presentation;
    gint rotation;
    gint size;
    BbGedaText *text_item = NULL;
    gint visibility;

    insert_x = bb_params_get_int(params, PARAM_INSERT_X, &local_error);

    if (local_error == NULL)
    {
        insert_y = bb_params_get_int(params, PARAM_INSERT_Y, &local_error);
    }

    if (local_error == NULL)
    {
        color = bb_params_get_int(params, PARAM_COLOR, &local_error);
    }

    if (local_error == NULL)
    {
        size = bb_params_get_int(params, PARAM_SIZE, &local_error);
    }

    if (local_error == NULL)
    {
        visibility = bb_params_get_int(params, PARAM_VISIBILITY, &local_error);
    }

    if (local_error == NULL)
    {
        presentation = bb_params_get_int(params, PARAM_PRESENTATION, &local_error);
    }

    if (local_error == NULL)
    {
        rotation = bb_params_get_int(params, PARAM_ROTATION, &local_error);
    }

    if (local_error == NULL)
    {
        alignment = bb_params_get_int(params, PARAM_ALIGNMENT, &local_error);
    }

    if (local_error == NULL)
    {
        gchar *text = g_strjoinv(LINE_BREAK, lines);

        text_item = BB_GEDA_TEXT(g_object_new(
            BB_TYPE_GEDA_TEXT,
            "insert-x", insert_x,
            "insert-y", insert_y,
            "item-color", color,
            "size", size,
            "visibility", visibility,
            "presentation", presentation,
            "rotation", rotation,
            "alignment", alignment,
            "text", text,
            NULL
            ));

        g_free(text);
    }

    if (local_error != NULL)
    {
        g_propagate_error(error, local_error);
        g_clear_object(&text_item);
    }

    return text_item;
}


static void
bb_geda_text_render(BbGedaItem *item, BbItemRenderer *renderer)
{
    BbGedaText *text = BB_GEDA_TEXT(item);
    g_return_if_fail(text != NULL);

    bb_item_renderer_render_text(
        renderer,
        bb_geda_text_get_insert_x(text),
        bb_geda_text_get_insert_y(text),
        bb_geda_text_get_text(text)
        );
}


void
bb_geda_text_set_alignment(BbGedaText *text_item, BbTextAlignment alignment)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text_item));
    g_return_if_fail(bb_text_alignment_is_valid(alignment));

    if (text_item->alignment != alignment)
    {
        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        text_item->alignment = alignment;

        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_ALIGNMENT]);
    }
}


void
bb_geda_text_set_insert_x(BbGedaText *text_item, int x)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text_item));

    if (text_item->insert_x != x)
    {
        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        text_item->insert_x = x;

        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_INSERT_X]);
    }
}


void
bb_geda_text_set_insert_y(BbGedaText *text_item, int y)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text_item));

    if (text_item->insert_y != y)
    {
        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        text_item->insert_y = y;

        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_INSERT_Y]);
    }
}


static void
bb_geda_text_set_item_color(BbGedaText *text_item, int color)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text_item));
    g_return_if_fail(bb_color_is_valid(color));

    if (text_item->color != color)
    {
        text_item->color = color;

        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_ITEM_COLOR]);
    }
}


void
bb_geda_text_set_presentation(BbGedaText *text_item, BbTextPresentation presentation)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text_item));
    g_return_if_fail(bb_text_presentation_is_valid(presentation));

    if (text_item->presentation != presentation)
    {
        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        text_item->presentation = presentation;

        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_PRESENTATION]);
    }
}


static void
bb_geda_text_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ALIGNMENT:
            bb_geda_text_set_alignment(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        case PROP_INSERT_X:
            bb_geda_text_set_insert_x(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        case PROP_INSERT_Y:
            bb_geda_text_set_insert_y(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        case PROP_ITEM_COLOR:
            bb_geda_text_set_item_color(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        case PROP_PRESENTATION:
            bb_geda_text_set_presentation(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        case PROP_ROTATION:
            bb_geda_text_set_rotation(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        case PROP_SIZE:
            bb_geda_text_set_size(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        case PROP_TEXT:
            bb_geda_text_set_text(BB_GEDA_TEXT(object), g_value_get_string(value));
            break;

        case PROP_VISIBILITY:
            bb_geda_text_set_visibility(BB_GEDA_TEXT(object), g_value_get_int(value));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


void
bb_geda_text_set_rotation(BbGedaText *text_item, int rotation)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text_item));

    if (text_item->rotation != rotation)
    {
        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        text_item->rotation = rotation;

        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_ROTATION]);
    }
}


void
bb_geda_text_set_size(BbGedaText *text_item, int size)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text_item));
    g_return_if_fail(bb_text_size_is_valid(size));

    if (text_item->size != size)
    {
        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        text_item->size = size;

        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_SIZE]);
    }
}



void
bb_geda_text_set_text(BbGedaText *text_item, const gchar *text)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text_item));
    g_return_if_fail(text != NULL);

    if (g_strcmp0(text_item->text, text) != 0)
    {
        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_free(text_item->text);
        text_item->text = g_strdup(text);

        GMatchInfo *match_info;

        gboolean result = g_regex_match(
            bb_geda_text_get_attribute_regex(),
            text_item->text,
            0,
            &match_info
            );

        g_free(text_item->attributes[BB_TEXT_PRESENTATION_BOTH]);
        text_item->attributes[BB_TEXT_PRESENTATION_BOTH] = g_match_info_fetch_named(match_info, "both");

        g_free(text_item->attributes[BB_TEXT_PRESENTATION_VALUE]);
        text_item->attributes[BB_TEXT_PRESENTATION_VALUE] = g_match_info_fetch_named(match_info, "value");

        g_free(text_item->attributes[BB_TEXT_PRESENTATION_NAME]);
        text_item->attributes[BB_TEXT_PRESENTATION_NAME] = g_match_info_fetch_named(match_info, "name");

        g_match_info_free(match_info);

        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_TEXT]);
    }
}


void
bb_geda_text_set_visibility(BbGedaText *text_item, BbTextVisibility visibility)
{
    g_return_if_fail(BB_IS_GEDA_TEXT(text_item));
    g_return_if_fail(bb_text_visibility_is_valid(visibility));

    if (text_item->visibility != visibility)
    {
        text_item->visibility = visibility;
        
        g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

        g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_VISIBILITY]);
    }
}



static void
bb_geda_text_translate(BbGedaItem *item, int dx, int dy)
{
    BbGedaText *text_item = BB_GEDA_TEXT(item);
    g_return_if_fail(text_item != NULL);

    g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

    bb_coord_translate(dx, dy, &text_item->insert_x, &text_item->insert_y, 1);

    g_signal_emit(text_item, signals[SIG_INVALIDATE], 0);

    g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_INSERT_X]);
    g_object_notify_by_pspec(G_OBJECT(text_item), properties[PROP_INSERT_Y]);
}


static gboolean
bb_geda_text_write(BbGedaItem *item, GOutputStream *stream, GCancellable *cancellable, GError **error)
{
    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d %d %d %d %d\n%s\n",
        BB_GEDA_TEXT_TOKEN,
        bb_geda_text_get_insert_x(BB_GEDA_TEXT(item)),
        bb_geda_text_get_insert_y(BB_GEDA_TEXT(item)),
        bb_adjustable_item_color_get_color(BB_ADJUSTABLE_ITEM_COLOR(item)),
        bb_geda_text_get_size(BB_GEDA_TEXT(item)),
        bb_geda_text_get_visibility(BB_GEDA_TEXT(item)),
        bb_geda_text_get_presentation(BB_GEDA_TEXT(item)),
        bb_geda_text_get_rotation(BB_GEDA_TEXT(item)),
        bb_geda_text_get_alignment(BB_GEDA_TEXT(item)),
        bb_geda_text_count_lines(BB_GEDA_TEXT(item)),
        bb_geda_text_get_text(BB_GEDA_TEXT(item))
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
bb_geda_text_write_async(
    BbGedaItem *item,
    GOutputStream *stream,
    int io_priority,
    GCancellable *cancellable,
    GAsyncReadyCallback callback,
    gpointer callback_data
    )
{
    GString *params = g_string_new(NULL);

    g_string_printf(
        params,
        "%s %d %d %d %d %d %d %d %d %d\n%s\n",
        BB_GEDA_TEXT_TOKEN,
        bb_geda_text_get_insert_x(BB_GEDA_TEXT(item)),
        bb_geda_text_get_insert_y(BB_GEDA_TEXT(item)),
        bb_adjustable_item_color_get_color(BB_ADJUSTABLE_ITEM_COLOR(item)),
        bb_geda_text_get_size(BB_GEDA_TEXT(item)),
        bb_geda_text_get_visibility(BB_GEDA_TEXT(item)),
        bb_geda_text_get_presentation(BB_GEDA_TEXT(item)),
        bb_geda_text_get_rotation(BB_GEDA_TEXT(item)),
        bb_geda_text_get_alignment(BB_GEDA_TEXT(item)),
        bb_geda_text_count_lines(BB_GEDA_TEXT(item)),
        bb_geda_text_get_text(BB_GEDA_TEXT(item))
        );

    g_output_stream_write_all_async(
        stream,
        params->str,
        params->len,
        io_priority,
        cancellable,
        callback,
        callback_data
        );
}


static gboolean
bb_geda_text_write_finish(
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
