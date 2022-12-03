cmake_minimum_required(VERSION 3.5)

project(Test1Plugin LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Gui Quick REQUIRED)

add_library(Test1Plugin SHARED
  test1plugin.cpp
  test1plugin.h
  test1plugin.json
  test1plugin.qrc
)

target_include_directories(Test1Plugin PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(Test1Plugin PRIVATE Qt5::Gui Qt5::Quick)

target_compile_definitions(Test1Plugin PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET Test1Plugin POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libTest1Plugin.so" "${PLUGINS_OUTPUT_DIR}")
