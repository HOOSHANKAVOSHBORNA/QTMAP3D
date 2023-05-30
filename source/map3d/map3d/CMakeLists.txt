
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

find_package(Qt6 COMPONENTS Core Quick Gui Widgets REQUIRED)

add_executable(Map3d
        main.cpp
        mainwindow.cpp  mainwindow.h
        plugininterface.cpp plugininterface.h
        application.cpp mapItem.h
        mapItem.cpp application.h
        pluginmanager.cpp   pluginmanager.h
        defenseDataManager.cpp  defenseDataManager.h
        listwindow.cpp  listwindow.h
        layersmodel.cpp layersmodel.h
        osgRenderNode.cpp   osgRenderNode.h
        mapControllerItem.cpp mapControllerItem.h
        qml.qrc
        resources.qrc)


target_include_directories(Map3d PUBLIC "/usr/local/include")
target_link_directories(Map3d PUBLIC ${PLUGINS_OUTPUT_DIR})
target_link_libraries(Map3d
        PRIVATE Qt6::Core Qt6::Quick Qt6::Gui Qt6::Widgets
        PUBLIC osg osgViewer osgDB osgGA OpenThreads osgManipulator osgSim osgParticle osgText osgUtil
        gdal osgEarth osgEarthFeatures osgEarthUtil osgEarthSymbology osgEarthAnnotation
        GL
        )

target_compile_definitions(Map3d PUBLIC EXTERNAL_RESOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../data/")
target_compile_definitions(Map3d PUBLIC PLUGINS_OUTPUT_DIR="${PLUGINS_OUTPUT_DIR}/")


add_custom_command(TARGET Map3d POST_BUILD
    COMMAND mkdir ARGS "-p" "${CMAKE_CURRENT_BINARY_DIR}/../data"
    COMMAND cp ARGS "-rf" "${CMAKE_CURRENT_SOURCE_DIR}/../data/*" "${CMAKE_CURRENT_BINARY_DIR}/../data/")

add_custom_command(TARGET Map3d POST_BUILD
        COMMAND cp ARGS "-rf" "${CMAKE_CURRENT_SOURCE_DIR}/moon_1024x512.jpg" "${CMAKE_CURRENT_BINARY_DIR}/")

add_custom_command(TARGET Map3d POST_BUILD
        COMMAND mkdir ARGS "-p" "${CMAKE_CURRENT_BINARY_DIR}/Images")
add_custom_command(TARGET Map3d POST_BUILD
        COMMAND cp ARGS "-rf" "${CMAKE_CURRENT_SOURCE_DIR}/Images/smoke.rgb" "${CMAKE_CURRENT_BINARY_DIR}/Images/")
add_custom_command(TARGET Map3d POST_BUILD
        COMMAND cp ARGS "-rf" "${CMAKE_CURRENT_SOURCE_DIR}/Images/continous_smoke.rgb" "${CMAKE_CURRENT_BINARY_DIR}/Images/")





