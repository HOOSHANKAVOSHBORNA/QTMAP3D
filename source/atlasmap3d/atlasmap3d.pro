QT -= gui
QT += widgets
QT += opengl
TEMPLATE = lib
DEFINES += ATLASMAP3D_LIBRARY

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
    core/AtlasMainWindow/AtlasMainWindow.cpp \
    core/AtlasMainWindow/NXDockWidget.cpp \
    core/AtlasMainWindow/NXDockWidgetTabBar.cpp \
    core/AtlasMainWindow/NXDockWidgetTabButton.cpp \
    core/AtlasMainWindow/NXDockWidgetTitle.cpp \
    core/DataManager/DataManager.cpp \
    core/DataManager/DataRecord.cpp \
    core/DataManager/DataTree.cpp \
    core/DataManager/FeatureStyleSettingDlg.cpp \
    core/DataManager/FontVisitor.cpp \
    core/MapController/MapController.cpp \
    core/MousePicker/MousePicker.cpp \
    core/PluginInterface/PluginInterface.cpp \
    core/PluginManager/PluginManager.cpp \
    core/SettingsManager/SettingsManager.cpp \
    core/ViewerWidget/Compass.cpp \
    core/ViewerWidget/ViewerWidget.cpp

HEADERS += \
    NameSpace.h \
    core/AtlasMainWindow/AtlasMainWindow.h \
    core/AtlasMainWindow/NXDockWidget.h \
    core/AtlasMainWindow/NXDockWidgetTabBar.h \
    core/AtlasMainWindow/NXDockWidgetTabButton.h \
    core/AtlasMainWindow/NXDockWidgetTitle.h \
    core/AtlasMainWindow/atlasmainwindow_global.h \
    core/DataManager/DataFormats.h \
    core/DataManager/DataManager.h \
    core/DataManager/DataManager_global.h \
    core/DataManager/DataRecord.h \
    core/DataManager/DataTree.h \
    core/DataManager/FeatureStyleSettingDlg.h \
    core/DataManager/FindNode.hpp \
    core/DataManager/FontVisitor.h \
    core/MapController/MapController.h \
    core/MapController/MapController_global.h \
    core/MousePicker/MousePicker.h \
    core/MousePicker/MousePicker_global.h \
    core/PluginInterface/PluginInterface.h \
    core/PluginInterface/PluginInterface_global.h \
    core/PluginManager/PluginManager.h \
    core/PluginManager/PluginManager_global.h \
    core/SettingsManager/SettingsManager.h \
    core/SettingsManager/SettingsManager_global.h \
    core/ViewerWidget/Compass.h \
    core/ViewerWidget/ViewerWidget.h \
    core/ViewerWidget/ViewerWidget_global.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

FORMS += \
    core/AtlasMainWindow/AtlasMainWindow.ui \
    core/DataManager/FeatureStyleSettingDlg.ui \
    core/DataManager/StyleTab.ui

DISTFILES +=

LIBS  = -L$$PWD/../plugins

unix:!macx: LIBS += -L$$PWD/../../osgQt/build-topic-Qt4/lib/ -losgQt5

INCLUDEPATH += $$PWD/../../osgQt/osgQt-topic-Qt4/include
DEPENDPATH += $$PWD/../../osgQt/osgQt-topic-Qt4/include

