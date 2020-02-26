#set($moduleLowerSnake = $moduleSnake.toLowerCase())
#set($moduleUpperSnake = $moduleSnake.toUpperCase())
#set($classLowerSnake = $classSnake.toLowerCase())
#set($classUpperSnake = $classSnake.toUpperCase())
#set($baseLowerSnake = $baseSnake.toLowerCase())
#set($baseUpperSnake = $baseSnake.toUpperCase())
#set($thing = "PreventUserQuery")
#set($basePascalCase = "PreventUserQuery")
#set($basePascalCase = "")
#foreach($thing in $baseLowerSnake.split("_",0))
#set($basePascalCase = $basePascalCase.concat("${thing.charAt(0).toString().toUpperCase()}${thing.substring(1)}"))
#end
#set($modulePascalCase = "PreventUserQuery")
#set($modulePascalCase = "")
#foreach($thing in $moduleLowerSnake.split("_",0))
#set($modulePascalCase = $modulePascalCase.concat("${thing.charAt(0).toString().toUpperCase()}${thing.substring(1)}"))
#end
#set($classPascalCase = "PreventUserQuery")
#set($classPascalCase = "")
#foreach($thing in $classLowerSnake.split("_",0))
#set($classPascalCase = $classPascalCase.concat("${thing.charAt(0).toString().toUpperCase()}${thing.substring(1)}"))
#end
#set($number = "PreventUserQuery")
#parse("License.h")

#[[#include]]# <gtk/gtk.h>
#[[#include]]# "$modulePascalCase.toLowerCase()${classPascalCase.toLowerCase()}.h"


enum
{
#foreach(${number} in [0..3])
    PROP_${number},
#end
};


struct _${modulePascalCase}${classPascalCase}
{
    ${basePascalCase} parent;
};


G_DEFINE_TYPE(${modulePascalCase}${classPascalCase}, ${moduleLowerSnake}_${classLowerSnake}, ${baseUpperSnake});


static void
${moduleLowerSnake}_${classLowerSnake}_dispose(GObject *object);

static void
${moduleLowerSnake}_${classLowerSnake}_finalize(GObject *object);

static void
${moduleLowerSnake}_${classLowerSnake}_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec);

static void
${moduleLowerSnake}_${classLowerSnake}_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec);


static void
${moduleLowerSnake}_${classLowerSnake}_class_init(${modulePascalCase}${classPascalCase}Class *klasse)
{
    G_OBJECT_CLASS(klasse)->dispose = ${moduleLowerSnake}_${classLowerSnake}_dispose;
    G_OBJECT_CLASS(klasse)->finalize = ${moduleLowerSnake}_${classLowerSnake}_finalize;
    G_OBJECT_CLASS(klasse)->get_property = ${moduleLowerSnake}_${classLowerSnake}_get_property;
    G_OBJECT_CLASS(klasse)->set_property = ${moduleLowerSnake}_${classLowerSnake}_set_property;

    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/$modulePascalCase.toLowerCase()${classPascalCase.toLowerCase()}.ui"
        );
}


static void
${moduleLowerSnake}_${classLowerSnake}_dispose(GObject *object)
{
    // ${modulePascalCase}${classPascalCase}* privat = ${moduleUpperSnake}${classUpperSnake}_GET_PRIVATE(object);
}


static void
${moduleLowerSnake}_${classLowerSnake}_finalize(GObject *object)
{
    // ${modulePascalCase}${classPascalCase}* privat = ${moduleUpperSnake}${classUpperSnake}_GET_PRIVATE(object);
}


static void
${moduleLowerSnake}_${classLowerSnake}_get_property(GObject *object, guint property_id, GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
#foreach(${number} in [1..3])
        case PROP_${number}:
            break;

#end
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}


static void
${moduleLowerSnake}_${classLowerSnake}_init(${modulePascalCase}${classPascalCase} *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
${moduleLowerSnake}_${classLowerSnake}_register()
{
    ${moduleLowerSnake}_${classLowerSnake}_get_type();
}


static void
${moduleLowerSnake}_${classLowerSnake}_set_property(GObject *object, guint property_id, const GValue *value, GParamSpec *pspec)
{
    switch (property_id)
    {
#foreach(${number} in [1..3])
        case PROP_${number}:
            break;

#end
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID(object, property_id, pspec);
    }
}
