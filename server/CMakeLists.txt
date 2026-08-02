cmake_minimum_required(VERSION 3.10)

project(server)

file(GLOB SERVER_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(server_core STATIC ${SERVER_SRC})


target_include_directories(server_core
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(server_core
    PUBLIC
    server_base
)