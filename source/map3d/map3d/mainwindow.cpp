
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
#include "pluginmanager.h"
#include "plugininterface.h"
#include "mapItem.h"
#include "listwindow.h"

#include <QQuickOpenGLUtils>
MainWindow::MainWindow(QWindow *parent) :
    QQuickWindow(parent)
{

//    mOGLF = new QOpenGLFunctions_2_0;
    QObject::connect(this, &MainWindow::sceneGraphInitialized,
                     this, &MainWindow::initializeGL,
                     Qt::DirectConnection);
//    QObject::connect(this, &MainWindow::sceneGraphInvalidated,
//                     this, &MainWindow::cleanup,
//                     Qt::DirectConnection);

//    setClearBeforeRendering(false);
    setColor(Qt::black);
    mToolbox = new ToolboxProxyModel();
    Toolbox *toolbox = new Toolbox();
    mToolbox->setSourceModel(toolbox);
//    setToolbox(toolbox);

//    LayersModel *layerModel = new LayersModel;







///////////////////
//    QObject::connect(this, &MainWindow::homeButtonClicked,
//                     mMapItem, &MapItem::goToHome);
//    QObject::connect(this, &MainWindow::view3DButtonClicked,
//                     mMapItem, &MapItem::toggle3DView);
//////////////////////

//    QObject::connect(this, &MainWindow::upButtonClicked,
//                     mMapItem, &MapItem::panUp);
//    QObject::connect(this, &MainWindow::downButtonClicked,
//                     mMapItem, &MapItem::panDown);
//    QObject::connect(this, &MainWindow::leftButtonClicked,
//                     mMapItem, &MapItem::panLeft);
//    QObject::connect(this, &MainWindow::rightButtonClicked,
//                     mMapItem, &MapItem::panRight);
//
//
//    QObject::connect(this, &MainWindow::rotateUpButtonClicked,
//                     mMapItem, &MapItem::rotateUp);
//    QObject::connect(this, &MainWindow::rotateDownButtonClicked,
//                     mMapItem, &MapItem::rotateDown);
//    QObject::connect(this, &MainWindow::rotateLeftButtonClicked,
//                     mMapItem, &MapItem::rotateLeft);
//    QObject::connect(this, &MainWindow::rotateRightButtonClicked,
//                     mMapItem, &MapItem::rotateRight);
//
//
//    QObject::connect(this, &MainWindow::zoomInButtonClicked,
//                     mMapItem, &MapItem::zoomIn);
//    QObject::connect(this, &MainWindow::zoomOutButtonClicked,
//                     mMapItem, &MapItem::zoomOut);
///////////////
//    QObject::connect(mMapItem, &MapItem::headingAngleChanged,
//                     this, &MainWindow::setHeadingAngle);


//    QObject::connect(mMapItem, &MapItem::mousePointingLocationWgs84Changed,
//                     this, &MainWindow::setMousePointingLocationWgs84);
//    QObject::connect(mMapItem, &MapItem::mousePointingLocationChanged,
//                     this, &MainWindow::setMousePointingLocation);
//    QObject::connect(this, &MainWindow::goToLocation,
//                     mMapItem, QOverload<double, double, double>::of(&MapItem::goToPosition));


//    QObject::connect(mMapItem, &MapItem::focalPointLatChanged,
//                     this, &MainWindow::setFocalPointLat);
//    QObject::connect(mMapItem, &MapItem::focalPointLongChanged,
//                     this, &MainWindow::setFocalPointLong);
//    QObject::connect(mMapItem, &MapItem::focalPointRangeChanged,
//                     this, &MainWindow::setFocalPointRange);
//    QObject::connect(mMapItem, &MapItem::focalPointPitchChanged,
//                     this, &MainWindow::setFocalPointPitch);
//    QObject::connect(mMapItem, &MapItem::focalPointHeadChanged,
//                     this, &MainWindow::setFocalPointHead);

////////////////


    mUIHandle = new UIHandle(this);

//    QObject::connect(this, &MainWindow::infoWidget2D3DButtonClicked, [this](){
//        mUIHandle->onInfoWidget2D3DButtonClicked();
//    });
//    QObject::connect(this, &MainWindow::infoWidgetRouteButtonClicked, [this](){
//        mUIHandle->onInfoWidgetRouteButtonClicked();
//    });
//    QObject::connect(this, &MainWindow::infoWidgetFollowButtonClicked, [this](){
//        mUIHandle->onInfoWidgetFollowButtonClicked();
//    });
//    QObject::connect(this, &MainWindow::infoWidgetMoreButtonClicked, [this](){
//        mUIHandle->onInfoWidgetMoreButtonClicked();
//    });



    QObject::connect(this, &MainWindow::frameSwapped,
                     this, &MainWindow::onFrameSwapped,
                     Qt::DirectConnection);

}


