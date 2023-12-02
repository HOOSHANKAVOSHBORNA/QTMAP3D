
cmake_minimum_required(VERSION 3.5)

project(combatModelNode LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(combatModelNode SHARED
    combatModelNode.h
    combatModelNode.cpp
    combatModelNode.qrc
    combatModelNode.json
    bulletNode.h    bulletNode.cpp
    combatManager.h combatManager.cpp
    assignLine.h    assignLine.cpp
)

target_include_directories(combatModelNode PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_include_directories(combatModelNode PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawSphere")
target_include_directories(combatModelNode PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawCircle")
target_include_directories(combatModelNode PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawCone")
target_include_directories(combatModelNode PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../particle")
target_include_directories(combatModelNode PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../information")
target_include_directories(combatModelNode PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../model")

target_link_libraries(combatModelNode PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)
target_link_libraries(combatModelNode PRIVATE drawSphere)
target_link_libraries(combatModelNode PRIVATE drawCircle)
target_link_libraries(combatModelNode PRIVATE drawCone)
target_link_libraries(combatModelNode PRIVATE particle)
target_link_libraries(combatModelNode PRIVATE information)
target_link_libraries(combatModelNode PRIVATE model)
target_compile_definitions(combatModelNode PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET combatModelNode POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libcombatModelNode.so" "${PLUGINS_OUTPUT_DIR}")
