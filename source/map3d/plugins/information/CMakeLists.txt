cmake_minimum_required(VERSION 3.5)

project(information LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(information SHARED
    information.cpp
    information.h
    information.json
    information.qrc
    mapPin.h mapPin.cpp
    statusNode.cpp statusNode.h
)

target_include_directories(information PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_include_directories(information PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../model")

target_link_libraries(information PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)
target_link_libraries(information PRIVATE model)
target_compile_definitions(information PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET information POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libinformation.so" "${PLUGINS_OUTPUT_DIR}")
