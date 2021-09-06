cmake_minimum_required(VERSION 3.12)
project(Tests)
add_compile_definitions(UNIT_TESTS)
add_executable(Tests
        main.c
        ../st25dv/st25dv.c
        ../st25dv/Mazer.c
        picoMock.c)