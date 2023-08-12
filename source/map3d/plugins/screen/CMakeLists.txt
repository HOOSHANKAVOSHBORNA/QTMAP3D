cmake_minimum_required(VERSION 3.5)

project(screen LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(screen SHARED
    screen.cpp
    screen.h
    screen.json
    screen.qrc
)

target_include_directories(screen PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(screen PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)

target_compile_definitions(screen PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET screen POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libscreen.so" "${PLUGINS_OUTPUT_DIR}")
