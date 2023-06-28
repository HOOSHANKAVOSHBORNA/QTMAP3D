
cmake_minimum_required(VERSION 3.5)

project(drawline LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(drawLine SHARED
drawLine.cpp
drawLine.h
drawLine.json
lineNode.cpp
lineNode.h
drawshapeautoscaler.cpp
drawshapeautoscaler.h
drawLine.qrc
lineProperties.cpp
lineProperties.h
measureHeight.cpp
measureHeight.h
)

target_include_directories(drawLine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(drawLine PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)

target_compile_definitions(drawLine PRIVATE CPTEST_LIBRARY)
target_include_directories(drawLine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawShape")
target_link_libraries(drawLine PRIVATE drawShape)



add_custom_command(TARGET drawLine POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdrawLine.so" "${PLUGINS_OUTPUT_DIR}")
