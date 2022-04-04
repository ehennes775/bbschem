project(tests)

find_package(Catch2 REQUIRED)

add_executable(tests tests.c)

target_include_directories(tests PUBLIC ../src/lib)

target_link_libraries(tests
        Catch2::Catch2WithMain
        )
