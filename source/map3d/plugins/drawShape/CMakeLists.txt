cmake_minimum_required(VERSION 3.5)

project(drawShape LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(drawShape SHARED
  drawShapeQml.qrc
  drawShape.cpp
  drawShape.qrc
  spherenodeeditor.cpp
  drawShape.json
  drawshapeautoscaler.cpp
  property.cpp

)

target_include_directories(drawShape PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d"
"${CMAKE_CURRENT_SOURCE_DIR}/../../map3d/style")
target_link_libraries(drawShape PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)

target_compile_definitions(drawShape PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET drawShape POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdrawShape.so" "${PLUGINS_OUTPUT_DIR}")
