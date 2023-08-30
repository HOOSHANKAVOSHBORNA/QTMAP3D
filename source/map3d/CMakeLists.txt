
cmake_minimum_required(VERSION 3.12)

project(map3d LANGUAGES CXX)

set(CMAKE_ENABLE_EXPORTS ON)
set(PLUGINS_OUTPUT_DIR "${CMAKE_BINARY_DIR}/plugins/bin")


add_subdirectory(map3d)
add_subdirectory(plugins)
add_subdirectory(test)
