#include "plugininterface.h"
#include "pluginmanager.h"
#include "toolbarwidget.h"

#include <QPluginLoader>
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QDir>


PluginManager::PluginManager(Map3dlib* mainWindow, Map3dWidget* map3dWidget, ToolBarWidget *toolBar)
{
    mMap3dWidget = map3dWidget;
    mToolBar = toolBar;
    mMainWindow = mainWindow;
}
void  PluginManager::loadPlugins()
{
    QDir  pluginsDir(qApp->applicationDirPath());

//#if defined (Q_OS_MAC)

//    if (pluginsDir.dirName() == "MacOS")
//    {
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//        pluginsDir.cdUp();
//    }

//#endif
    pluginsDir.cd("..");
    pluginsDir.cd("plugins");

    foreach(const QString& fileName, pluginsDir.entryList(QDir::Files))
    {
        if ((fileName.split('.').back() == "so") || (fileName.split('.').back() == "dll"))
        {
            QString path = pluginsDir.absoluteFilePath(fileName);
            loadPlugin(path);
        }
    }
}
void  PluginManager::loadPlugin(QString path)
{
    // Try load plugin
    QPluginLoader  pluginLoader(path);
    QObject       *instance = pluginLoader.instance();

    if (instance)
    {
        // Try init plugin
        PluginInterface *plugin = qobject_cast<PluginInterface *>(instance);

        if (!plugin)
        {
            return;
        }
//        plugin->setParent(mMainWindow);
        plugin->setToolBar(mToolBar);
        plugin->setMap3dWidget(mMap3dWidget);
        plugin->setMainWindow(mMainWindow);
        plugin->setUpUI();
    }
    else
    {
        qWarning() << "Plugin loading failed: [" << path
                   << "] " << pluginLoader.errorString();
    }
}
