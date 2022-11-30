
cmake_minimum_required(VERSION 3.12)

project(Map3d LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BINARY_DIR "${CMAKE_SOURCE_DIR}/build")

set(CMAKE_CXX_EXTENSIONS ON)

find_package(Qt5 COMPONENTS Core Quick Gui Widgets REQUIRED)

add_executable(Map3d
        main.cpp
        osgrenderer.cpp
        osgquickwindow.cpp
        mainwindow.cpp
        plugininterface.cpp
        application.cpp
        mapcontroller.cpp
        pluginmanager.cpp
        osgcontroller.cpp
        qml.qrc
        resources.qrc)


target_include_directories(Map3d PUBLIC "/usr/local/include")
target_link_libraries(Map3d
        PRIVATE Qt5::Core Qt5::Quick Qt5::Gui Qt5::Widgets
        PUBLIC osg osgViewer osgDB osgGA OpenThreads osgManipulator osgSim osgParticle osgText osgUtil
        gdal osgEarth osgEarthFeatures osgEarthUtil osgEarthSymbology osgEarthAnnotation
        GL
        )

target_compile_definitions(Map3d PUBLIC EXTERNAL_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../resources/")
