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
#[[#ifndef]]# __${modulePascalCase.toUpperCase()}${actionPascalCase.toUpperCase()}ACTION__
#[[#define]]# __${modulePascalCase.toUpperCase()}${actionPascalCase.toUpperCase()}ACTION__
#parse("License.h")

#[[#include]]# <gtk/gtk.h>

#[[#define]]# ${moduleUpperSnake}_TYPE_${actionUpperSnake}_ACTION ${moduleLowerSnake}_${actionLowerSnake}_action_get_type()
G_DECLARE_FINAL_TYPE(${modulePascalCase}${actionPascalCase}Action, ${moduleLowerSnake}_${actionLowerSnake}_action, ${moduleUpperSnake}, ${actionUpperSnake}_ACTION, GObject)

${modulePascalCase}${actionPascalCase}Action*
${moduleLowerSnake}_${actionLowerSnake}_action_new();

#[[#endif]]#
