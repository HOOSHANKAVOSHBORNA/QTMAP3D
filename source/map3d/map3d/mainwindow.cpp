
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgGA/GUIEventAdapter>
#include <iostream>
#include <QTimer>
#include <QWindow>
#include <QOpenGLFunctions_2_0>
#include <chrono>

#include "mainwindow.h"
#include "plugininterface.h"
#include "mapItem.h"
#include "listwindow.h"

#include <QQuickOpenGLUtils>
MainWindow::MainWindow(QWindow *parent) :
    QQuickWindow(parent)
{

    setColor(Qt::black);
    mToolbox = new ToolboxProxyModel();
    Toolbox *toolbox = new Toolbox(this);
    mToolbox->setSourceModel(toolbox);

    mUIHandle = new UIHandle(this);
}


MainWindow::~MainWindow()
{
    //    cleanup();
    //    mMapItem->deleteLater();
}


LayersModel *MainWindow::layersModel() const
{
    return mLayersModel;
}

ToolboxProxyModel *MainWindow::toolbox() const
{
    return mToolbox;
}

UIHandle *MainWindow::uiHandle() const
{
    return mUIHandle;
}

void MainWindow::addToCenterCenterContainer(QQuickItem *item)
{
    QMetaObject::invokeMethod(this, "addToCenterCenterContainer",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item))
                              );
}

void MainWindow::showListWindow()
{
    if (mListWindow) {
        if (!mListWindow->isVisible())
            mListWindow->showNormal();
        else
            mListWindow->hide();
    }
}

void MainWindow::setLayersModel(LayersModel *layersModel)
{
    if (mLayersModel != layersModel) {
        mLayersModel = layersModel;
        emit layersModelChanged();
    }
}

void MainWindow::setToolbox(ToolboxProxyModel *toolbox)
{
    mToolbox = toolbox;
    emit toolboxChanged();
}

MapItem *MainWindow::getMapItem()
{
    return mMapItem;
}

void MainWindow::setMapItem(MapItem &mapItem)
{
    mMapItem = &mapItem;
    mLayersModel = new LayersModel(mMapItem);
}

void MainWindow::addToLeftContainer(QQuickItem *item, QString title)
{
    QMetaObject::invokeMethod(this, "addToLeftContainer",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item)),
                              Q_ARG(QVariant, QVariant::fromValue<QString>(title))
                              );
}

void MainWindow::addToRightContainer(QQuickItem *item, QString title)
{
    QMetaObject::invokeMethod(this, "addToRightContainer",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item)),
                              Q_ARG(QVariant, QVariant::fromValue<QString>(title))
                              );
}

void MainWindow::setListWindow(ListWindow *listWindow)
{
    mListWindow = listWindow;
    mUIHandle->setListWindow(listWindow);
}

bool MainWindow::event(QEvent *ev)
{
    switch (ev->type()) {
    case QEvent::Close:
        if (mListWindow) {
            mListWindow->close();
        }
        break;

    default: break;
    }

    return QQuickWindow::event(ev);
}
