#set($moduleLowerSnake = $moduleSnake.toLowerCase())
#set($moduleUpperSnake = $moduleSnake.toUpperCase())
#set($classLowerSnake = $classSnake.toLowerCase())
#set($classUpperSnake = $classSnake.toUpperCase())
#[[#ifndef]]# __$modulePascalCase.toUpperCase()$classPascalCase.toUpperCase()__
#[[#define]]# __$modulePascalCase.toUpperCase()$classPascalCase.toUpperCase()__
#parse("License.h")

#[[#include]]# <gtk/gtk.h>

#[[#define]]# ${moduleUpperSnake}_TYPE_${classUpperSnake} ${moduleLowerSnake}_${classLowerSnake}_get_type()
G_DECLARE_FINAL_TYPE(${modulePascalCase}${classPascalCase}, ${moduleLowerSnake}_${classLowerSnake}, ${moduleUpperSnake}, ${classUpperSnake}, ${basePascalCase})

#[[#endif]]#