MainWindow::~MainWindow()
{
//    cleanup();
//    mMapItem->deleteLater();
}

//qreal MainWindow::headingAngle() const
//{
//    return mheadingAngle;
//}

//QVector3D MainWindow::mousePointingLocationWgs84() const
//{
//    return mMousePointingLocationWgs84;
//}

//QVector3D MainWindow::mousePointingLocation() const
//{
//    return mMousePointingLocation;
//}


//qreal MainWindow::focalPointLat() const
//{
//    return mFocalPointLat;
//}

//qreal MainWindow::focalPointLong() const
//{
//    return mFocalPointLong;
//}

//qreal MainWindow::focalPointRange() const
//{
//    return mFocalPointRange;

//}

//qreal MainWindow::focalPointPitch() const
//{
//    return mFocalPointPitch;
//}

//qreal MainWindow::focalPointHead() const
//{
//    return mFocalPointHead;
//}

qreal MainWindow::fps() const
{
    return mFps;
}

//bool MainWindow::zoomInButtonPressed() const
//{
//    return mZoomInButtonPressed;
//}

//bool MainWindow::zoomOutButtonPressed() const
//{
//    return mZoomOutButtonPressed;
//}

//bool MainWindow::upButtonPressed() const
//{
//    return mUpButtonPressed;
//}

//bool MainWindow::downButtonPressed() const
//{
//    return mDownButtonPressed;
//}

//bool MainWindow::leftButtonPressed() const
//{
//    return mLeftButtonPressed;
//}

//bool MainWindow::rightButtonPressed() const
//{
//    return mRightButtonPressed;
//}

//bool MainWindow::rotateUpButtonPressed() const
//{
//    return mRotateUpButtonPressed;
//}

//bool MainWindow::rotateDownButtonPressed() const
//{
//    return mRotateDownButtonPressed;
//}

//bool MainWindow::rotateLeftButtonPressed() const
//{
//    return mRotateLeftButtonPressed;
//}

//bool MainWindow::rotateRightButtonPressed() const
//{
//    return mRotateRightButtonPressed;
//}

LayersModel *MainWindow::layersModel() const
{
    return mLayersModel;
}

LayersProxyModel *MainWindow::layersProxyModel() const
{
    return mLayersProxyModel;
}

ToolboxProxyModel *MainWindow::toolbox() const
{
    return mToolbox;
}

UIHandle *MainWindow::uiHandle() const
{
    return mUIHandle;
}

