find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(THING1 bbsch.gresource.ui.c)
set(THING2 bbsch.gresource.ui)

add_custom_command(
        OUTPUT ${THING1}
        COMMAND ${GLIB_COMPILE_RESOURCES}
        ARGS
            --generate-source
            --sourcedir=${CMAKE_CURRENT_SOURCE_DIR}
            --target=${CMAKE_CURRENT_BINARY_DIR}/${THING1}
            ${CMAKE_CURRENT_SOURCE_DIR}/${THING2}
        VERBATIM
        MAIN_DEPENDENCY ${THING2}
        COMMENT "Compiling GUI Resources to ${THING1}"
        )

add_library(bbgui
        bbapplication.c
        bbapplication.h
        bbattributeeditor.c
        bbattributeeditor.h
        bbarctool.c
        bbarctool.h
        bbboxbuilder.c
        bbboxbuilder.h
        bbcellrenderercolorswatch.c
        bbcellrenderercolorswatch.h
        bbcirclebuilder.c
        bbcirclebuilder.h
        bbcolorcombobox.c
        bbcolorcombobox.h
        bbcoloreditor.c
        bbcoloreditor.h
        bbcopyaction.c
        bbcopyaction.h
        bbcutaction.c
        bbcutaction.h
        bbcomponentselector.c
        bbcomponentselector.h
        bbcomponentselectorplugin.c
        bbcomponentselectorplugin.h
        bbdeleteaction.c
        bbdeleteaction.h
        bbdocumentwindow.c
        bbdocumentwindow.h
        bbdocumentwindowtab.c
        bbdocumentwindowtab.h
        bbdrawingtool.c
        bbdrawingtool.h
        bbfillstyleeditor.c
        bbfillstyleeditor.h
        bbint32combobox.c
        bbint32combobox.h
        bbitembuilder.c
        bbitembuilder.h
        bbitembuildtool.c
        bbitembuildtool.h
        bblinebuilder.c
        bblinebuilder.h
        bblinestyleeditor.c
        bblinestyleeditor.h
        bbmainwindow.c
        bbmainwindow.h
        bbpasteaction.c
        bbpasteaction.h
        bbpinpropertyeditor.c
        bbpinpropertyeditor.h
        bbpropertycombobox.c
        bbpropertycombobox.h
        bbquitaction.c
        bbquitaction.h
        bbredoaction.c
        bbredoaction.h
        bbreloadaction.c
        bbreloadaction.h
        bbsaveaction.c
        bbsaveaction.h
        bbsaveallaction.c
        bbsaveallaction.h
        bbsaveasaction.c
        bbsaveasaction.h
        bbselectallaction.c
        bbselectallaction.h
        bbselectnoneaction.c
        bbselectnoneaction.h
        bbschematicwindow.c
        bbschematicwindow.h
        bbschematicwindowinner.c
        bbschematicwindowinner.h
        bbtextpropertyeditor.c
        bbtextpropertyeditor.h
        bbtoolpalette.c
        bbtoolpalette.h
        bbundoaction.c
        bbundoaction.h
        ${CMAKE_CURRENT_BINARY_DIR}/${THING1}
        )

set_source_files_properties(
        ${CMAKE_CURRENT_BINARY_DIR}/${THING1}
        PROPERTIES GENERATED TRUE
        )
