#include "map3dlib.h"
#include "toolbarwidget.h"

#include "pluginmanager.h"
#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QPushButton>
#include <QToolBar>
#include <QVBoxLayout>
#include <QDebug>

Map3dlib::Map3dlib(QWidget *parent)
    : QMainWindow(parent)
{
    //--main window setting----------------------------------------------------------------
    setWindowTitle("Map3D Demo");
    QIcon ic(":/res/map3d.ico");
    setWindowIcon(ic);
    setStyleSheet("background-color:#282a31;");
    setStyleSheet("background-color:#282a31; color : white;");

    //--Map3dWidget setting----------------------------------------------------------------
    Map3dWidget* map3dWidget = new Map3dWidget(true, this);
    setCentralWidget(map3dWidget);

    //--ToolBarWidget setting--------------------------------------------------------------
    mToolBarWidget = new ToolBarWidget(this);
    connect(mToolBarWidget, &ToolBarWidget::onPin, this, &Map3dlib::onToolBarWidgetPin);

    //--ToolBarDocWidget setting-----------------------------------------------------------
    mToolBarDocWidget = new QDockWidget("Toolboxes",this);
    mToolBarDocWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    mToolBarDocWidget->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    mToolBarDocWidget->setMaximumWidth(200);
    mToolBarDocWidget->setMinimumWidth(200);
    //mToolBarDocWidget->setMinimumHeight(250);
    addDockWidget(Qt::LeftDockWidgetArea, mToolBarDocWidget);
    mToolBarDocWidget->hide();

    //--load plugins-----------------------------------------------------------------------
    PluginManager pluginmanager(map3dWidget, mToolBarWidget);
    pluginmanager.loadPlugins();

}

void Map3dlib::mousePressEvent(QMouseEvent *event)
{
    mToolBarWidget->setClose();
    mToolBarWidget->setSizeWidget(false);
    QMainWindow::mousePressEvent(event);
}

void Map3dlib::onToolBarWidgetPin(bool isPin)
{
    if(isPin){
        mToolBarDocWidget->show();
        mToolBarDocWidget->setWidget(mToolBarWidget);

    }else{
        mToolBarDocWidget->hide();
        mToolBarWidget->setParent(this);
        mToolBarWidget->move(0,0);
        mToolBarWidget->show();
    }
}

