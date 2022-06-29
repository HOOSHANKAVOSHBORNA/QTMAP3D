QT += gui
QT += widgets

TEMPLATE = lib
CONFIG += plugin

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
    OrthoMap.cpp \
    PosterPrinter.cpp \
    SaveOrthoDSMDialog.cpp \
    SaveOrthoProjDialog.cpp

HEADERS += \
    OrthoMap.h \
    PosterPrinter.h \
    SaveOrthoDSMDialog.h \
    SaveOrthoProjDialog.h

DISTFILES += \
    OrthoMap.json

# Default rules for deployment.
unix {
    target.path = $$[QT_INSTALL_PLUGINS]/generic
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    SaveOrthoProjDialog.ui

LIBS  += -L/usr/local/lib64/ -losg -losgViewer -losgGA -losgDB -losgManipulator -losgSim -losgParticle -losgText -losgUtil
LIBS  += -L/usr/local/lib64/ -lgdal -losgEarth -losgEarthFeatures -losgEarthUtil -losgEarthSymbology -losgEarthAnnotation

INCLUDEPATH  += ../../atlasmap3d/

unix:!macx: LIBS += -L$$OUT_PWD/../../atlasmap3d/ -latlasmap3d

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../
