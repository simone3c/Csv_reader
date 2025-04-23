cmake_minimum_required(VERSION 3.22.1)

project(Csv_reader VERSION 0.1.0 LANGUAGES C)

set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include/")
set(GXX_COMPILE_OPTIONS "-Wextra;-Wall;-Wfloat-equal;-Wundef;-Wshadow;-Wpointer-arith;-Wcast-align;-Wwrite-strings;")

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

add_subdirectory(src)