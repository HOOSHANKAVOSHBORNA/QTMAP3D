
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgGA/GUIEventAdapter>
#include <iostream>

#include "mainwindow.h"
#include "pluginmanager.h"

MainWindow::MainWindow(QWindow *parent) :
    OsgQuickWindow(parent)
{

    QObject::connect(this, &MainWindow::homeButtonClicked,
                     mMapController, &MapController::goToHome);
    QObject::connect(this, &MainWindow::projectionButtonClicked,
                     mMapController, &MapController::toggleProjection);


    QObject::connect(this, &MainWindow::upButtonClicked,
                     mMapController, &MapController::panUp);
    QObject::connect(this, &MainWindow::downButtonClicked,
                     mMapController, &MapController::panDown);
    QObject::connect(this, &MainWindow::leftButtonClicked,
                     mMapController, &MapController::panLeft);
    QObject::connect(this, &MainWindow::rightButtonClicked,
                     mMapController, &MapController::panRight);


    QObject::connect(this, &MainWindow::rotateUpButtonClicked,
                     mMapController, &MapController::rotateUp);
    QObject::connect(this, &MainWindow::rotateDownButtonClicked,
                     mMapController, &MapController::rotateDown);
    QObject::connect(this, &MainWindow::rotateLeftButtonClicked,
                     mMapController, &MapController::rotateLeft);
    QObject::connect(this, &MainWindow::rotateRightButtonClicked,
                     mMapController, &MapController::rotateRight);


    QObject::connect(this, &MainWindow::zoomInButtonClicked,
                     mMapController, &MapController::zoomIn);
    QObject::connect(this, &MainWindow::zoomOutButtonClicked,
                     mMapController, &MapController::zoomOut);

    QObject::connect(mMapController, &MapController::headingAngleChanged,
                     this, &MainWindow::setHeadingAngle);
}

MainWindow::~MainWindow()
{

}

qreal MainWindow::headingAngle() const
{
    return mheadingAngle;
}

void MainWindow::initializePluginsUI(std::list<CrystalPluginInfo> pluginsInfoList)
{
    for (auto& item : pluginsInfoList) {

        if (item.qmlDesc->pluginHasSideItem) {
            QVariant ret;
            QMetaObject::invokeMethod(this,
                                      "addSideItem",
                                      Qt::DirectConnection,
                                      Q_RETURN_ARG(QVariant, ret),
                                      Q_ARG(QVariant, QVariant::fromValue<QString>(item.qmlDesc->sideItemMenuBarTitle)),
                                      Q_ARG(QVariant, QVariant::fromValue<QString>(item.qmlDesc->sideItemMenuBarIconUrl)),
                                      Q_ARG(QVariant, QVariant::fromValue<QString>(item.qmlDesc->sideItemUrl))
                                      );

            bool bOk = false;
            const int idx = ret.toInt(&bOk);
            if (bOk) {
                item.sideItemIndex = idx;
            }
        }

//        for (auto toolboxItemDesc : item.qmlDesc->toolboxItemsList) {


//            QVariant ret;
//            QMetaObject::invokeMethod(this,
//                                      "addToolboxItem",
//                                      Qt::DirectConnection,
//                                      Q_RETURN_ARG(QVariant, ret),
//                                      Q_ARG(QVariant, QVariant::fromValue<ToolboxItemDesc*>(toolboxItemDesc))
//                                      );
//             bool bOk = false;
//            const int idx = ret.toInt(&bOk);
        //}
    }
}

void MainWindow::setHeadingAngle(qreal angle)
{
    if (mheadingAngle != angle) {
        mheadingAngle = angle;
        emit headingAngleChanged(angle);
    }
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    OsgQuickWindow::keyPressEvent(event);
}
