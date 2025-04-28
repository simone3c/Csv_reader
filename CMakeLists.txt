cmake_minimum_required(VERSION 3.22.1)

project(csv_reader VERSION 0.1.0 LANGUAGES C)

set(CSV_READER_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")
set(CSV_READER_GXX_COMPILE_OPTIONS "-Wextra;-Wall;-Wfloat-equal;-Wundef;-Wshadow;-Wpointer-arith;-Wcast-align;-Wwrite-strings;")

add_subdirectory(src)