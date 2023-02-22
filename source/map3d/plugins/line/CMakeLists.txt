
cmake_minimum_required(VERSION 3.5)

project(line LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(line SHARED
line.cpp
line.h
line.json
)

target_include_directories(line PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(line PRIVATE Qt5::Gui Qt5::Quick Qt5::Widgets)

target_compile_definitions(line PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET line POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libline.so" "${PLUGINS_OUTPUT_DIR}")
