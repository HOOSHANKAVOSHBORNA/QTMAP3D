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
    addMenuWidget(mToolBarWidget);
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
    PluginManager pluginmanager(this, map3dWidget, mToolBarWidget);
    pluginmanager.loadPlugins();
}

void Map3dlib::addMenuWidget(QWidget *widget)
{
    widget->setParent(this);
    mWidgets.append(widget);
//    reorderMenuWidgets();
    widget->show();
    resize(size().width()+1, size().height()+1);
}

void Map3dlib::removeMenuWidget(QWidget *widget)
{
    widget->setParent(nullptr);
    mWidgets.removeOne(widget);
//    if(mWidgets.removeOne(widget))
//        reorderMenuWidgets();
    resize(size().width()+1, size().height()+1);
}

void Map3dlib::mousePressEvent(QMouseEvent *event)
{
    mToolBarWidget->setClose();

    QMainWindow::mousePressEvent(event);
    emit onPressEvent();

}

void Map3dlib::resizeEvent(QResizeEvent *event)
{
    reorderMenuWidgets();
    QMainWindow::resizeEvent(event);
}

void Map3dlib::onToolBarWidgetPin(bool isPin)
{
    if(isPin){
        removeMenuWidget(mToolBarWidget);
        mToolBarDocWidget->show();
        mToolBarDocWidget->setWidget(mToolBarWidget);

    }else{
        mToolBarDocWidget->hide();
//        mToolBarWidget->move(0,0);
        addMenuWidget(mToolBarWidget);
//        mToolBarWidget->show();
    }
}

void Map3dlib::reorderMenuWidgets()
{
//    int x;
//    for(auto dock: this->findChildren(QDockWidget))
//        dockWidgetArea(dock);

    int w = 0;
    for(auto widget: mWidgets)
    {
        if(widget->parent() == this)
        {
            int x = centralWidget()->geometry().x();
            widget->move(x + w + 1, 0);
            w += widget->width();
        }
    }
}

