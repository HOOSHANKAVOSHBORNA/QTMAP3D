
cmake_minimum_required(VERSION 3.5)

project(model LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(model SHARED
    model.h
    model.cpp
    simpleModelNode.h
    simpleModelNode.cpp
    moveableModelNode.h
    moveableModelNode.cpp
    flyableModelNode.h
    flyableModelNode.cpp
    modelAutoScaler.h
    modelautoscaler.cpp
    model.json
    model.qrc
    nodeInformationManager.h
    nodeInformationManager.cpp
    dataManager.h dataManager.cpp
    circularMenuModel.h circularMenuModel.cpp
)

target_include_directories(model PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_include_directories(model PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawLine")
target_include_directories(model PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawSphere")
target_include_directories(model PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawCircle")
target_include_directories(model PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawCone")
target_include_directories(model PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../particle")
target_include_directories(model PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../information")

target_link_libraries(model PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)
target_link_libraries(model PRIVATE drawLine)
target_link_libraries(model PRIVATE drawSphere)
target_link_libraries(model PRIVATE drawCircle)
target_link_libraries(model PRIVATE drawCone)
target_link_libraries(model PRIVATE particle)
target_link_libraries(model PRIVATE information)
target_compile_definitions(model PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET model POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libmodel.so" "${PLUGINS_OUTPUT_DIR}")
