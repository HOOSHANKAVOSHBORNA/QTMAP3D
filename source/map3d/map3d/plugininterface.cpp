
#include "plugininterface.h"
#include "mainwindow.h"
#include <QQuickItem>
#include <QTimer>
#include <QQmlComponent>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QtQml>
#include "bookmark.h"
//-------------------------------------------------------------------------

PluginInterface::PluginInterface(QObject *parent):
    QObject(parent)
{

}

MapControllerItem *PluginInterface::mapItem() const
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
    mToolbox = dynamic_cast<Toolbox*>(mainWindow->getToolboxManager()->getToolboxProxyModel()->sourceModel());
    mBookmarkManager = mainWindow->getBookmarkManager();
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

void PluginInterface::setPluginsMap(const QMap<QString, PluginInterface *> &newPluginsMap)
{
    mPluginsMap = newPluginsMap;
}

PluginInterface *PluginInterface::getPlugin(QString name)
{
    if(mPluginsMap.contains(name))
        return mPluginsMap[name];
    return nullptr;
}

int PluginInterface::layerId()
{
    return mLayerId++;
}

int PluginInterface::nodeId()
{
    return mNodeId++;
}

BookmarkManager *PluginInterface::bookmarkManager() const
{
    return mBookmarkManager;
}
