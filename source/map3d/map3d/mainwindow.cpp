#include <QJSEngine>
#include <QJsonArray>
#include <QJsonObject>
#include <QOpenGLFunctions_2_0>
#include <QQmlEngine>
#include <QQuickOpenGLUtils>
#include <QTimer>
#include <QWindow>
#include <chrono>
#include <iostream>
#include <osgEarth/ImageLayer>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarthUtil/EarthManipulator>
#include <osgGA/GUIEventAdapter>

#include "bookmark.h"
#include "layerManager.h"
#include "listWindow.h"
#include "locationManager.h"
#include "mainwindow.h"
#include "mapControllerItem.h"
#include "mapItem.h"
#include "qmlNode.h"
#include "smallMap.h"

MainWindow::MainWindow(QWindow *parent)
{
    qmlRegisterType<SmallMap>("Crystal", 1, 0, "SmallMap");
    qmlRegisterType<QmlNode>("Crystal", 1, 0, "QmlNode");

    mMapItem = new MapControllerItem;
    mLocationManager = new LocationManager(mMapItem);
    mToolboxManager = new ToolboxManager;
    mLayerManager = new LayerManager(mMapItem);
    mBookmarkManager = new BookmarkManager;
}

MainWindow::~MainWindow()
{
    //    cleanup();
    //    mMapItem->deleteLater();
}

QQmlEngine *MainWindow::getQmlEngine()
{
    return qmlEngine(this);
}

MapControllerItem *MainWindow::getMapItem()
{
    return mMapItem;
}

ToolboxManager *MainWindow::getToolboxManager() const
{
    return mToolboxManager;
}

LayerManager *MainWindow::getLayerManager() const
{
    return mLayerManager;
}

BookmarkManager *MainWindow::getBookmarkManager() const
{
    return mBookmarkManager;
}

LocationProxyModel *MainWindow::getLocationManager() const
{
    return mLocationManager->locationProxyModel();
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

void MainWindow::addTabToListWindow(const QString tabTitle, QQuickItem *tabItem)
{
    if (mListWindow) {
        mListWindow->appendItem(tabTitle, tabItem);
    } else {
        qInfo() << "info - fail: "
                << "mListWindow is nullptr";
    }
}

void MainWindow::setListWindow(ListWindow *listWindow)
{
//    mListWindow = listWindow;
    //    mUIHandle->setListWindow(listWindow);
}
