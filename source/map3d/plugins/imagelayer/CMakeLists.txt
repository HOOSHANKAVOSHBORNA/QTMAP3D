cmake_minimum_required(VERSION 3.5)

project(imagelayer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(imagelayer SHARED
  imagelayer.cpp
  imagelayer.h
  urldialog.cpp
  urldialog.h
  urldialog.ui
  multichoosedlg.cpp
  multichoosedlg.h
  imagelayer.json
  imagelayer.qrc
)

target_include_directories(imagelayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(imagelayer PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)

target_compile_definitions(imagelayer PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET imagelayer POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libimagelayer.so" "${PLUGINS_OUTPUT_DIR}")
