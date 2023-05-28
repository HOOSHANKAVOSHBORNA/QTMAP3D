
cmake_minimum_required(VERSION 3.5)

project(drawCylinder LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(drawCylinder SHARED
    drawCylinder.json
    drawCylinder.qrc
    drawCylinder.cpp
    cylinder.cpp
    cylinderProperties.cpp
)

target_include_directories(drawCylinder PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(drawCylinder PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)

target_compile_definitions(drawCylinder PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET drawCylinder POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdrawCylinder.so" "${PLUGINS_OUTPUT_DIR}")
