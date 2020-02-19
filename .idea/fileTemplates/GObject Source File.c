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
#parse("License.h")

#[[#include]]# <gtk/gtk.h>
#[[#include]]# "$modulePascalCase.toLowerCase()${classPascalCase.toLowerCase()}.h"

struct _${modulePascalCase}${classPascalCase}
{
    ${basePascalCase} parent;
};


G_DEFINE_TYPE(${modulePascalCase}${classPascalCase}, ${moduleLowerSnake}_${classLowerSnake}, ${baseUpperSnake});


static void
${moduleLowerSnake}_${classLowerSnake}_class_init(${modulePascalCase}${classPascalCase}Class *klasse)
{
    gtk_widget_class_set_template_from_resource(
        GTK_WIDGET_CLASS(klasse),
        "/com/github/ehennes775/bbsch/gui/$modulePascalCase.toLowerCase()${classPascalCase.toLowerCase()}.ui"
        );
}


static void
${moduleLowerSnake}_${classLowerSnake}_init(${modulePascalCase}${classPascalCase} *window)
{
    gtk_widget_init_template(GTK_WIDGET(window));
}


__attribute__((constructor)) void
${moduleLowerSnake}_${classLowerSnake}_register()
{
    ${moduleLowerSnake}${classLowerSnake}_get_type();
}
