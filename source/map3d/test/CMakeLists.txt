cmake_minimum_required(VERSION 3.14)

project(test LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Network)
find_package(Qt6 REQUIRED COMPONENTS Gui)

add_executable(test
  main.cpp
  networkManager.h networkManager.cpp
  flyableNodeTest.h flyableNodeTest.cpp
  layerTest.cpp layerTest.h
  movableNodeTest.h movableNodeTest.cpp
)
target_link_libraries(test Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Network Qt${QT_VERSION_MAJOR}::Gui)

install(TARGETS test
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

#--network----------------------------
add_custom_command(TARGET test PRE_LINK
    COMMAND mkdir ARGS "-p" "${CMAKE_CURRENT_BINARY_DIR}/../libs"
    COMMAND cp ARGS "-rf" "${CMAKE_CURRENT_SOURCE_DIR}/../libs/*" "${CMAKE_CURRENT_BINARY_DIR}/../libs/")

target_include_directories(test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/qamqp/include")
target_link_directories(test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../libs/qamqp")

target_link_libraries(test qamqp)
