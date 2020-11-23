cmake_minimum_required(VERSION 3.16)
project(bbsch C)

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

add_subdirectory(src/lib)
add_subdirectory(src/gui)

add_executable(bbsch src/main.c)

target_link_libraries(bbsch
        bbgui
        bblib
        m
        ${GLIB_LIBRARIES}
        ${GTK3_LIBRARIES}
        ${PEAS_LIBRARIES}
        )
