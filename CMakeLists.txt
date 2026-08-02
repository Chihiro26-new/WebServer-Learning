cmake_minimum_required(VERSION 3.10)

project(WebServer)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(base)
add_subdirectory(server)

add_executable(WebServer Main.cpp)
target_link_libraries(WebServer server_core)