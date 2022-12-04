
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

void MainWindow::initializePluginsUI(std::list<PluginInfo>& pluginsInfoList)
{

    const auto simple_add_toolbox_item = [this](
                    QString name      = QString(),
                    QString category  = QString(),
                    QString iconUrl   = QString(),
                    bool    checkable = false,
                    bool    hasMenu   = false,
                    QString menuUrl   = QString()) {
        QVariant ret;
        const ToolboxItemDesc desc(
                                name     ,
                                category ,
                                iconUrl  ,
                                checkable,
                                hasMenu  ,
                                menuUrl  );

        ToolboxItemDescProxy proxy(desc);
        QMetaObject::invokeMethod(this, "addToolboxItem", Qt::DirectConnection, Q_RETURN_ARG(QVariant, ret),
                                  Q_ARG(QVariant, QVariant::fromValue<ToolboxItemDescProxy*>(&proxy)));
    };

    //    simple_add_toolbox_item("Amir",   "Jafari","qrc:/Resources/extrudepoly.png" ,true );
    //    simple_add_toolbox_item("Bagher", "Roodsarab","qrc:/Resources/geocentric.png" ,false);
    //    simple_add_toolbox_item("Hasan",  "Roodsarabi","qrc:/Resources/geocentric.png" ,true);
    //    simple_add_toolbox_item("Hasan1",  "Roodsarabi","qrc:/Resources/geocentric.png" ,false);
    //    simple_add_toolbox_item("Hasan2",  "Roodsarabi","qrc:/Resources/geocentric.png" ,false);
    //    simple_add_toolbox_item("Hasa3",  "Roodsara","qrc:/Resources/circle.png" ,true);
    //    simple_add_toolbox_item("Hasa4",  "Roodsara","qrc:/Resources/ellipse.png" ,false);
    //    simple_add_toolbox_item("Hasa5",  "Roodsara","qrc:/Resources/image.png" ,false);
    //    simple_add_toolbox_item("Hasa6",  "Roodsara","qrc:/Resources/line.png" ,false);
    //    simple_add_toolbox_item("Hasa7", "Roodsara", "qrc:/Resources/polygon.png" ,true);
    //    simple_add_toolbox_item("Hasa8", "Roodsara", "qrc:/Resources/rectangle.png" ,true);
    //    simple_add_toolbox_item("Hasa9",  "Roodsara","qrc:/Resources/sphere.png" ,false);
    //    simple_add_toolbox_item("Hasa10",  "Roodsara","qrc:/Resources/extrudepoly.png" ,false);



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

        for (auto toolboxItemDesc : item.qmlDesc->toolboxItemsList) {
            QVariant ret;
            ToolboxItemDescProxy proxy(*toolboxItemDesc, item.interface);
            QMetaObject::invokeMethod(this,
                                      "addToolboxItem",
                                      Qt::DirectConnection,
                                      Q_RETURN_ARG(QVariant, ret),
                                      Q_ARG(QVariant, QVariant::fromValue<ToolboxItemDescProxy*>(&proxy))
                                      );
            bool bOk = false;
            const int idx = ret.toInt(&bOk);
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


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    OsgQuickWindow::keyPressEvent(event);
}
