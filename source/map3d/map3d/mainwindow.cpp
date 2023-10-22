
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgGA/GUIEventAdapter>
#include <iostream>
#include <QTimer>
#include <QWindow>
#include <QOpenGLFunctions_2_0>
#include <chrono>
#include <bookmark.h>
#include <QQmlEngine>
#include <QJSEngine>

#include "mainwindow.h"
#include "mapItem.h"
#include "listwindow.h"
#include "qqmlcontext.h"
#include "mapControllerItem.h"

#include "locationManagerModel.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QQuickOpenGLUtils>
#include "smallmap.h"

MainWindow::MainWindow(QWindow *parent) :
    QQuickWindow(parent)
{
    qmlRegisterType<LayersModel>("Crystal", 1, 0, "CLayersModel");
    qmlRegisterType<MapControllerItem>("Crystal",1,0,"MapController");
    qmlRegisterType<SmallMap>("Crystal", 1, 0, "SmallMap");
    qmlRegisterType<Toolbox>("Crystal",1,0,"Toolbox");

    qmlRegisterSingletonType<ToolboxProxyModel>("Crystal", 1, 0, "ToolboxInstance", ToolboxProxyModel::createSingletonInstance);
    qmlRegisterSingletonType<LayersModel>("Crystal", 1, 0, "LayersInstance", LayersModel::createSingletonInstance);
    qmlRegisterSingletonType<LocationManagerProxyModel>("Crystal", 1, 0, "LocatoinManagerInstance", LocationManagerProxyModel::createSingletonInstance);
    qmlRegisterSingletonType<BookmarkProxyModel>("Crystal", 1, 0, "BookmarkInstance", BookmarkProxyModel::createSingletonInstance);

    setColor(Qt::black);
}


MainWindow::~MainWindow()
{
    //    cleanup();
    //    mMapItem->deleteLater();
}

void MainWindow::initComponent()
{
//    QQmlEngine *engine = qmlContext(this)->engine();
    QQmlEngine *engine = qmlEngine(this);

    QQmlComponent* comp = new QQmlComponent(engine);
    connect(comp, &QQmlComponent::statusChanged,[&](QQmlComponent::Status status){
        if(status == QQmlComponent::Error){
            qDebug()<<"Can not load MapControllerItem: "<<comp->errorString();
        }

        if(status == QQmlComponent::Ready){
            QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
            mMapItem = static_cast<MapControllerItem*>(item);
            mMapItem->setQmlEngine(engine);
            addToCenterCenterContainer(mMapItem);

            // ----------------------------------
            // --------- model settings ---------
            // ----------------------------------
            LocationManagerModel *locationManagerModel = new LocationManagerModel(mMapItem);
            LocationManagerProxyModel* myModel = LocationManagerProxyModel::createSingletonInstance(nullptr, nullptr);
            myModel->setSourceModel(locationManagerModel);


            ToolboxProxyModel* toolboxProxyModel = ToolboxProxyModel::createSingletonInstance(nullptr, nullptr);
            Toolbox *toolbox = new Toolbox(this);
            toolboxProxyModel->setSourceModel(toolbox);

            LayersModel *layersModel = LayersModel::createSingletonInstance(nullptr, nullptr);
            layersModel->initialize(mMapItem);



            mBookmark = BookmarkProxyModel::createSingletonInstance(nullptr, nullptr);
        }
    });
    comp->loadUrl(QUrl("qrc:/MapControllerItem.qml"));

//    engine->rootContext()->setContextProperty("bookmarkproxymodel", mBookmark);
}

QQmlEngine *MainWindow::getQmlEngine()
{
    return qmlEngine(this);
}


LayersModel *MainWindow::layersModel() const
{
    return mLayersModel;
}

void MainWindow::addToCenterCenterContainer(QQuickItem *item)
{
    QMetaObject::invokeMethod(this, "addToCenterCenterContainer",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item))
                              );
}

void MainWindow::removeFromRightContainer(QQuickItem *item)
{
    QMetaObject::invokeMethod(this, "removeFromRightContainer",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item))
                              );
}

void MainWindow::removeFromLeftContainer(QQuickItem *item)
{
    QMetaObject::invokeMethod(this, "removeFromLeftContainer",
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

MapItem *MainWindow::getMapItem()
{
    return mMapItem;
}

void MainWindow::showInfoItem(QQuickItem *item, QString title)
{
    QMetaObject::invokeMethod(this,
                              "showInfoView",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item)),
                              Q_ARG(QVariant, QVariant::fromValue<QString>(title))
                              );
}

void MainWindow::hideInfoItem(QQuickItem *item)
{
    removeFromLeftContainer(item);
}

void MainWindow::addTabToListWindow(const QString tabTitle, QQuickItem *tabItem)
{
    if (mListWindow) {
        QMetaObject::invokeMethod(mListWindow,
                                  "addTab",
                                  Q_ARG(QVariant, QVariant::fromValue<QString>(tabTitle)),
                                  Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(tabItem))
                                  );

    }
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
//    mUIHandle->setListWindow(listWindow);
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

BookmarkProxyModel *MainWindow::bookmark() const
{
    return mBookmark;
}
