cmake_minimum_required(VERSION 3.10)

project(WebServer)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(base)
add_subdirectory(protocol)
add_subdirectory(net)
add_subdirectory(server)
add_executable(WebServer Main.cpp)

target_link_libraries(
    WebServer
    server_core
    server_protocol
)