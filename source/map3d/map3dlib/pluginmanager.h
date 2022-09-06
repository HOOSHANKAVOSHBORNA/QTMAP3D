#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QString>

class Map3dWidget;
class QToolBar;

class PluginManager
{
public:
    PluginManager(Map3dWidget* map3dWidget, QToolBar *toolBar);
    void  loadPlugins();
private:
    void loadPlugin(QString path);
private:
    Map3dWidget* mMap3dWidget;
    QToolBar* mToolBar;
};

#endif // PLUGINMANAGER_H
