cmake_minimum_required(VERSION 3.10)


add_library(server_protocol INTERFACE)


target_include_directories(
    server_protocol
    INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)