cmake_minimum_required(VERSION 3.16)
project(bbschem C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GLIB REQUIRED glib-2.0)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
PKG_CHECK_MODULES(PEAS REQUIRED libpeas-1.0)

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${PEAS_INCLUDE_DIRS})

link_directories(${GLIB_LIBRARY_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
link_directories(${PEAS_LIBRARY_DIRS})

add_definitions(${GLIB_CFLAGS_OTHER})
add_definitions(${GTK3_CFLAGS_OTHER})
add_definitions(${PEAS_CFLAGS_OTHER})

include_directories(.)
include_directories(src/gui)
include_directories(src/lib)
include_directories(src/ext)

add_subdirectory(src/ext)
add_subdirectory(src/lib)
add_subdirectory(src/gui)

find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(THING1 bbsch.gresource.ui.c)
set(THING2 src/bbsch.gresource.ui)

add_custom_command(
        OUTPUT ${THING1}
        COMMAND ${GLIB_COMPILE_RESOURCES}
        ARGS
        --generate-source
        --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}/src
        --target=${CMAKE_CURRENT_BINARY_DIR}/${THING1}
        ${CMAKE_CURRENT_SOURCE_DIR}/${THING2}
        VERBATIM
        MAIN_DEPENDENCY ${THING2}
        COMMENT "Compiling GUI Resources to ${THING1}"
    )


add_executable(bbschem
        src/main.c
        ${CMAKE_CURRENT_BINARY_DIR}/${THING1}
        )

set_source_files_properties(
        ${CMAKE_CURRENT_BINARY_DIR}/${THING1}
        PROPERTIES GENERATED TRUE
)

#add_custom_target(
#        dummy
#        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${THING1}
#)
#
#add_dependencies(
#        bbgui
#        ${CMAKE_CURRENT_BINARY_DIR}/${THING1}
#)

target_link_libraries(bbschem
        bbgui
        bblib
        bbext
        m
        ${GLIB_LIBRARIES}
        ${GTK3_LIBRARIES}
        ${PEAS_LIBRARIES}
        )
