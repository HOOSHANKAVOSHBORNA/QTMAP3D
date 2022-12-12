
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgGA/GUIEventAdapter>
#include <iostream>

#include "mainwindow.h"
#include "pluginmanager.h"
#include "plugininterface.h"

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


    QObject::connect(mMapController, &MapController::mousePointedLocationChanged,
                     this, &MainWindow::setMousePointedLocation);
    QObject::connect(this, &MainWindow::goToLocation,
                     mMapController, &MapController::goToPosition);


    QObject::connect(mMapController, &MapController::focalPointLatChanged,
                     this, &MainWindow::setFocalPointLat);
    QObject::connect(mMapController, &MapController::focalPointLongChanged,
                     this, &MainWindow::setFocalPointLong);
    QObject::connect(mMapController, &MapController::focalPointRangeChanged,
                     this, &MainWindow::setFocalPointRange);
    QObject::connect(mMapController, &MapController::focalPointPitchChanged,
                     this, &MainWindow::setFocalPointPitch);
    QObject::connect(mMapController, &MapController::focalPointHeadChanged,
                     this, &MainWindow::setFocalPointHead);



    QObject::connect(mMapController, &MapController::fpsChanged,
                     this, &MainWindow::setFps);

    mInfoWidgetHandle = new InfoWidgetHandle(this);
}

MainWindow::~MainWindow()
{

}

qreal MainWindow::headingAngle() const
{
    return mheadingAngle;
}

QVector3D MainWindow::mousePointedLocation() const
{
    return mMousePointedLocation;
}

qreal MainWindow::focalPointLat() const
{
    return mFocalPointLat;
}

qreal MainWindow::focalPointLong() const
{
    return mFocalPointLong;
}

qreal MainWindow::focalPointRange() const
{
    return mFocalPointRange;

}

qreal MainWindow::focalPointPitch() const
{
    return mFocalPointPitch;
}

qreal MainWindow::focalPointHead() const
{
    return mFocalPointHead;
}

qreal MainWindow::fps() const
{
    return mFps;
}

InfoWidgetHandle *MainWindow::infoWidgetHandle() const
{
    return mInfoWidgetHandle;
}

void MainWindow::initializePluginsUI(std::list<PluginInfo>& pluginsInfoList)
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

        for (auto toolboxItemDesc : item.qmlDesc->toolboxItemsList)
        {
            QVariant ret;
            ItemDescProxy proxy(*toolboxItemDesc, item.interface);
            QMetaObject::invokeMethod(this,
                                      "addToolboxItem",
                                      Qt::DirectConnection,
                                      Q_RETURN_ARG(QVariant, ret),
                                      Q_ARG(QVariant, QVariant::fromValue<ItemDescProxy*>(&proxy))
                                      );
        }
        for (auto fileItem : item.qmlDesc->fileItemsList)
        {
            QVariant ret;
            ItemDescProxy proxy(*fileItem, item.interface);
            QMetaObject::invokeMethod(this,
                                      "addFileItem",
                                      Qt::DirectConnection,
                                      Q_RETURN_ARG(QVariant, ret),
                                      Q_ARG(QVariant, QVariant::fromValue<ItemDescProxy*>(&proxy))
                                      );
        }
    }
}

void MainWindow::setHeadingAngle(qreal angle)
{
    if (mheadingAngle != angle) {
        mheadingAngle = angle;
        emit headingAngleChanged(angle);
    }
}

void MainWindow::setMousePointedLocation(const QVector3D &pointedLoc)
{
    if (mMousePointedLocation != pointedLoc) {
        mMousePointedLocation = pointedLoc;
        emit mousePointedLocationChanged();
    }
}

void MainWindow::setFocalPointLat(qreal focalPointLat)
{
    if (mFocalPointLat != focalPointLat) {
        mFocalPointLat = focalPointLat;
        emit focalPointLatChanged();
    }
}

void MainWindow::setFocalPointLong(qreal focalPointLong)
{
    if (mFocalPointLong != focalPointLong) {
        mFocalPointLong = focalPointLong;
        emit focalPointLongChanged();
    }
}

void MainWindow::setFocalPointRange(qreal focalPointRange)
{
    if (mFocalPointRange != focalPointRange) {
        mFocalPointRange = focalPointRange;
        emit focalPointRangeChanged();
    }

}

void MainWindow::setFocalPointPitch(qreal focalPointPitch)
{
    if (mFocalPointPitch != focalPointPitch) {
        mFocalPointPitch = focalPointPitch;
        emit focalPointPitchChanged();
    }
}

void MainWindow::setFocalPointHead(qreal focalPointHead)
{
    if (mFocalPointHead != focalPointHead) {
        mFocalPointHead = focalPointHead;
        emit focalPointHeadChanged();
    }
}

void MainWindow::setFps(qreal fps)
{
    if (mFps != fps) {
        mFps = fps;
        emit fpsChanged();
    }
}

void MainWindow::travelToViewpoint(qreal latitude, qreal longitude, qreal range, qreal pitch, qreal heading)
{
    mMapController->travelToViewpoint(latitude,
                                      longitude,
                                      range,
                                      pitch,
                                      heading);
}

void MainWindow::showInfoWidget(MainWindow::InfoWidgetType infoWidgetType)
{

}

void MainWindow::updateInfoWidgetData(const QString &dataJSON)
{

}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    OsgQuickWindow::keyPressEvent(event);
}
