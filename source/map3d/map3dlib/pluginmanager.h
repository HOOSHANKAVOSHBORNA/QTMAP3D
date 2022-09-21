#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QString>

class Map3dWidget;
class ToolBarWidget;

class PluginManager
{
public:
    PluginManager(Map3dWidget* map3dWidget, ToolBarWidget *toolBar);
    void  loadPlugins();
private:
    void loadPlugin(QString path);
private:
    Map3dWidget* mMap3dWidget;
    ToolBarWidget* mToolBar;
};

#endif // PLUGINMANAGER_H
