#set($moduleLowerSnake = $moduleSnake.toLowerCase())
#set($moduleUpperSnake = $moduleSnake.toUpperCase())
#set($actionLowerSnake = $actionSnake.toLowerCase())
#set($actionUpperSnake = $actionSnake.toUpperCase())
#set($thing = "PreventUserQuery")
#set($modulePascalCase = "PreventUserQuery")
#set($modulePascalCase = "")
#foreach($thing in $moduleLowerSnake.split("_",0))
#set($modulePascalCase = $modulePascalCase.concat("${thing.charAt(0).toString().toUpperCase()}${thing.substring(1)}"))
#end
#set($actionPascalCase = "PreventUserQuery")
#set($actionPascalCase = "")
#foreach($thing in $actionLowerSnake.split("_",0))
#set($actionPascalCase = $actionPascalCase.concat("${thing.charAt(0).toString().toUpperCase()}${thing.substring(1)}"))
#end
#parse("License.h")

#[[#include]]# <gtk/gtk.h>
#[[#include]]# "$modulePascalCase.toLowerCase()${actionPascalCase.toLowerCase()}action.h"

enum
{
    PROP_0,
    PROP_ENABLED,
    PROP_NAME,
    PROP_PARAMETER_TYPE,
    PROP_STATE,
    PROP_STATE_HINT,
    PROP_STATE_TYPE,
    N_PROPERTIES
};


struct _${modulePascalCase}${actionPascalCase}Action
{
    GObject parent;
};


static void
${moduleLowerSnake}_${actionLowerSnake}_action_action_init(GActionInterface *iface);

static void
${moduleLowerSnake}_${actionLowerSnake}_action_activate(GAction *action, GVariant *parameter);

static void
${moduleLowerSnake}_${actionLowerSnake}_action_change_state(GAction *action, GVariant *value);

static void
${moduleLowerSnake}_${actionLowerSnake}_action_dispose(GObject *object);

static void
${moduleLowerSnake}_${actionLowerSnake}_action_finalize(GObject *object);

static gboolean
${moduleLowerSnake}_${actionLowerSnake}_action_get_enabled(GAction *action);

static const gchar*
${moduleLowerSnake}_${actionLowerSnake}_action_get_name(GAction *action);

static const GVariantType*
${moduleLowerSnake}_${actionLowerSnake}_action_get_parameter_type(GAction *action);

static void
${moduleLowerSnake}_${actionLowerSnake}_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static GVariant*
${moduleLowerSnake}_${actionLowerSnake}_action_get_state(GAction *action);

static GVariant*
${moduleLowerSnake}_${actionLowerSnake}_action_get_state_hint(GAction *action);

static const GVariantType*
${moduleLowerSnake}_${actionLowerSnake}_action_get_state_type(GAction *action);

static void
${moduleLowerSnake}_${actionLowerSnake}_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


GParamSpec *properties[N_PROPERTIES];


G_DEFINE_TYPE_WITH_CODE(
    ${modulePascalCase}${actionPascalCase}Action,
    ${moduleLowerSnake}_${actionLowerSnake}_action,
    G_TYPE_OBJECT,
    G_IMPLEMENT_INTERFACE(G_TYPE_ACTION, ${moduleLowerSnake}_${actionLowerSnake}_action_action_init)
    )


