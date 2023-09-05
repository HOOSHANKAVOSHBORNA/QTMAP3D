
#include "plugininterface.h"
#include "mainwindow.h"
#include <QQuickItem>
#include <QTimer>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQmlEngine>

//UIHandle::UIHandle(MainWindow *mainWindow, QObject *parent) : QObject(parent)
//{
//    mMainWindow = mainWindow;
//}
//-------------------------------------------------------------------------
MapItem *PluginInterface::mMapItem;
//UIHandle *PluginInterface::mUiHandle;
DefenseDataManager *PluginInterface::mDefenseDataManager;
QQmlEngine *PluginInterface::mQmlEngine;
Toolbox *PluginInterface::mToolbox;
MainWindow *PluginInterface::mMainWindow;
ServiceManager *PluginInterface::mServiceManager;

PluginInterface::PluginInterface(QObject *parent):
    QObject(parent)
{

}

MapItem *PluginInterface::mapItem() const
{
    return mMapItem;
}
void PluginInterface::setMapItem(MapItem *mapItem)
{
    mMapItem = mapItem;
    //    bool isSetHandler = false;
    //    for(auto eventHandler :mMapItem->getViewer()->getEventHandlers()){
    //        auto pli = dynamic_cast<PluginInterface*>(eventHandler.get());
    //        if(pli){
    //            isSetHandler = true;
    //            break;
    //        }
    //    }
    //    if(!isSetHandler)
    //        mMapItem->getViewer()->addEventHandler(this);
}

QQmlEngine *PluginInterface::qmlEngine() const
{
    return mQmlEngine;
}

void PluginInterface::setQmlEngine(QQmlEngine *newQmlEngine)
{
    mQmlEngine = newQmlEngine;
}

//UIHandle *PluginInterface::uiHandle() const
//{
//    return mUiHandle;
//}

//void PluginInterface::setUiHandle(UIHandle *newUiHandle)
//{
//    mUiHandle = newUiHandle;
//}

MainWindow *PluginInterface::mainWindow() const
{
    return mMainWindow;
}

void PluginInterface::setMainWindow(MainWindow *mainWindow)
{
    mMainWindow = mainWindow;
}

DefenseDataManager *PluginInterface::defenseDataManager() const
{
    return mDefenseDataManager;
}

void PluginInterface::setDefenseDataManager(DefenseDataManager *defenseDataManager)
{
    mDefenseDataManager = defenseDataManager;
}

//bool PluginInterface::handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa)
//{
//    // Get view and event adaptor
//    osgViewer::View *view = dynamic_cast<osgViewer::View *>(&aa);

//    if (!view)
//    {
//        return false;
//    }
//    switch (ea.getEventType())
//    {
//    case osgGA::GUIEventAdapter::FRAME:
//        return frameEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::KEYDOWN):
//        return keyPressEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::KEYUP):
//        return keyReleaseEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::PUSH):
//        return mousePressEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::RELEASE):
//        return mouseReleaseEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::DOUBLECLICK):
//        return mouseDoubleClickEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::MOVE):
//        return mouseMoveEvent(ea, aa);
//    case (osgGA::GUIEventAdapter::SCROLL):
//        return wheelEvent(ea, aa);
//    default:

//        return false;
//    }
//}

QString PluginInterface::name() const
{
    return mName;
}

void PluginInterface::setName(const QString &newName)
{
    mName = newName;
}

Toolbox *PluginInterface::toolbox() const
{
    return mToolbox;
}

void PluginInterface::setToolbox(Toolbox *newToolbox)
{
    mToolbox = newToolbox;
}

ServiceManager *PluginInterface::serviceManager() const
{
    return mServiceManager;
}

void PluginInterface::setServiceManager(ServiceManager *newServiceManager)
{
    mServiceManager = newServiceManager;
}
