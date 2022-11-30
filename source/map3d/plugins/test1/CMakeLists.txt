cmake_minimum_required(VERSION 3.5)

project(CPTest LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Gui Quick REQUIRED)

add_library(CPTest SHARED
  test1plugin.cpp
  test1plugin.h
  test1plugin.json
  test1plugin.qrc
)

target_include_directories(CPTest PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_link_libraries(CPTest PRIVATE Qt5::Gui Qt5::Quick)

target_compile_definitions(CPTest PRIVATE CPTEST_LIBRARY)
