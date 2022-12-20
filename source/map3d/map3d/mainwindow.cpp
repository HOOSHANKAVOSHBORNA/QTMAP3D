
#include <osgEarthUtil/EarthManipulator>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ImageLayer>
#include <osgGA/GUIEventAdapter>
#include <iostream>
#include <QTimer>
#include <QWindow>
#include <QOpenGLFunctions_2_0>

#include "mainwindow.h"
#include "pluginmanager.h"
#include "plugininterface.h"
#include "mapcontroller.h"
#include "listwindow.h"
#include "application.h"

MainWindow::MainWindow(QWindow *parent) :
        QQuickWindow(parent),
        mMapController(new MapController(this))
{
    mOGLF = new QOpenGLFunctions_2_0;


    QObject::connect(this, &MainWindow::sceneGraphInitialized,
                     this, &MainWindow::initializeGL,
                     Qt::DirectConnection);
    QObject::connect(this, &MainWindow::sceneGraphInvalidated,
                     this, &MainWindow::cleanup,
                     Qt::DirectConnection);

    setClearBeforeRendering(false);
    setColor(Qt::black);






    QObject::connect(this, &MainWindow::homeButtonClicked,
                     mMapController, &MapController::goToHome);
    QObject::connect(this, &MainWindow::view3DButtonClicked,
                     mMapController, &MapController::toggle3DView);


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


    QObject::connect(mMapController, &MapController::mousePointingLocationWgs84Changed,
                     this, &MainWindow::setMousePointingLocationWgs84);
    QObject::connect(mMapController, &MapController::mousePointingLocationChanged,
                     this, &MainWindow::setMousePointingLocation);
    QObject::connect(this, &MainWindow::goToLocation,
                     mMapController, QOverload<double, double, double>::of(&MapController::goToPosition));


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

    mUIHandle = new UIHandle(this);

    QObject::connect(this, &MainWindow::infoWidget2D3DButtonClicked, [this](){
        mUIHandle->onInfoWidget2D3DButtonClicked();
    });
    QObject::connect(this, &MainWindow::infoWidgetRouteButtonClicked, [this](){
        mUIHandle->onInfoWidgetRouteButtonClicked();
    });
    QObject::connect(this, &MainWindow::infoWidgetFollowButtonClicked, [this](){
        mUIHandle->onInfoWidgetFollowButtonClicked();
    });
    QObject::connect(this, &MainWindow::infoWidgetMoreButtonClicked, [this](){
        mUIHandle->onInfoWidgetMoreButtonClicked();
    });



}


MainWindow::~MainWindow()
{
    cleanup();
    mMapController->deleteLater();
}

qreal MainWindow::headingAngle() const
{
    return mheadingAngle;
}

QVector3D MainWindow::mousePointingLocationWgs84() const
{
    return mMousePointingLocationWgs84;
}

