cmake_minimum_required(VERSION 3.10)
project(server_base)


file(GLOB BASE_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(server_base STATIC ${BASE_SRC})

target_include_directories(server_base
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)