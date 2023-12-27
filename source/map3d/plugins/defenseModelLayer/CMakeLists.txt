cmake_minimum_required(VERSION 3.5)

project(defenseModelLayer LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Gui Quick Widgets REQUIRED)

add_library(defenseModelLayer SHARED
  aircraftTableModel.cpp
  assignmentTableModel.cpp
  aircraftDataManager.cpp
  contextMenu.cpp
  aircraftInfoItem.cpp
  stationInformation.cpp
  stationDataManager.cpp
  systemInfoItem.cpp
  systemDataManager.cpp
  stationTableModel.cpp
  systemTableModel.cpp
  defenseModelLayer.cpp
  defenseModelNode.cpp
  defenseModelNodeAutoScaler.cpp
  dataManager.cpp
  aircraftModelNode.cpp
  rocket.cpp
  stationModelNode.cpp
  systemModelNode.cpp
  truck.cpp
  defenseModelLayer.json
  defenseModelLayer.qrc
  defenseModelLayerqml.qrc
  truckf.cpp
  truckl.cpp
  trucks.cpp
)

target_include_directories(defenseModelLayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../../map3d")
target_include_directories(defenseModelLayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawCircle")
#target_include_directories(defenseModelLayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawLine")
target_include_directories(defenseModelLayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawAnnotation")
target_include_directories(defenseModelLayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawSphere")
#target_include_directories(defenseModelLayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../drawPolygon")
target_include_directories(defenseModelLayer PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../information")

#target_link_libraries(defenseModelLayer PRIVATE drawPolygon)
target_link_libraries(defenseModelLayer PRIVATE drawCircle)
target_link_libraries(defenseModelLayer PRIVATE drawSphere)
#target_link_libraries(defenseModelLayer PRIVATE drawLine)
target_link_libraries(defenseModelLayer PRIVATE drawAnnotation)
target_link_libraries(defenseModelLayer PRIVATE information)
target_link_libraries(defenseModelLayer PRIVATE Qt6::Gui Qt6::Quick Qt6::Widgets)

target_compile_definitions(defenseModelLayer PRIVATE CPTEST_LIBRARY)

add_custom_command(TARGET defenseModelLayer POST_BUILD
    COMMAND mkdir ARGS "-p" "${PLUGINS_OUTPUT_DIR}"
    COMMAND cp ARGS "${CMAKE_CURRENT_BINARY_DIR}/libdefenseModelLayer.so" "${PLUGINS_OUTPUT_DIR}")
