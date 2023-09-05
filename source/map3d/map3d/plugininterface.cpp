
#include "plugininterface.h"
#include "mainwindow.h"
#include <QQuickItem>
#include <QTimer>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtQml>

//-------------------------------------------------------------------------
MapItem *PluginInterface::mMapItem;
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

QQmlEngine *PluginInterface::qmlEngine() const
{
    return mQmlEngine;
}

MainWindow *PluginInterface::mainWindow() const
{
    return mMainWindow;
}

void PluginInterface::setMainWindow(MainWindow *mainWindow)
{
    mMainWindow = mainWindow;
    mMapItem = mainWindow->getMapItem();
    mQmlEngine = mainWindow->getQmlEngine();
    mToolbox = dynamic_cast<Toolbox*>(mainWindow->toolbox()->sourceModel());
}

DefenseDataManager *PluginInterface::defenseDataManager() const
{
    return mDefenseDataManager;
}

void PluginInterface::setDefenseDataManager(DefenseDataManager *defenseDataManager)
{
    mDefenseDataManager = defenseDataManager;
}

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

ServiceManager *PluginInterface::serviceManager() const
{
    return mServiceManager;
}

void PluginInterface::setServiceManager(ServiceManager *newServiceManager)
{
    mServiceManager = newServiceManager;
}
