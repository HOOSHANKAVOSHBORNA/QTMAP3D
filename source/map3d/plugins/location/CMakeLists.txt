cmake_minimum_required(VERSION 3.5)

project(location LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(location SHARED
    location.cpp
    location.h
    location.json
    location.qrc
    mappin.h mappin.cpp
)

target_include_directories(location PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_include_directories(location PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../model")

target_link_libraries(location PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)
target_link_libraries(location PRIVATE model)
target_compile_definitions(location PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET location POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/liblocation.so" "${PLUGINS_OUTPUT_DIR}")
