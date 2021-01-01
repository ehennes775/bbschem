

add_library(bbgedaplugin SHARED
        bbgedaopener.c
        bbgedaopener.h
        bbgedaplugin.c
        bbgedaplugin.h
        bbgedapluginregister.c
        )

target_link_libraries(bbgedaplugin
    bbext
    )

add_custom_command(TARGET bbgedaplugin POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${CMAKE_SOURCE_DIR}/src/gedaplugin/geda.plugin $<TARGET_FILE_DIR:bbgedaplugin>
        )