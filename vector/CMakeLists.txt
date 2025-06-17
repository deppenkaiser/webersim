cmake_minimum_required(VERSION 3.10)

project(vector LANGUAGES C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(vector STATIC)

target_sources(vector PRIVATE
    vector.c
)

target_include_directories(vector PRIVATE
)

target_link_libraries(vector
)
