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


add_custom_command(
        OUTPUT bbsch.gresource.ui.c
        COMMAND glib-compile-resources --generate-source src/gui/bbsch.gresource.ui
        )


add_executable(bbsch
        src/gui/bbapplication.c
        src/gui/bbapplication.h
        src/gui/bbattributeeditor.c
        src/gui/bbattributeeditor.h
        src/gui/bbboxbuilder.c
        src/gui/bbboxbuilder.h
        src/gui/bbcellrenderercolorswatch.c
        src/gui/bbcellrenderercolorswatch.h
        src/gui/bbcirclebuilder.c
        src/gui/bbcirclebuilder.h
        src/gui/bbcolorcombobox.c
        src/gui/bbcolorcombobox.h
        src/gui/bbcoloreditor.c
        src/gui/bbcoloreditor.h
        src/gui/bbcomponentselector.c
        src/gui/bbcomponentselector.h
        src/gui/bbcomponentselectorplugin.c
        src/gui/bbcomponentselectorplugin.h
        src/gui/bbdocumentwindow.c
        src/gui/bbdocumentwindow.h
        src/gui/bbdocumentwindowtab.c
        src/gui/bbdocumentwindowtab.h
        src/gui/bbdrawingtool.c
        src/gui/bbdrawingtool.h
        src/gui/bbfillstyleeditor.c
        src/gui/bbfillstyleeditor.h
        src/gui/bbint32combobox.c
        src/gui/bbint32combobox.h
        src/gui/bbitembuilder.c
        src/gui/bbitembuilder.h
        src/gui/bbitembuildtool.c
        src/gui/bbitembuildtool.h
        src/gui/bblinebuilder.c
        src/gui/bblinebuilder.h
        src/gui/bblinestyleeditor.c
        src/gui/bblinestyleeditor.h
        src/gui/bbmainwindow.c
        src/gui/bbmainwindow.h
        src/gui/bbpinpropertyeditor.c
        src/gui/bbpinpropertyeditor.h
        src/gui/bbpropertycombobox.c
        src/gui/bbpropertycombobox.h
        src/gui/bbschematicwindow.c
        src/gui/bbschematicwindow.h
        src/gui/bbschematicwindowinner.c
        src/gui/bbschematicwindowinner.h
        src/gui/bbschematicwrapper.c
        src/gui/bbschematicwrapper.h
        src/gui/bbtextpropertyeditor.c
        src/gui/bbtextpropertyeditor.h
        src/gui/bbtoolpalette.c
        src/gui/bbtoolpalette.h
        src/lib/bbabsolutelineto.c
        src/lib/bbabsolutelineto.h
        src/lib/bbabsolutemoveto.c
        src/lib/bbabsolutemoveto.h
        src/lib/bbadjustablefillstyle.c
        src/lib/bbadjustablefillstyle.h
        src/lib/bbadjustableitemcolor.c
        src/lib/bbadjustableitemcolor.h
        src/lib/bbadjustablelinestyle.c
        src/lib/bbadjustablelinestyle.h
        src/lib/bbangle.c
        src/lib/bbangle.h
        src/lib/bbbounds.c
        src/lib/bbbounds.h
        src/lib/bbboundscalculator.c
        src/lib/bbboundscalculator.h
        src/lib/bbcaptype.c
        src/lib/bbcaptype.h
        src/lib/bbcoord.c
        src/lib/bbcoord.h
        src/lib/bbdashtype.c
        src/lib/bbdashtype.h
        src/lib/bbfillstyle.c
        src/lib/bbfillstyle.h
        src/lib/bbfilltype.c
        src/lib/bbfilltype.h
        src/lib/bbgraphicarc.c
        src/lib/bbgraphicarc.h
        src/lib/bbgraphicbox.c
        src/lib/bbgraphicbox.h
        src/lib/bbgraphiccircle.c
        src/lib/bbgraphiccircle.h
        src/lib/bbgraphicline.c
        src/lib/bbgraphicline.h
        src/lib/bbgraphicpath.c
        src/lib/bbgraphicpath.h
        src/lib/bbhashtable.c
        src/lib/bbhashtable.h
        src/lib/bbitemparams.c
        src/lib/bbitemparams.h
        src/lib/bbitemrenderer.c
        src/lib/bbitemrenderer.h
        src/lib/bblinestyle.c
        src/lib/bblinestyle.h
        src/lib/bbpathcommand.c
        src/lib/bbpathcommand.h
        src/lib/bbpred.c
        src/lib/bbpred.h
        src/lib/bbqueryfunc.h
        src/lib/bbrelativelineto.c
        src/lib/bbrelativelineto.h
        src/lib/bbrelativemoveto.c
        src/lib/bbrelativemoveto.h
        src/lib/bbschematic.c
        src/lib/bbschematic.h
        src/lib/bbschematicitem.c
        src/lib/bbschematicitem.h
        src/lib/bbsweep.c
        src/lib/bbsweep.h
        src/lib/bbvaluecount.c
        src/lib/bbvaluecount.h
        src/gui/bbsch.gresource.ui.c
        src/main.c
        )

target_link_libraries(bbsch
        m
        ${GLIB_LIBRARIES}
        ${GTK3_LIBRARIES}
        ${PEAS_LIBRARIES}
        )
