file(GLOB LOGGING_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(server_logging
    ${LOGGING_SOURCES}
)

target_include_directories(server_logging
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(server_logging
    PUBLIC
        server_base
)