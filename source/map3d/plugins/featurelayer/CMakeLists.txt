cmake_minimum_required(VERSION 3.5)

project(featurelayer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(featurelayer SHARED
  featurelayer.cpp
  featurelayer.h
  urldialog.cpp
  urldialog.h
  urldialog.ui
  multichoosedlg.cpp
  multichoosedlg.h
  featurelayer.json
)

target_include_directories(featurelayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(featurelayer PRIVATE Qt5::Gui Qt5::Quick Qt5::Widgets)

target_compile_definitions(featurelayer PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET featurelayer POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libfeaturelayer.so" "${PLUGINS_OUTPUT_DIR}")
