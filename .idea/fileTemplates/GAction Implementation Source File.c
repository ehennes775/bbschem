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
    
    /* From GAction */
    
    PROP_ENABLED,
    PROP_NAME,
    PROP_PARAMETER_TYPE,
    PROP_STATE,
    PROP_STATE_HINT,
    PROP_STATE_TYPE,
    
    /* From ${modulePascalCase}${actionPascalCase}Action */
    
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


static GParamSpec *properties[N_PROPERTIES];


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
    g_warn_if_reached();
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_class_init(${modulePascalCase}${actionPascalCase}ActionClass *klasse)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klasse);
    g_return_if_fail(G_IS_GOBJECT_CLASS(object_class));
    
    object_class->dispose = ${moduleLowerSnake}_${actionLowerSnake}_action_dispose;
    object_class->finalize = ${moduleLowerSnake}_${actionLowerSnake}_action_finalize;
    object_class->get_property = ${moduleLowerSnake}_${actionLowerSnake}_action_get_property;
    object_class->set_property = ${moduleLowerSnake}_${actionLowerSnake}_action_set_property;

    /* From GAction */

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

    g_object_class_override_property(
        klasse,
        PROP_STATE_TYPE,
        "state-type"
        );
        
    /* From ${modulePascalCase}${actionPascalCase}Action */
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_dispose(GObject *object)
{
    ${modulePascalCase}${actionPascalCase}Action *${actionLowerSnake}_action = ${actionUpperSnake}_ACTION(object);
    g_return_if_fail(${moduleUpperSnake}_IS_${actionUpperSnake}_ACTION(${actionLowerSnake}_action));
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_finalize(GObject *object)
{
    ${modulePascalCase}${actionPascalCase}Action *${actionLowerSnake}_action = ${actionUpperSnake}_ACTION(object);
g_return_if_fail(${moduleUpperSnake}_IS_${actionUpperSnake}_ACTION(${actionLowerSnake}_action));
}


static gboolean
${moduleLowerSnake}_${actionLowerSnake}_action_get_enabled(GAction *action)
{
    g_return_val_if_fail(${moduleUpperSnake}_IS_${actionUpperSnake}_ACTION(action), FALSE);

    return TRUE;
}


static const gchar*
${moduleLowerSnake}_${actionLowerSnake}_action_get_name(GAction *action)
{
    g_warn_if_fail(${moduleUpperSnake}_IS_${actionUpperSnake}_ACTION(action));

    return "${actionLowerSnake}";
}


static const GVariantType*
${moduleLowerSnake}_${actionLowerSnake}_action_get_parameter_type(GAction *action)
{
    g_warn_if_fail(${moduleUpperSnake}_IS_${actionUpperSnake}_ACTION(action));

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
    g_return_if_fail(${moduleUpperSnake}_IS_${actionUpperSnake}_ACTION(action));

    return NULL;
}


static GVariant*
${moduleLowerSnake}_${actionLowerSnake}_action_get_state_hint(GAction *action)
{
    g_return_if_fail(${moduleUpperSnake}_IS_${actionUpperSnake}_ACTION(action));

    return NULL;
}


static const GVariantType*
${moduleLowerSnake}_${actionLowerSnake}_action_get_state_type(GAction *action)
{
    g_warn_if_fail(${moduleUpperSnake}_IS_${actionUpperSnake}_ACTION}(action));

    return NULL;
}


static void
${moduleLowerSnake}_${actionLowerSnake}_action_init(${modulePascalCase}${actionPascalCase}Action *${actionLowerSnake}_action)
{
    g_return_if_fail(${moduleUpperSnake}_IS_${actionUpperSnake}_ACTION}(${actionLowerSnake}_action));
}


${modulePascalCase}${actionPascalCase}Action*
${moduleLowerSnake}_${actionLowerSnake}_action_new()
{
    return ${moduleUpperSnake}_${actionUpperSnake}_ACTION(g_object_new(
        ${moduleUpperSnake}_TYPE_${actionUpperSnake}_ACTION,
        NULL
        ));
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
