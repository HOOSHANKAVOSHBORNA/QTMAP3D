
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

find_package(Qt6 COMPONENTS Core Quick Gui Widgets Network REQUIRED)

add_executable(Map3d
        main.cpp
        mainwindow.cpp  mainwindow.h
        plugininterface.cpp plugininterface.h
        application.cpp mapItem.h
        mapItem.cpp application.h
        pluginmanager.cpp   pluginmanager.h
        serviceManager.h serviceManager.cpp
        defenseDataManager.cpp  defenseDataManager.h
        listWindow.cpp  listWindow.h
        layerManager.cpp layerManager.h
        osgRenderNode.cpp   osgRenderNode.h
        osgViewerItem.cpp osgViewerItem.h
        mapControllerItem.cpp mapControllerItem.h
        mapObject.cpp mapObject.h
        cameraController.cpp cameraController.h
        toolboxManager.h toolboxManager.cpp
        compositeAnnotationLayer.h compositeAnnotationLayer.cpp
        utility.h utility.cpp
        searchNodeModel.h searchNodeModel.cpp
        networkManager.h networkManager.cpp
        smallMap.h smallMap.cpp
        bookmark.h bookmark.cpp
        qml.qrc
        resources.qrc
        statusBar.cpp
        statusBar.h
        locationManager.h locationManager.cpp
        qmlNode.h qmlNode.cpp
        userManager.cpp userManager.h
        filterManager.h filterManager.cpp
        layerProperty.h layerProperty.cpp
        autoScaler.h autoScaler.cpp
        splash.h splash.cpp
        settings.h settings.cpp)


target_include_directories(Map3d PUBLIC "/usr/local/include" "style/" "Components/")
target_link_directories(Map3d PUBLIC ${PLUGINS_OUTPUT_DIR})
target_link_libraries(Map3d
        PRIVATE Qt6::Core Qt6::Quick Qt6::Gui Qt6::Widgets Qt6::Network
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


#--network----------------------------
add_custom_command(TARGET Map3d PRE_LINK
    COMMAND mkdir ARGS "-p" "${CMAKE_CURRENT_BINARY_DIR}/../libs"
    COMMAND cp ARGS "-rf" "${CMAKE_CURRENT_SOURCE_DIR}/../libs/*" "${CMAKE_CURRENT_BINARY_DIR}/../libs/")

target_include_directories(Map3d PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../libs/qamqp/include")
target_link_directories(Map3d PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/../libs/qamqp")

target_link_libraries(Map3d PUBLIC qamqp)

