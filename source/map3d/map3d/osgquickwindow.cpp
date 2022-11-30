
#include <QResizeEvent>
#include <iostream>
#include <QScreen>
#include <osgGA/FirstPersonManipulator>
#include <osgGA/TrackballManipulator>
#include <osg/ShapeDrawable>
#include <osg/Shape>
#include <QThread>
#include <QtOpenGL/QtOpenGL>
#include <iostream>

#include "osgquickwindow.h"
#include "mapcontroller.h"

OsgQuickWindow::OsgQuickWindow(QWindow *parent) :
    QQuickWindow(parent),
    mMapController(new MapController(this))
{
    mOGLF = new QOpenGLFunctions_2_0;


    QObject::connect(this, &OsgQuickWindow::sceneGraphInitialized,
                     this, &OsgQuickWindow::initializeGL,
                     Qt::DirectConnection);
    QObject::connect(this, &OsgQuickWindow::sceneGraphInvalidated,
                     this, &OsgQuickWindow::cleanup,
                     Qt::DirectConnection);

    setClearBeforeRendering(false);
    setColor(Qt::black);

}

OsgQuickWindow::~OsgQuickWindow()
{
    cleanup();
    mMapController->deleteLater();
}

MapController *OsgQuickWindow::mapController() const
{
    return mMapController;
}


void OsgQuickWindow::cleanup()
{
    mMapController->cleanup();
}

void OsgQuickWindow::frame()
{
    if (mResized) {
        resizeGL();
        mResized = false;
    }

    paintGL();
}

void OsgQuickWindow::restoreContext()
{
    if (mContext && mSurface) {
        mContext->makeCurrent(mSurface);
    }
}


void OsgQuickWindow::initializeGL()
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

    QObject::connect(this, &OsgQuickWindow::beforeRendering,
                     this, &OsgQuickWindow::frame,
                     Qt::DirectConnection);
}

void OsgQuickWindow::resizeGL()
{
    mMapController->resizeGL(mViewportWidth, mViewportHeight, screen());
}

void OsgQuickWindow::paintGL()
{
    resetOpenGLState();

    mMapController->paintGL();

    mOGLF->glClear(GL_DEPTH_BUFFER_BIT);

    resetOpenGLState();
}

void OsgQuickWindow::resizeEvent(QResizeEvent *ev)
{
    QQuickWindow::resizeEvent(ev);

    const QSize s = ev->size();
    mResized       = true;
    mViewportWidth  = s.width();
    mViewportHeight = s.height();

}

void OsgQuickWindow::keyPressEvent(QKeyEvent *event)
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

    mMapController->keyPressEvent(event);


}

void OsgQuickWindow::keyReleaseEvent(QKeyEvent *event)
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

    mMapController->keyReleaseEvent(event);
}

void OsgQuickWindow::mousePressEvent(QMouseEvent *event)
{
    QQuickWindow::mousePressEvent(event);

    if (event->isAccepted())
        return;

    mMapController->mousePressEvent(event);


    if (event->button() == Qt::LeftButton) {
        mLastMousePressTime = QTime::currentTime();
        mLastPressPoint = event->pos();
    }
}

void OsgQuickWindow::mouseReleaseEvent(QMouseEvent *event)
{
    QQuickWindow::mouseReleaseEvent(event);

    if (event->isAccepted())
        return;

    mMapController->mouseReleaseEvent(event);


    if (event->button() == Qt::LeftButton) {
        if (mLastMousePressTime.msecsTo(QTime::currentTime()) < 400) {
            const QPoint diff = event->pos() - mLastPressPoint;
            if (std::abs(diff.x()) < 10 && std::abs(diff.y()) < 10) {
                emit clicked();
            }
        }
    }
}

void OsgQuickWindow::mouseDoubleClickEvent(QMouseEvent *event)
{
    QQuickWindow::mouseDoubleClickEvent(event);

    if (event->isAccepted())
        return;

    mMapController->mouseDoubleClickEvent(event);


}

void OsgQuickWindow::mouseMoveEvent(QMouseEvent *event)
{
    QQuickWindow::mouseMoveEvent(event);

    if (event->isAccepted())
        return;

    mMapController->mouseMoveEvent(event);

}

void OsgQuickWindow::wheelEvent(QWheelEvent *event)
{
    QQuickWindow::wheelEvent(event);

    if (event->isAccepted())
        return;


    mMapController->wheelEvent(event);

}

