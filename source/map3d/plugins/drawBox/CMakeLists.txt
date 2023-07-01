
cmake_minimum_required(VERSION 3.5)

project(drawbox LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(drawBox SHARED
box.cpp
box.h
drawBox.cpp
drawBox.h
drawBox.json
drawBox.qrc
boxProperties.cpp
boxProperties.h
)

target_include_directories(drawBox PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d"
    "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d/style")
target_link_libraries(drawBox PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)
target_include_directories(drawBox PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawShape")

target_link_libraries(drawBox PRIVATE drawShape)
target_compile_definitions(drawBox PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET drawBox POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdrawBox.so" "${PLUGINS_OUTPUT_DIR}")
