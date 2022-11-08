#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "map3dlib.h"
#include <QString>
#include <QWidget>

class Map3dWidget;
class ToolBarWidget;

class PluginManager
{
public:
    PluginManager(Map3dlib *mainWindow, Map3dWidget* map3dWidget, ToolBarWidget *toolBar);
    void  loadPlugins();
private:
    void loadPlugin(QString path, QString name);
private:
    Map3dWidget* mMap3dWidget;
    ToolBarWidget* mToolBar;
    Map3dlib* mMainWindow;
};

#endif // PLUGINMANAGER_H
