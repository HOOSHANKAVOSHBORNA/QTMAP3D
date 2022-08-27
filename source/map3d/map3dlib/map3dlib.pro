QT += core gui quick quickcontrols2 quickwidgets
QT += widgets

TEMPLATE = lib
DEFINES += MAP3DLIB_LIBRARY

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
    cameramanipulatorwidget.cpp \
    compasswidget.cpp \
    map3dlib.cpp \
    map3dwidget.cpp

HEADERS += \
    cameramanipulatorwidget.h \
    compasswidget.h \
    map3dlib_global.h \
    map3dlib.h \
    map3dwidget.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS +=

LIBS  +=  -losg -losgViewer -losgGA -losgDB -losgManipulator -losgSim -losgParticle -losgText -losgUtil
LIBS  +=  -lgdal -losgEarth -losgEarthFeatures -losgEarthUtil -losgEarthSymbology -losgEarthAnnotation



RESOURCES += \
    map3dlib.qrc \
    qml.qrc



#copy resource folder
defineTest(copyToDestDir) {
    files = $$1
    dir = $$2
    # replace slashes in destination path for Windows
    win32:dir ~= s,/,\\,g

    for(file, files) {
        # replace slashes in source path for Windows
        win32:file ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
    }

    export(QMAKE_POST_LINK)
}
copyToDestDir($$PWD/data, $$OUT_PWD/data)

unix:!macx: LIBS += -L$$PWD/../../osgQt/lib/ -losgQOpenGL

INCLUDEPATH += $$PWD/../../osgQt/include
DEPENDPATH += $$PWD/../../osgQt/include

DISTFILES += \
    *.qml \ \
    data/earth_files/geocentric.earth \
    data/earth_files/projected.earth \
    data/earth_files/world.tif

QMAKE_RESOURCE_FLAGS += -compress 1
#QMAKE_RESOURCE_FLAGS += -no-compress