QVector3D MainWindow::mousePointingLocation() const
{
    return mMousePointingLocation;
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

UIHandle *MainWindow::uiHandle() const
{
    return mUIHandle;
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

void MainWindow::setMousePointingLocationWgs84(const QVector3D &pointedLoc)
{
    if (mMousePointingLocationWgs84 != pointedLoc) {
        mMousePointingLocationWgs84 = pointedLoc;
        emit mousePointingLocationWgs84Changed();
    }
}
void MainWindow::setMousePointingLocation(const QVector3D &pointedLoc)
{
    if (mMousePointingLocation != pointedLoc) {
        mMousePointingLocation = pointedLoc;
        emit mousePointingLocationChanged();
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



void MainWindow::setStatusBarText(const QString message, int time) {
    QVariant ret;
    QMetaObject::invokeMethod(this,
                              "showStatusMessage",
                              Qt::DirectConnection,
                              Q_ARG(QVariant, QVariant::fromValue<QString>(message)),
                              Q_ARG(QVariant, QVariant::fromValue<int>(time))
                              );
}







MapController *MainWindow::mapController() const
{
    return mMapController;
}


void MainWindow::cleanup()
{
    mMapController->cleanup();
}

void MainWindow::frame()
{
    if (mResized) {
        resizeGL();
        mResized = false;
    }

    paintGL();

    const auto pluginManager = Application::instance()->pluginManager();
    if (pluginManager) {
        pluginManager->frameEvent();
    }

}

void MainWindow::restoreContext()
{
    if (mContext && mSurface) {
        mContext->makeCurrent(mSurface);
    }
}

void MainWindow::setListWindow(ListWindow *listWindow)
{
    mListWindow = listWindow;
    mUIHandle->setListWindow(listWindow);
}


void MainWindow::initializeGL()
{
    mOGLF->initializeOpenGLFunctions();

    mContext = QOpenGLContext::currentContext();
    mSurface = mContext->surface();


    mMapController->initializeGL(width(), height(), screen(), renderTargetId());

    restoreContext();
    mMapController->initializeOsgEarth();
    restoreContext();
    emit osgInitialized();
    restoreContext();

    resetOpenGLState();

    QObject::connect(this, &MainWindow::beforeRendering,
                     this, &MainWindow::frame,
                     Qt::DirectConnection);
}

void MainWindow::resizeGL()
{
    mMapController->resizeGL(mViewportWidth, mViewportHeight, screen());
}

void MainWindow::paintGL()
{
    resetOpenGLState();

    mMapController->paintGL();

    mOGLF->glClear(GL_DEPTH_BUFFER_BIT);

    resetOpenGLState();
}

void MainWindow::resizeEvent(QResizeEvent *ev)
{
    QQuickWindow::resizeEvent(ev);

    const QSize s = ev->size();
    mResized       = true;
    mViewportWidth  = s.width();
    mViewportHeight = s.height();

}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    QQuickWindow::keyPressEvent(event);

    if (event->isAccepted())
        return;


    switch (event->key()) {
    case Qt::Key_Up:
    {
        auto manip = mMapController->getEarthManipulator();
        if (manip) {
            manip->pan(0, -0.03);
        }
        event->accept();
    }
        break;
    case Qt::Key_Down:
    {
        auto manip = mMapController->getEarthManipulator();
        if (manip) {
            manip->pan(0, 0.03);
        }
        event->accept();
    }
        break;
    case Qt::Key_Left:
    {
        auto manip = mMapController->getEarthManipulator();
        if (manip) {
            manip->pan(0.03, 0);
        }
        event->accept();
    }
        break;
    case Qt::Key_Right:
    {
        auto manip = mMapController->getEarthManipulator();
        if (manip) {
            manip->pan(-0.03, 0);
        }
        event->accept();
    }
        break;
    }


    if (event->isAccepted())
        return;

    const auto pluginManager = Application::instance()->pluginManager();
    if (pluginManager) {
        pluginManager->keyPressEvent(event);
    }

    if (event->isAccepted())
        return;

    mMapController->keyPressEvent(event);


}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QQuickWindow::keyReleaseEvent(event);

    if (event->isAccepted())
        return;

    switch (event->key()) {
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Left:
    case Qt::Key_Right:
        event->accept();
        break;
    }

    if (event->isAccepted())
        return;

    const auto pluginManager = Application::instance()->pluginManager();
    if (pluginManager) {
        pluginManager->keyReleaseEvent(event);
    }
    if (event->isAccepted())
        return;

    mMapController->keyReleaseEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QQuickWindow::mousePressEvent(event);

    if (event->isAccepted())
        return;


    const auto pluginManager = Application::instance()->pluginManager();
    if (pluginManager) {
        pluginManager->mousePressEvent(event);
    }
    if (event->isAccepted())
        return;


    mMapController->mousePressEvent(event);


    if (event->button() == Qt::LeftButton) {
        mLastMousePressTime = QTime::currentTime();
        mLastPressPoint = event->pos();

        if (!mInClickProcess) {
            mMousePressOusideClickProcess = true;
        } else {
            mMousePressOusideClickProcess = false;
        }

    }


}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QQuickWindow::mouseReleaseEvent(event);

    if (event->isAccepted())
        return;

    const auto pluginManager = Application::instance()->pluginManager();
    if (pluginManager) {
        pluginManager->mouseReleaseEvent(event);
    }
    if (event->isAccepted())
        return;


    mMapController->mouseReleaseEvent(event);


    if (event->button() == Qt::LeftButton) {
        if (mLastMousePressTime.msecsTo(QTime::currentTime()) < 400) {
            const QPoint diff = event->pos() - mLastPressPoint;
            if (std::abs(diff.x()) < 10 && std::abs(diff.y()) < 10) {

                if (!mInClickProcess && mMousePressOusideClickProcess) {
                    mInClickProcess = true;
                    QTimer::singleShot(300, [this](){
                        if (mMousePressOusideClickProcess)
                            emit clicked();
                        this->mInClickProcess = false;
                    });

                }
            }
        }
    }

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{

    QQuickWindow::mouseDoubleClickEvent(event);

    if (event->isAccepted())
        return;

    const auto pluginManager = Application::instance()->pluginManager();
    if (pluginManager) {
        pluginManager->mouseDoubleClickEvent(event);
    }
    if (event->isAccepted())
        return;


    mMapController->mouseDoubleClickEvent(event);

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QQuickWindow::mouseMoveEvent(event);

    if (event->isAccepted())
        return;

    const auto pluginManager = Application::instance()->pluginManager();
    if (pluginManager) {
        pluginManager->mouseMoveEvent(event);
    }
    if (event->isAccepted())
        return;


    mMapController->mouseMoveEvent(event);

}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    QQuickWindow::wheelEvent(event);

    if (event->isAccepted())
        return;

    const auto pluginManager = Application::instance()->pluginManager();
    if (pluginManager) {
        pluginManager->wheelEvent(event);
    }
    if (event->isAccepted())
        return;

    mMapController->wheelEvent(event);

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
