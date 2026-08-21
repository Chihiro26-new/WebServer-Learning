cmake_minimum_required(VERSION 3.10)
project(server_net)


file(GLOB NET_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
    
)


add_library(server_net STATIC ${NET_SRC})


target_include_directories(server_net
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_link_libraries(
    server_net
    PUBLIC
    server_base
    server_protocol
)