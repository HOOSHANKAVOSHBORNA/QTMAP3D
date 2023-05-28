
cmake_minimum_required(VERSION 3.5)

project(drawimage LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(drawimage SHARED
drawImage.cpp
drawImage.h
drawImage.json
drawImage.qrc
imageProperties.cpp
imageProperties.h
)

target_include_directories(drawimage PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(drawimage PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)

target_compile_definitions(drawimage PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET drawimage POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdrawimage.so" "${PLUGINS_OUTPUT_DIR}")
