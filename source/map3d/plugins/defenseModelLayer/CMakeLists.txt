cmake_minimum_required(VERSION 3.5)

project(defenseModelLayer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(defenseModelLayer SHARED
  assignmentModel.cpp
  aircraftTableModel.cpp
  contextMenu.cpp
  aircraftInformation.cpp
  stationInformation.cpp
  systemInformation.cpp
  stationTableModel.cpp
  systemTableModel.cpp
  defenseModelLayer.cpp
  defenseModelNode.cpp
  defenseModelNodeAutoScaler.cpp
  aircraftModelNode.cpp
  rocket.cpp
  stationModelNode.cpp
  systemModelNode.cpp
  truck.cpp
  defenseModelLayer.json
  defenseModelLayer.qrc
  defenseModelLayerqml.qrc
  dataManager.cpp
  truckf.cpp
  truckl.cpp
  trucks.cpp
)

target_include_directories(defenseModelLayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_include_directories(defenseModelLayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawshapes")
target_link_libraries(defenseModelLayer PRIVATE drawshapes)
target_link_libraries(defenseModelLayer PRIVATE Qt5::Gui Qt5::Quick Qt5::Widgets)

target_compile_definitions(defenseModelLayer PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET defenseModelLayer POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdefenseModelLayer.so" "${PLUGINS_OUTPUT_DIR}")
