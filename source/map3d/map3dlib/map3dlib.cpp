#include "map3dlib.h"
#include "pluginmanager.h"
#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>

Map3dlib::Map3dlib(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Map3D Demo");
    QIcon ic(":/res/map3d.ico");
    setWindowIcon(ic);

    Map3dWidget* map3dWidget = new Map3dWidget(true, this);
    QDockWidget *docWidget = new QDockWidget(this);

    QToolBar* toolBar = new QToolBar();
    toolBar->setOrientation(Qt::Orientation::Vertical);
    docWidget->setWidget(toolBar);

    //load plugins
    PluginManager pluginmanager(map3dWidget, toolBar);
    pluginmanager.loadPlugins();

    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, docWidget);
    setCentralWidget(map3dWidget);
}

