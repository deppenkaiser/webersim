cmake_minimum_required(VERSION 3.10)

project(logging LANGUAGES C)

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -D_DEBUG")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(logging STATIC)

target_sources(logging PRIVATE
    logging.c
)

target_include_directories(logging PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../threading
    ${CMAKE_CURRENT_LIST_DIR}/../string
)
