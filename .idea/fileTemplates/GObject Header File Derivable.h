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
#[[#ifndef]]# __${modulePascalCase.toUpperCase()}${classPascalCase.toUpperCase()}__
#[[#define]]# __${modulePascalCase.toUpperCase()}${classPascalCase.toUpperCase()}__
#parse("License.h")

#[[#include]]# <gtk/gtk.h>

#[[#define]]# ${moduleUpperSnake}_TYPE_${classUpperSnake} ${moduleLowerSnake}_${classLowerSnake}_get_type()
G_DECLARE_DERIVABLE_TYPE(${modulePascalCase}${classPascalCase}, ${moduleLowerSnake}_${classLowerSnake}, ${moduleUpperSnake}, ${classUpperSnake}, ${basePascalCase})

struct _${modulePascalCase}${classPascalCase}Class
{
    ${basePascalCase}Class parent_class;
}

#[[#endif]]#
