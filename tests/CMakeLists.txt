

add_executable(tests
        lib2/TestSchematic.cpp
        lib2/TestCircleItem.cpp
        lib2/TestLineItem.cpp
        )

target_include_directories(tests PUBLIC
        ../src/lib2
        )

target_link_libraries(tests
        bblib2
        Catch2::Catch2WithMain
        )
