file(GLOB REDIS_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp
)

target_include_directories(
    redis_handler
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    redis_handler
    PUBLIC
    server_net
)