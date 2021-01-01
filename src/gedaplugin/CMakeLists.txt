find_program(GLIB_COMPILE_RESOURCES NAMES glib-compile-resources REQUIRED)

set(THING1 bbgedaplugin.gresource.ui.c)
set(THING2 bbgedaplugin.gresource.ui)

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

set_source_files_properties(
        ${CMAKE_CURRENT_BINARY_DIR}/${THING1}
        PROPERTIES GENERATED TRUE
        )

add_library(bbgedaplugin SHARED
        bbgedaeditor.c
        bbgedaeditor.h
        bbgedaopener.c
        bbgedaopener.h
        bbgedaplugin.c
        bbgedaplugin.h
        bbgedapluginregister.c
        bbgedareader.c
        bbgedareader.h
        bbgedaview.c
        bbgedaview.h
        ${CMAKE_CURRENT_BINARY_DIR}/${THING1}
        )

target_link_libraries(bbgedaplugin
        bbext
        )

add_custom_command(TARGET bbgedaplugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/gedaplugin/geda.plugin $<TARGET_FILE_DIR:bbgedaplugin>
        )