void MainWindow::initializePluginsUI(std::list<PluginInfo>& pluginsInfoList)
{


//    for (auto& item : pluginsInfoList) {

//        if (item.qmlDesc->pluginHasSideItem) {
//            QVariant ret;
//            QMetaObject::invokeMethod(this,
//                                      "addSideItem",
//                                      Qt::DirectConnection,
//                                      Q_RETURN_ARG(QVariant, ret),
//                                      Q_ARG(QVariant, QVariant::fromValue<QString>(item.qmlDesc->sideItemMenuBarTitle)),
//                                      Q_ARG(QVariant, QVariant::fromValue<QString>(item.qmlDesc->sideItemMenuBarIconUrl)),
//                                      Q_ARG(QVariant, QVariant::fromValue<QString>(item.qmlDesc->sideItemUrl))
//                                      );

//            bool bOk = false;
//            const int idx = ret.toInt(&bOk);
//            if (bOk) {
//                item.sideItemIndex = idx;
//            }
//        }

//        for (auto toolboxItemDesc : item.qmlDesc->toolboxItemsList)
//        {
////            qDebug() <<"item name:"<<toolboxItemDesc->name;
//            QVariant ret;
//            ItemDescProxy proxy(*toolboxItemDesc, item.interface);
//            QMetaObject::invokeMethod(this,
//                                      "addToolboxItem",
//                                      Qt::DirectConnection,
//                                      Q_RETURN_ARG(QVariant, ret),
//                                      Q_ARG(QVariant, QVariant::fromValue<ItemDescProxy*>(&proxy))
//                                      );
//        }
//        for (auto fileItem : item.qmlDesc->fileItemsList)
//        {
//            QVariant ret;
//            ItemDescProxy proxy(*fileItem, item.interface);
//            QMetaObject::invokeMethod(this,
//                                      "addFileItem",
//                                      Qt::DirectConnection,
//                                      Q_RETURN_ARG(QVariant, ret),
//                                      Q_ARG(QVariant, QVariant::fromValue<ItemDescProxy*>(&proxy))
//                                      );
//        }
//    }
}

//void MainWindow::setHeadingAngle(qreal angle)
//{
//    if (mheadingAngle != angle) {
//        mheadingAngle = angle;
//        emit headingAngleChanged(angle);
//    }
//}

//void MainWindow::setMousePointingLocationWgs84(const QVector3D &pointedLoc)
//{
//    if (mMousePointingLocationWgs84 != pointedLoc) {
//        mMousePointingLocationWgs84 = pointedLoc;
//        emit mousePointingLocationWgs84Changed();
//    }
//}

//void MainWindow::setMousePointingLocation(const QVector3D &pointedLoc)
//{
//    if (mMousePointingLocation != pointedLoc) {
//        mMousePointingLocation = pointedLoc;
//        emit mousePointingLocationChanged();
//    }
//}


//void MainWindow::setFocalPointLat(qreal focalPointLat)
//{
//    if (mFocalPointLat != focalPointLat) {
//        mFocalPointLat = focalPointLat;
//        emit focalPointLatChanged();
//    }
//}

//void MainWindow::setFocalPointLong(qreal focalPointLong)
//{
//    if (mFocalPointLong != focalPointLong) {
//        mFocalPointLong = focalPointLong;
//        emit focalPointLongChanged();
//    }
//}

//void MainWindow::setFocalPointRange(qreal focalPointRange)
//{
//    if (mFocalPointRange != focalPointRange) {
//        mFocalPointRange = focalPointRange;
//        emit focalPointRangeChanged();
//    }

//}

//void MainWindow::setFocalPointPitch(qreal focalPointPitch)
//{
//    if (mFocalPointPitch != focalPointPitch) {
//        mFocalPointPitch = focalPointPitch;
//        emit focalPointPitchChanged();
//    }
//}

//void MainWindow::setFocalPointHead(qreal focalPointHead)
//{
//    if (mFocalPointHead != focalPointHead) {
//        mFocalPointHead = focalPointHead;
//        emit focalPointHeadChanged();
//    }
//}

void MainWindow::setFps(qreal fps)
{
    if (mFps != fps) {
        mFps = fps;
        emit fpsChanged();
    }
}

//void MainWindow::setZoomInButtonPressed(bool pressed)
//{
//    if (mZoomInButtonPressed != pressed) {
//        mZoomInButtonPressed = pressed;
//        emit zoomInButtonPressedChanged();
//    }
//}

//void MainWindow::setZoomOutButtonPressed(bool pressed)
//{
//    if (mZoomOutButtonPressed != pressed) {
//        mZoomOutButtonPressed = pressed;
//        emit zoomOutButtonPressedChanged();
//    }
//}

//void MainWindow::setUpButtonPressed(bool pressed)
//{
//    if (mUpButtonPressed != pressed) {
//        mUpButtonPressed = pressed;
//        emit upButtonPressedChanged();
//    }

//}

