
cmake_minimum_required(VERSION 3.5)

project(drawEllipse LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(drawEllipse SHARED
ellipse.json
ellipse.cpp
ellipse.h
drawellipse.cpp
drawellipse.h
)

target_include_directories(drawEllipse PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(drawEllipse PRIVATE Qt5::Gui Qt5::Quick Qt5::Widgets)

target_compile_definitions(drawEllipse PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET drawEllipse POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdrawEllipse.so" "${PLUGINS_OUTPUT_DIR}")
