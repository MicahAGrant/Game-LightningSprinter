project(utilities)

add_library(utilities
    vec.cpp vec.h
    physics.cpp physics.h
)

target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})