//void MainWindow::setdownButtonPressed(bool pressed)
//{
//    if (mDownButtonPressed != pressed) {
//        mDownButtonPressed = pressed;
//        emit downButtonPressedChanged();
//    }
//}

//void MainWindow::setleftButtonPressed(bool pressed)
//{
//    if (mLeftButtonPressed != pressed) {
//        mLeftButtonPressed = pressed;
//        emit leftButtonPressedChanged();
//    }
//}

//void MainWindow::setrightButtonPressed(bool pressed)
//{
//    if (mRightButtonPressed != pressed) {
//        mRightButtonPressed = pressed;
//        emit rightButtonPressedChanged();
//    }
//}

//void MainWindow::setrotateUpButtonPressed(bool pressed)
//{
//    if (mRotateUpButtonPressed != pressed) {
//        mRotateUpButtonPressed = pressed;
//        emit rotateUpButtonPressedChanged();
//    }
//}

//void MainWindow::setrotateDownButtonPressed(bool pressed)
//{
//    if (mRotateDownButtonPressed != pressed) {
//        mRotateDownButtonPressed = pressed;
//        emit rotateDownButtonPressedChanged();
//    }
//}

//void MainWindow::setrotateLeftButtonPressed(bool pressed)
//{
//    if (mRotateLeftButtonPressed != pressed) {
//        mRotateLeftButtonPressed = pressed;
//        emit rotateLeftButtonPressedChanged();
//    }
//}

//void MainWindow::setrotateRightButtonPressed(bool pressed)
//{
//    if (mRotateRightButtonPressed != pressed) {
//        mRotateRightButtonPressed = pressed;
//        emit rotateRightButtonPressedChanged();
//    }
//}

//void MainWindow::travelToViewpoint(qreal latitude, qreal longitude, qreal range, qreal pitch, qreal heading)
//{
////    mMapItem->travelToViewpoint(latitude,
////                                      longitude,
////                                      range,
////                                      pitch,
////                                      heading);
//}

void MainWindow::showInfoWidget(MainWindow::InfoWidgetType infoWidgetType)
{

}

void MainWindow::updateInfoWidgetData(const QString &dataJSON)
{

}



//void MainWindow::setStatusBarText(const QString message, int time) {
//    QVariant ret;
//    QMetaObject::invokeMethod(this,
//                              "showStatusMessage",
//                              Qt::DirectConnection,
//                              Q_ARG(QVariant, QVariant::fromValue<QString>(message)),
//                              Q_ARG(QVariant, QVariant::fromValue<int>(time))
//                              );
//}

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
}

void MainWindow::setLayersProxyModel(LayersProxyModel *layer)
{
    mLayersProxyModel = layer;
    emit layersModelChanged();
}

void MainWindow::onFrameSwapped()
{

    //
    // Calculate FPS
    //
    static std::deque<std::chrono::high_resolution_clock::time_point> timepoint_list;
    const auto now_timepoint = std::chrono::high_resolution_clock::now();

    timepoint_list.push_back(now_timepoint);
    while (timepoint_list.size() > 2) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>
                (now_timepoint - timepoint_list.front()).count() > 2000) {
            timepoint_list.pop_front();
        } else {
            break;
        }
    }

    if (timepoint_list.size() >= 2) {
        const qreal duration = qreal(std::chrono::duration_cast<std::chrono::milliseconds>(timepoint_list.back() - timepoint_list.front()).count());
        if (duration > 0) {
            const qreal fps = qreal((timepoint_list.size()-1) * 1000) / duration;
            setFps(fps);
        }
    }

}

//void MainWindow::orientCameraToNorth()
//{
////    if (mMapItem)
////        mMapItem->orientCameraToNorth();
//}







MapItem *MainWindow::getMapItem()
{
    if(!mMapItem)
        mMapItem = findChild<MapItem*>("MainMap");
//    if (mapItem)
//        qDebug()<<"mapitem"<<mapItem->getMode();
    return mMapItem;
}

//void MainWindow::setMapItem(MapItem &mapItem)
//{
//    mMapItem = &mapItem;
//}


//void MainWindow::cleanup()
//{
////    mMapItem->cleanup();
//}

