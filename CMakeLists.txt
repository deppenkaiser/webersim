cmake_minimum_required(VERSION 3.10)
project(webersim LANGUAGES C)

set(SOURCES
    main.c
)

set(LIBRARIES
    sm
    threading
    string
    physics
    m
)

set(INCLUDES
    ${CMAKE_CURRENT_LIST_DIR}/sm
    ${CMAKE_CURRENT_LIST_DIR}/threading
    ${CMAKE_CURRENT_LIST_DIR}/physics
)

add_subdirectory(sm)
add_subdirectory(threading)
add_subdirectory(string)
add_subdirectory(physics)
add_subdirectory(logging)
add_subdirectory(vector)
add_executable(webersim ${SOURCES})

target_link_libraries(webersim ${LIBRARIES})
target_include_directories(webersim PRIVATE ${INCLUDES})

target_compile_definitions(webersim PRIVATE
    $<$<CONFIG:Debug>:DEBUG>
)
