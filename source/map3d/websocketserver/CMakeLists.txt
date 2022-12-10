
cmake_minimum_required(VERSION 3.12)

project(websocketserver LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS ON)

find_package(Qt5 COMPONENTS Core WebSockets REQUIRED)

add_executable(websocketserver
        main.cpp
        createdata.cpp
        server.cpp
        createdata.h
	server.h
	websocketserver.qrc
	)

target_link_libraries(websocketserver
        PRIVATE Qt5::Core Qt5::WebSockets
        )
