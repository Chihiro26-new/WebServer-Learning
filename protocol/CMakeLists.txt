cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE PROTOCOL_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(server_protocol
    ${PROTOCOL_SRC}
)

target_include_directories(
    server_protocol
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/http/include
    ${CMAKE_CURRENT_SOURCE_DIR}/redis/include
    ${CMAKE_CURRENT_SOURCE_DIR}/echo/include
)
target_link_libraries(
    server_protocol
    server_net
)