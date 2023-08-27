cmake_minimum_required(VERSION 3.5)

project(particle LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(particle SHARED
    particle.cpp
    particle.h
    particle.json
    particle.qrc
    explosion.h explosion.cpp
    fireSmoke.h fireSmoke.cpp
    snow.h snow.cpp
    rain.h rain.cpp
    fog.h fog.cpp
)

target_include_directories(particle PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")

target_link_libraries(particle PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)
target_compile_definitions(particle PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET particle POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libparticle.so" "${PLUGINS_OUTPUT_DIR}")
