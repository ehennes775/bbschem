cmake_minimum_required(VERSION 3.16)

enable_testing()

project(bbschem)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

find_package(PkgConfig REQUIRED)
find_package(Catch2 REQUIRED)

PKG_CHECK_MODULES(GLIB REQUIRED glib-2.0)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
PKG_CHECK_MODULES(PEAS REQUIRED libpeas-1.0)
PKG_CHECK_MODULES(GTKSRC REQUIRED gtksourceview-3.0)

include_directories(${GLIB_INCLUDE_DIRS})
include_directories(${GTK3_INCLUDE_DIRS})
include_directories(${PEAS_INCLUDE_DIRS})
include_directories(${GTKSRC_INCLUDE_DIRS})

link_directories(${GLIB_LIBRARY_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
link_directories(${PEAS_LIBRARY_DIRS})
link_directories(${GTKSRC_LIBRARY_DIRS})

add_definitions(${GLIB_CFLAGS_OTHER})
add_definitions(${GTK3_CFLAGS_OTHER})
add_definitions(${PEAS_CFLAGS_OTHER})
add_definitions(${GTKSRC_CFLAGS_OTHER})

include_directories(src/gui)
include_directories(src/lib)
include_directories(src/ext)
include_directories(src)

add_subdirectory(src/ext)
add_subdirectory(src/lib)
add_subdirectory(src/libtest)
add_subdirectory(src/gui)
add_subdirectory(src/gedaplugin)
add_subdirectory(src/lib2)
add_subdirectory(tests)

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
        #bbgedaplugin
        bbschemgui
        bblib
        #bblib2
        bbext
        m
        ${GLIB_LIBRARIES}
        ${GTK3_LIBRARIES}
        ${PEAS_LIBRARIES}
        ${GTKSRC_LIBRARIES}
        )

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
#set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fPIE")

set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -u bb_general_opener_add_specific_opener")
