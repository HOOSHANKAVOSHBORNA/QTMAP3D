#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QString>

class Map3dWidget;
class QDockWidget;

class PluginManager
{
public:
    PluginManager(Map3dWidget* map3dWidget, QDockWidget* docWidget);
    void  loadPlugins();
private:
    void loadPlugin(QString path);
private:
    Map3dWidget* map3dWidget;
    QDockWidget* docWidget;
};

#endif // PLUGINMANAGER_H
