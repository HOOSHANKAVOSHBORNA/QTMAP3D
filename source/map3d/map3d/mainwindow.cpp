
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
#include "listWindow.h"
#include "qqmlcontext.h"
#include "mapControllerItem.h"
#include "layerManager.h"
#include "locationManager.h"
#include "settings.h"
#include "qmlNode.h"
#include "filterManager.h"
#include "userManager.h"
#include <QJsonArray>
#include <QJsonObject>
#include <QQuickOpenGLUtils>
#include "smallMap.h"

MainWindow::MainWindow(QWindow *parent) :
    QQuickWindow(parent)
{
    qmlRegisterType<MapControllerItem>("Crystal",1,0,"MapController");
    qmlRegisterType<SmallMap>("Crystal", 1, 0, "SmallMap");
  //  qmlRegisterType<UserManager>("Crystal",1, 0,"UserInfo");
    qmlRegisterSingletonType<ProfileManager>("Crystal", 1, 0, "ProfileManagerInstance", ProfileManager::createSingletonInstance);
    qmlRegisterSingletonType<ToolboxManager>("Crystal", 1, 0, "ToolboxManagerInstance", ToolboxManager::createSingletonInstance);
    qmlRegisterSingletonType<LayerManager>("Crystal", 1, 0, "LayerManagerInstance", LayerManager::createSingletonInstance);
    qmlRegisterSingletonType<LocationManager>("Crystal", 1, 0, "LocatoinManagerInstance", LocationManager::createSingletonInstance);
    qmlRegisterSingletonType<BookmarkManager>("Crystal", 1, 0, "BookmarkInstance", BookmarkManager::createSingletonInstance);
    qmlRegisterSingletonType<SettingsManager>("Crystal", 1, 0, "SettingsInstance", SettingsManager::createSingletonInstance);

    qmlRegisterType<QmlNode>("Crystal", 1, 0, "QmlNode");
    setColor(Qt::black);


}


MainWindow::~MainWindow()
{
    //    cleanup();
    //    mMapItem->deleteLater();
}

void MainWindow::initComponent()
{
    QQmlEngine *engine = qmlEngine(this);

    QQmlComponent* comp = new QQmlComponent(engine);
    connect(comp, &QQmlComponent::statusChanged, [&](QQmlComponent::Status status){
        if(status == QQmlComponent::Error){
            qDebug()<<"Can not load MapControllerItem: "<<comp->errorString();
        }

        if(status == QQmlComponent::Ready){
            QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());
            mMapItem = static_cast<MapControllerItem*>(item);
//            //            mMapItem->initializeOsgEarth();
            mMapItem->setQmlEngine(engine);
            addToCenterCenterContainer(mMapItem);

            // --------------------------------------------------------- model settings
            LocationManager* locationManager = LocationManager::createSingletonInstance(nullptr, nullptr);
            locationManager->initialize(mMapItem);

            // TEST
//            QQmlComponent* comp = new QQmlComponent(engine);

//            QObject::connect(comp, &QQmlComponent::statusChanged, [&](QQmlComponent::Status status) {
//                if(status == QQmlComponent::Error) {
//                    qDebug() << "Can not load this: " << comp->errorString();
//                }

//                if(status == QQmlComponent::Ready) {
//                    QQuickItem *item = qobject_cast<QQuickItem*>(comp->create());

//                    ToolboxManager::createSingletonInstance(nullptr, nullptr)->addPropertyItem(item, QStringLiteral("Test Item"));
//                }
//            });

            //            comp->loadUrl(QUrl("qrc:/TestItem.qml"));
            // ENDTEST

            ToolboxManager *toolboxManager = ToolboxManager::createSingletonInstance(nullptr, nullptr);

            LayerManager *layerManager = LayerManager::createSingletonInstance(nullptr, nullptr);
            layerManager->setMapItem(mMapItem);
        }
    });
    comp->loadUrl(QUrl("qrc:/MapControllerItem.qml"));
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
    return ToolboxManager::createSingletonInstance(nullptr, nullptr);
}

LayerManager *MainWindow::getLayerManager() const
{
    return LayerManager::createSingletonInstance(nullptr, nullptr);
}

BookmarkManager *MainWindow::getBookmarkManager() const
{
    return BookmarkManager::createSingletonInstance(nullptr, nullptr);
}

LocationProxyModel *MainWindow::getLocationManager() const
{
    return LocationManager::createSingletonInstance(nullptr, nullptr)->locationProxyModel();
}

void MainWindow::addToMenuItemContainer(QQuickItem *item, QString title)
{
    QMetaObject::invokeMethod(this, "addToMenuItemContainer",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item))
                              );
}

void MainWindow::removeFromMenuItemContainer(QQuickItem *item)
{
    QMetaObject::invokeMethod(this, "removeFromMenuItemContainer",
                              Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(item))
                              );
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
//        QMetaObject::invokeMethod(mListWindow,
//                                  "addTab",
//                                  Q_ARG(QVariant, QVariant::fromValue<QString>(tabTitle)),
//                                  Q_ARG(QVariant, QVariant::fromValue<QQuickItem*>(tabItem))
//                                  );

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
