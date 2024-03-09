#include <QJSEngine>
#include <QJsonArray>
#include <QJsonObject>
#include <QOpenGLFunctions_2_0>
#include <QQmlEngine>
#include <QQuickOpenGLUtils>
#include <QTimer>
#include <QWindow>
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

MainWindow::MainWindow(UserManager *userManager, QWindow *parent)
{
    qmlRegisterType<QmlNode>("Crystal", 1, 0, "QmlNode");

    mMapItem = new MapControllerItem();
    mLocationManager = new LocationManager(mMapItem, userManager, this);
    mToolboxManager = new ToolboxManager(this);
    mBookmarkManager = new BookmarkManager(this);
    mListWindow = new ListWindow(this);
    mLayerManager = new LayerManager(mMapItem, this);
}

MainWindow::~MainWindow()
{
    // qDebug() << mMapItem.
    delete mMapItem;
    qDebug() << "~MainWindow!";
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
    mListWindow = listWindow;
}


void MainWindow::clearData()
{

}

ListWindow *MainWindow::getListWindow() const
{
    return mListWindow;
}
