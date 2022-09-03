#include "map3dlib.h"
#include "pluginmanager.h"
#include <QDockWidget>
#include <QFileDialog>
#include <QPushButton>

Map3dlib::Map3dlib(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Map3D Demo");
    QIcon ic(":/res/map3d.ico");
    setWindowIcon(ic);

    Map3dWidget* map3dWidget = new Map3dWidget(true, this);
    QDockWidget *docWidget = new QDockWidget(this);

    //load plugins
    PluginManager pluginmanager(map3dWidget, docWidget);
    pluginmanager.loadPlugins();

    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, docWidget);
    setCentralWidget(map3dWidget);
}

