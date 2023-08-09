
cmake_minimum_required(VERSION 3.5)

project(drawSphere LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(drawSphere SHARED
    drawSphere.json
    drawSphere.cpp
    drawSphere.qrc
    sphereProperty.cpp
    sphereNode.cpp
    drawSphere.h
    sphereProperty.h
    sphereNode.h
)

target_include_directories(drawSphere PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(drawSphere PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)
target_include_directories(drawSphere PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawShape")

target_link_libraries(drawSphere PRIVATE drawShape)

target_compile_definitions(drawSphere PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET drawSphere POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdrawSphere.so" "${PLUGINS_OUTPUT_DIR}")