void MainWindow::frame()
{
//    static auto lastFrameTimePoint = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(10);
//    auto now = std::chrono::high_resolution_clock::now();


//    const double deltaTime =
//            static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrameTimePoint).count())
//            * 0.001;


//    if (mResized) {
////        resizeGL();
//        mResized = false;
//    }

//    tickNavigation(deltaTime);
////    paintGL();

//    const auto pluginManager = Application::instance()->pluginManager();
//    if (pluginManager) {
//        pluginManager->frameEvent();
//    }

//    lastFrameTimePoint = now;

}

void MainWindow::tickNavigation(double deltaTime)
{
//    if (mZoomInButtonPressed) {
//        mMapItem->zoom(0.0018 * deltaTime);
//    } else if (mZoomOutButtonPressed) {
//        mMapItem->zoom(-0.0018 * deltaTime);
//    }

//    if (mUpButtonPressed) {
//        mMapItem->pan(0.0, -0.0015 * deltaTime);
//    } else if (mDownButtonPressed) {
//        mMapItem->pan(0.0, 0.0015 * deltaTime);
//    } else if (mLeftButtonPressed) {
//        mMapItem->pan(0.0015 * deltaTime, 0.0);
//    } else if (mRightButtonPressed) {
//        mMapItem->pan(-0.0015 * deltaTime, 0.0);
//    }

//    if (mRotateUpButtonPressed) {
//        mMapItem->rotate(0.0, 0.001 * deltaTime);
//    } else if (mRotateDownButtonPressed) {
//        mMapItem->rotate(0.0, -0.001 * deltaTime);
//    } else if (mRotateLeftButtonPressed) {
//        mMapItem->rotate(-0.001 * deltaTime, 0.0);
//    } else if (mRotateRightButtonPressed) {
//        mMapItem->rotate(0.001 * deltaTime, 0.0);
//    }
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

//    QQuickOpenGLUtils::resetOpenGLState();
//    mOGLF->initializeOpenGLFunctions();

//    mContext = QOpenGLContext::currentContext();
//    mSurface = mContext->surface();


//    mOGLF->glViewport(0, 0, mViewportWidth, mViewportHeight);
////    mMapItem->initializeGL(width(), height(), screen(), 1);

//    restoreContext();
//    mMapItem->initializeOsgEarth();
//    restoreContext();
//    emit osgInitialized();
    //----------------------------------------------------------

    LayersModel *layersModel = new LayersModel(getMapItem());
    setLayersModel(layersModel);

    mLayersProxyModel = new LayersProxyModel();
    mLayersProxyModel->setSourceModel(layersModel);
    setLayersProxyModel(mLayersProxyModel);



    QObject::connect(this, &MainWindow::toggleLayerEnabled,
                     layersModel, &LayersModel::toggleLayerEnabled);

    //----------------------------------------------------------
    restoreContext();

//    resetOpenGLState();

//    QObject::connect(this, &MainWindow::beforeRendering,
//                     this, &MainWindow::frame,
//                     Qt::DirectConnection);
//    QQuickOpenGLUtils::resetOpenGLState();
}

//void MainWindow::resizeGL()
//{
//    mOGLF->glViewport(0, 0, mViewportWidth, mViewportHeight);
//    mMapItem->resizeGL(mViewportWidth, mViewportHeight, screen());
//}

//void MainWindow::paintGL()
//{
//    QQuickOpenGLUtils::resetOpenGLState();
////    resetOpenGLState();
////    mOGLF->glClearColor(0,0,0,1);
////    mOGLF->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
////    mOGLF->glViewport(0, 0, mViewportWidth, mViewportHeight);
//    mMapItem->paintGL();
////    mOGLF->glClear(GL_DEPTH_BUFFER_BIT);
////    mOGLF->glViewport(0, 0, mViewportWidth, mViewportHeight);
////    resetOpenGLState();
////    QQuickOpenGLUtils::resetOpenGLState();
//}

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


