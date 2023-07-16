
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
    Toolbox *toolbox = new Toolbox();
    mToolbox->setSourceModel(toolbox);

    mUIHandle = new UIHandle(this);
}


MainWindow::~MainWindow()
{
//    cleanup();
//    mMapItem->deleteLater();
}


QQuickItem *MainWindow::wrapItemWithDockable(QQuickItem *item, const QString& title)
{
    QVariant dockableItemVariant;
    QMetaObject::invokeMethod(this, "wrapItemWithDockableImpl",
                              Q_RETURN_ARG(QVariant, dockableItemVariant),
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item)),
                              Q_ARG(QVariant, QVariant::fromValue<QString>(title)));

    QQuickItem *dockableItem = dockableItemVariant.value<QQuickItem*>();

    return dockableItem;
}

void MainWindow::setCentralDockItem(QQuickItem *dockItem)
{
    QMetaObject::invokeMethod(this, "setCentralDockItemImpl",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(dockItem)));
}

void MainWindow::attachToCentralDockItem(QQuickItem *dockItem, bool horizontalAttach, bool attachAsFirst, qreal splitScale)
{
    QMetaObject::invokeMethod(this, "attachToCentralDockItemImpl",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(dockItem)),
                              Q_ARG(QVariant, QVariant::fromValue<bool>(horizontalAttach)),
                              Q_ARG(QVariant, QVariant::fromValue<bool>(attachAsFirst)),
                              Q_ARG(QVariant, QVariant::fromValue<qreal>(splitScale))
                              );
}

void MainWindow::showInRightDock(QQuickItem *item)
{
    QQuickItem* dockItem = wrapItemWithDockable(item, "Property");
    attachToCentralDockItem(dockItem, true, false, 0.2);
}

void MainWindow::addDockItem(QQuickItem *item, int position)
{

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
