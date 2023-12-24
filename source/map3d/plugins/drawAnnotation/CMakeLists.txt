cmake_minimum_required(VERSION 3.5)

project(drawAnnotation LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(drawAnnotation SHARED
  drawAnnotation.h
  drawAnnotation.cpp
  property.h
  property.cpp
  lineNode.h
  lineNode.cpp
  lineProperty.h
  lineProperty.cpp
  polygonProperty.cpp
  polygonProperty.h
  measureHeight.h
  measureHeight.cpp
  polygon.h
  polygon.cpp
  drawAnnotation.json
  drawAnnotation.qrc
  drawAnnotationQml.qrc
)

target_include_directories(drawAnnotation PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d"
"${CMAKE_CURRENT_SOURCE_DIR}/../../map3d/style")
target_include_directories(drawAnnotation PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../information")
target_link_libraries(drawAnnotation PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)
target_link_libraries(drawAnnotation PRIVATE information)
target_compile_definitions(drawAnnotation PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET drawAnnotation POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdrawAnnotation.so" "${PLUGINS_OUTPUT_DIR}")
