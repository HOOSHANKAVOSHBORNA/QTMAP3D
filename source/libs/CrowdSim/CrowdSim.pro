QT -= gui

TEMPLATE = lib
DEFINES += CROWDSIM_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    CrowdSim.cpp \
    PathPlanner.cpp \
    agent.cpp \
    cell.cpp \
    grid.cpp \
    libpedsim/ped_agent.cpp \
    libpedsim/ped_obstacle.cpp \
    libpedsim/ped_outputwriter.cpp \
    libpedsim/ped_scene.cpp \
    libpedsim/ped_tree.cpp \
    libpedsim/ped_vector.cpp \
    libpedsim/ped_waypoint.cpp \
    micropather.cpp \
    obstacle.cpp \
    scene.cpp \
    tree.cpp \
    waypoint.cpp

HEADERS += \
    CrowdSim.h \
    PathPlanner.h \
    agent.h \
    cell.h \
    config.h \
    grid.h \
    libpedsim/ped_agent.h \
    libpedsim/ped_includes.h \
    libpedsim/ped_obstacle.h \
    libpedsim/ped_outputwriter.h \
    libpedsim/ped_scene.h \
    libpedsim/ped_tree.h \
    libpedsim/ped_vector.h \
    libpedsim/ped_waypoint.h \
    micropather.h \
    obstacle.h \
    scene.h \
    tree.h \
    waypoint.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

DISTFILES +=
