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


G_DEFINE_INTERFACE(${modulePascalCase}${classPascalCase}, ${moduleLowerSnake}_${classLowerSnake}, G_TYPE_OBJECT)


static void
${moduleLowerSnake}_${classLowerSnake}_default_init(${modulePascalCase}${classPascalCase}Interface *class)
{
    g_return_if_fail(class != NULL);
}
