cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(asciiTeX C)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_BUILD_TYPE Debug)

string(APPEND CMAKE_C_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address,undefined")
string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address,undefined")

if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable (asciitex
                src/array.c
                src/asciiTeX.c
                src/brace.c
                src/dim.c
                src/draw.c
                src/frac.c
                src/limit.c
                src/main.c
                src/ouline.c
                src/sqrt.c
                src/sscript.c
                src/symbols.c
                src/text.c
                src/utils.c
)

enable_testing()

add_test(NAME test
         COMMAND asciitex -f ${PROJECT_SOURCE_DIR}/EXAMPLES)
