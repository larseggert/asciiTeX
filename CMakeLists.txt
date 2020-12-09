cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(asciiTeX C)

set(CMAKE_C_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug
        CACHE STRING "Set build type (default Debug)" FORCE)
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
    if(MSVC)
        add_compile_options(/W4 /WX)
    else()
        add_compile_options(-Wall -Wextra -pedantic -Werror) # -Weverything
        string(APPEND CMAKE_C_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address,undefined")
        string(APPEND CMAKE_LINKER_FLAGS_DEBUG " -fno-omit-frame-pointer -fsanitize=address,undefined")
    endif()
endif()

add_executable(asciitex
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

file(GLOB test_inputs "examples/*.tex")

install(TARGETS asciitex RUNTIME)
install(FILES ${test_inputs} DESTINATION "${CMAKE_INSTALL_PREFIX}/share/doc/asciitex")
install(FILES man/asciitex.1 DESTINATION "${CMAKE_INSTALL_PREFIX}/share/man/man1")

enable_testing()

file(GLOB test_inputs "examples/*.tex")

foreach(test_input ${test_inputs})
    file(RELATIVE_PATH test_file ${CMAKE_CURRENT_SOURCE_DIR}/examples ${test_input})
    string(REPLACE ".tex" "" test ${test_file})
    string(REPLACE ".tex" ".txt" expected_result ${test_input})
    add_custom_command(TARGET asciitex
                       POST_BUILD
                       WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                       COMMAND $<TARGET_FILE:asciitex> -f ${test_input} > ${test}.txt
                       )
    add_test(NAME ${test}
             WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
             COMMAND ${CMAKE_COMMAND} -E compare_files ${expected_result} ${test}.txt
             )
endforeach()
