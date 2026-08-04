file(GLOB HTTP_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

add_library(http_handler
    ${HTTP_SRC}
)

target_include_directories(
    http_handler
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)


target_link_libraries(
    http_handler
    PUBLIC
    server_net
)

然后 redis：

protocol/redis/CMakeLists.txt

file(GLOB REDIS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)


target_include_directories(
    echo_handler
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    echo_handler
    PUBLIC
    server_net
)