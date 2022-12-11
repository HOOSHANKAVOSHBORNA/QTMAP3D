cmake_minimum_required(VERSION 3.5)

project(earthfile LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(earthfile SHARED
  earthfile.cpp
  earthfile.h
  earthfile.json
  earthfile.qrc
)

target_include_directories(earthfile PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(earthfile PRIVATE Qt5::Gui Qt5::Quick Qt5::Widgets)

target_compile_definitions(earthfile PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET earthfile POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libearthfile.so" "${PLUGINS_OUTPUT_DIR}")