//    switch (event->key()) {
//    case Qt::Key_Up:
//    {
//        auto manip = mMapItem->getEarthManipulator();
//        if (manip) {
//            manip->pan(0, -0.03);
//        }
//        event->accept();
//    }
//        break;
//    case Qt::Key_Down:
//    {
//        auto manip = mMapItem->getEarthManipulator();
//        if (manip) {
//            manip->pan(0, 0.03);
//        }
//        event->accept();
//    }
//        break;
//    case Qt::Key_Left:
//    {
//        auto manip = mMapItem->getEarthManipulator();
//        if (manip) {
//            manip->pan(0.03, 0);
//        }
//        event->accept();
//    }
//        break;
//    case Qt::Key_Right:
//    {
//        auto manip = mMapItem->getEarthManipulator();
//        if (manip) {
//            manip->pan(-0.03, 0);
//        }
//        event->accept();
//    }
//        break;
//    }


//    if (event->isAccepted())
//        return;

//    const auto pluginManager = Application::instance()->pluginManager();
//    if (pluginManager) {
//        pluginManager->keyPressEvent(event);
//    }

//    if (event->isAccepted())
//        return;

//    mMapItem->keyPressEvent(event);


}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QQuickWindow::keyReleaseEvent(event);

    if (event->isAccepted())
        return;

//    switch (event->key()) {
//    case Qt::Key_Up:
//    case Qt::Key_Down:
//    case Qt::Key_Left:
//    case Qt::Key_Right:
//        event->accept();
//        break;
//    }

//    if (event->isAccepted())
//        return;

//    const auto pluginManager = Application::instance()->pluginManager();
//    if (pluginManager) {
//        pluginManager->keyReleaseEvent(event);
//    }
//    if (event->isAccepted())
//        return;

//    mMapItem->keyReleaseEvent(event);
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    QQuickWindow::mousePressEvent(event);

    if (event->isAccepted())
        return;


//    const auto pluginManager = Application::instance()->pluginManager();
//    if (pluginManager) {
//        pluginManager->mousePressEvent(event);
//    }
//    if (event->isAccepted())
//        return;


//    mMapItem->mousePressEvent(event);


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

//    const auto pluginManager = Application::instance()->pluginManager();
//    if (pluginManager) {
//        pluginManager->mouseReleaseEvent(event);
//    }
//    if (event->isAccepted())
//        return;


//    mMapItem->mouseReleaseEvent(event);


//    if (event->button() == Qt::LeftButton) {
//        if (mLastMousePressTime.msecsTo(QTime::currentTime()) < 400) {
//            const QPoint diff = event->pos() - mLastPressPoint;
//            if (std::abs(diff.x()) < 10 && std::abs(diff.y()) < 10) {

//                if (!mInClickProcess && mMousePressOusideClickProcess) {
//                    mInClickProcess = true;
//                    QTimer::singleShot(300, [this](){
//                        if (mMousePressOusideClickProcess)
//                            emit clicked();
//                        this->mInClickProcess = false;
//                    });

//                }
//            }
//        }
//    }

}

void MainWindow::mouseDoubleClickEvent(QMouseEvent *event)
{

    QQuickWindow::mouseDoubleClickEvent(event);

    if (event->isAccepted())
        return;

//    const auto pluginManager = Application::instance()->pluginManager();
//    if (pluginManager) {
//        pluginManager->mouseDoubleClickEvent(event);
//    }
//    if (event->isAccepted())
//        return;


//    mMapItem->mouseDoubleClickEvent(event);

}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    QQuickWindow::mouseMoveEvent(event);

    if (event->isAccepted())
        return;

//    const auto pluginManager = Application::instance()->pluginManager();
//    if (pluginManager) {
//        pluginManager->mouseMoveEvent(event);
//    }
//    if (event->isAccepted())
//        return;


//    mMapItem->mouseMoveEvent(event);

}

void MainWindow::wheelEvent(QWheelEvent *event)
{
    QQuickWindow::wheelEvent(event);

    if (event->isAccepted())
        return;

//    const auto pluginManager = Application::instance()->pluginManager();
//    if (pluginManager) {
//        pluginManager->wheelEvent(event);
//    }
//    if (event->isAccepted())
//        return;

//    mMapItem->wheelEvent(event);

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
