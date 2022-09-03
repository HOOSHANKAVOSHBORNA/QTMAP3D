#include "plugininterface.h"
#include "pluginmanager.h"

#include <QPluginLoader>
#include <QApplication>
#include <QString>
#include <QDebug>
#include <QDir>


PluginManager::PluginManager(Map3dWidget* map3dWidget, QDockWidget* docWidget)
{
    this->map3dWidget = map3dWidget;
    this->docWidget = docWidget;
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
        plugin->setDocWidget(docWidget);
        plugin->setMap3dWidget(map3dWidget);
        plugin->setUpUI();
    }
    else
    {
        qWarning() << "Plugin loading failed: [" << path
                   << "] " << pluginLoader.errorString();
    }
}
