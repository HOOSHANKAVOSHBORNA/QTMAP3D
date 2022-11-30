
cmake_minimum_required(VERSION 3.12)

project(QTMAP3D LANGUAGES CXX)

set(PLUGINS_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/plugins/bin")


add_subdirectory(map3d)
add_subdirectory(plugins)


