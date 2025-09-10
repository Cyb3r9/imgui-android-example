file(GLOB KITTY_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_library(KittyMemory STATIC ${KITTY_SOURCES})

target_include_directories(KittyMemory PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)