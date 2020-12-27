add_executable(
    bbangletest
    bbangletest.c
    )

target_link_libraries(bbangletest
    bblib
    bbext
    m
    ${GLIB_LIBRARIES}
    ${GTK3_LIBRARIES}
    ${PEAS_LIBRARIES}
    )

add_executable(
    bbcoordtest
    bbcoordtest.c
    )

target_link_libraries(bbcoordtest
    bblib
    bbext
    m
    ${GLIB_LIBRARIES}
    ${GTK3_LIBRARIES}
    ${PEAS_LIBRARIES}
    )

add_executable(
    bbpathscannertest
    bbpathscannertest.c
    )

target_link_libraries(bbpathscannertest
    bblib
    bbext
    m
    ${GLIB_LIBRARIES}
    ${GTK3_LIBRARIES}
    ${PEAS_LIBRARIES}
    )




add_test(
    bbangletest
    gtester bbangletest
    )

add_test(
    bbcoordtest
    gtester bbcoordtest
    )

add_test(
    bbpathscannertest
    gtester bbpathscannertest
    )
