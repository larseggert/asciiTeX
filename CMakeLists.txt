cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(asciiTeX C)

set(CMAKE_CXX_STANDARD 11)

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
                src/utils.c
)
