project(utilities)

add_library(utilities
    vec.cpp vec.h
    physics.cpp physics.h
        toggle.cpp toggle.h
)

target_link_libraries(utilities PUBLIC SDL3::SDL3)
target_include_directories(utilities PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})