static void
${moduleLowerSnake}_${actionLowerSnake}_action_action_init(GActionInterface *iface)
{
    iface->activate = ${moduleLowerSnake}_${actionLowerSnake}_action_activate;
    iface->change_state = ${moduleLowerSnake}_${actionLowerSnake}_action_change_state;
    iface->get_enabled = ${moduleLowerSnake}_${actionLowerSnake}_action_get_enabled;
    iface->get_name = ${moduleLowerSnake}_${actionLowerSnake}_action_get_name;
    iface->get_parameter_type = ${moduleLowerSnake}_${actionLowerSnake}_action_get_parameter_type;
    iface->get_state = ${moduleLowerSnake}_${actionLowerSnake}_action_get_state;
    iface->get_state_hint = ${moduleLowerSnake}_${actionLowerSnake}_action_get_state_hint;
    iface->get_state_type = ${moduleLowerSnake}_${actionLowerSnake}_action_get_state_type;
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_activate(GAction *action, GVariant *parameter)
{
    g_message("${moduleLowerSnake}_${actionLowerSnake}_action_activate");
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_change_state(GAction *action, GVariant *value)
{

}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_class_init(${modulePascalCase}${actionPascalCase}ActionClass *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = ${moduleLowerSnake}_${actionLowerSnake}_action_dispose;
    G_OBJECT_CLASS(klasse)->finalize = ${moduleLowerSnake}_${actionLowerSnake}_action_finalize;
    G_OBJECT_CLASS(klasse)->get_property = ${moduleLowerSnake}_${actionLowerSnake}_action_get_property;
    G_OBJECT_CLASS(klasse)->set_property = ${moduleLowerSnake}_${actionLowerSnake}_action_set_property;

    g_object_class_override_property(
        klasse,
        PROP_ENABLED,
        "enabled"
        );

    g_object_class_override_property(
        klasse,
        PROP_NAME,
        "name"
        );

    g_object_class_override_property(
        klasse,
        PROP_PARAMETER_TYPE,
        "parameter-type"
        );

    g_object_class_override_property(
        klasse,
        PROP_STATE,
        "state"
        );

    //g_object_class_override_property(
    //    klasse,
    //    PROP_STATE_HINT,
    //    "state-hint"
    //    );

    g_object_class_override_property(
        klasse,
        PROP_STATE_TYPE,
        "state-type"
        );
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_dispose(GObject *object)
{
    // ${modulePascalCase}${actionPascalCase}Action* privat = ${modulePascalCase}${actionPascalCase}_ACTION_GET_PRIVATE(object);
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_finalize(GObject *object)
{
    // ${modulePascalCase}${actionPascalCase}Action* privat = ${modulePascalCase}${actionPascalCase}_ACTION_GET_PRIVATE(object);
}


static gboolean
${moduleLowerSnake}_${actionLowerSnake}_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(action != NULL, FALSE);

    return TRUE;
}


static const gchar*
${moduleLowerSnake}_${actionLowerSnake}_action_get_name(GAction *action)
{
    const gchar* name = "${actionPascalCase}";

    g_return_val_if_fail(action != NULL, name);

    return name;
}


static const GVariantType*
${moduleLowerSnake}_${actionLowerSnake}_action_get_parameter_type(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        case PROP_ENABLED:
            g_value_set_boolean(value, ${moduleLowerSnake}_${actionLowerSnake}_action_get_enabled(G_ACTION(object)));
            break;

        case PROP_NAME:
            g_value_set_string(value, ${moduleLowerSnake}_${actionLowerSnake}_action_get_name(G_ACTION(object)));
            break;

        case PROP_PARAMETER_TYPE:
            g_value_set_boxed(value, ${moduleLowerSnake}_${actionLowerSnake}_action_get_parameter_type(G_ACTION(object)));
            break;

        case PROP_STATE:
            g_value_set_variant(value, ${moduleLowerSnake}_${actionLowerSnake}_action_get_state(G_ACTION(object)));
            break;

        case PROP_STATE_HINT:
            g_value_set_variant(value, ${moduleLowerSnake}_${actionLowerSnake}_action_get_state_hint(G_ACTION(object)));
            break;

        case PROP_STATE_TYPE:
            g_value_set_boxed(value, ${moduleLowerSnake}_${actionLowerSnake}_action_get_state_type(G_ACTION(object)));
            break;

        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static GVariant *
${moduleLowerSnake}_${actionLowerSnake}_action_get_state(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static GVariant*
${moduleLowerSnake}_${actionLowerSnake}_action_get_state_hint(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static const GVariantType*
${moduleLowerSnake}_${actionLowerSnake}_action_get_state_type(GAction *action)
{
    g_return_val_if_fail(action != NULL, NULL);

    return NULL;
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_init(${modulePascalCase}${actionPascalCase}Action *window)
{
}


${modulePascalCase}${actionPascalCase}Action*
${moduleLowerSnake}_${actionLowerSnake}_action_new()
{
    return ${moduleUpperSnake}_${actionUpperSnake}_ACTION(g_object_new(${moduleUpperSnake}_TYPE_${actionUpperSnake}_ACTION, NULL));
}


__attribute__((constructor)) void
${moduleLowerSnake}_${actionLowerSnake}_action_register()
{
    ${moduleLowerSnake}_${actionLowerSnake}_action_get_type();
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
