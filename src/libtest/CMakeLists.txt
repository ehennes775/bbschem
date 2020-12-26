

add_executable(
    bbpathscannertest
    bbpathscannertest.c
    )

add_test(
    bbpathscannertest
    gtester bbpathscannertest
    )

target_link_libraries(bbpathscannertest
        bblib
        bbext
        m
        ${GLIB_LIBRARIES}
        ${GTK3_LIBRARIES}
        ${PEAS_LIBRARIES}
        )
