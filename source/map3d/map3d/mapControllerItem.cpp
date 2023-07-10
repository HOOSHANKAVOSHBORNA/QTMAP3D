#include "mapControllerItem.h"

#include <QTimer>


MapControllerItem::MapControllerItem():
    MapItem()
{
    setAcceptHoverEvents(true);
    setFlag(ItemAcceptsInputMethod, true);
}

void MapControllerItem::setZoomInButtonPressed(bool pressed)
{
    if (mZoomInButtonPressed != pressed) {
        mZoomInButtonPressed = pressed;
    }
}

void MapControllerItem::setZoomOutButtonPressed(bool pressed)
{
    if (mZoomOutButtonPressed != pressed) {
        mZoomOutButtonPressed = pressed;
    }
}

void MapControllerItem::setUpButtonPressed(bool pressed)
{
    if (mUpButtonPressed != pressed) {
        mUpButtonPressed = pressed;
    }

}

void MapControllerItem::setdownButtonPressed(bool pressed)
{
    if (mDownButtonPressed != pressed) {
        mDownButtonPressed = pressed;
    }
}

void MapControllerItem::setleftButtonPressed(bool pressed)
{
    if (mLeftButtonPressed != pressed) {
        mLeftButtonPressed = pressed;
    }
}

void MapControllerItem::setrightButtonPressed(bool pressed)
{
    if (mRightButtonPressed != pressed) {
        mRightButtonPressed = pressed;
    }
}

void MapControllerItem::setrotateUpButtonPressed(bool pressed)
{
    if (mRotateUpButtonPressed != pressed) {
        mRotateUpButtonPressed = pressed;
    }
}

void MapControllerItem::setrotateDownButtonPressed(bool pressed)
{
    if (mRotateDownButtonPressed != pressed) {
        mRotateDownButtonPressed = pressed;
    }
}

void MapControllerItem::setrotateLeftButtonPressed(bool pressed)
{
    if (mRotateLeftButtonPressed != pressed) {
        mRotateLeftButtonPressed = pressed;
    }
}

void MapControllerItem::setrotateRightButtonPressed(bool pressed)
{
    if (mRotateRightButtonPressed != pressed) {
        mRotateRightButtonPressed = pressed;
    }
}

void MapControllerItem::tickNavigation(double deltaTime)
{
    if (mZoomInButtonPressed) {
       getCameraController()->zoom(0.0018 * deltaTime);
    } else if (mZoomOutButtonPressed) {
        getCameraController()->zoom(-0.0018 * deltaTime);
    }

    if (mUpButtonPressed) {
        getCameraController()->pan(0.0, -0.0015 * deltaTime);
    } else if (mDownButtonPressed) {
        getCameraController()->pan(0.0, 0.0015 * deltaTime);
    } else if (mLeftButtonPressed) {
        getCameraController()->pan(0.0015 * deltaTime, 0.0);
    } else if (mRightButtonPressed) {
        getCameraController()->pan(-0.0015 * deltaTime, 0.0);
    }

    if (mRotateUpButtonPressed) {
        getCameraController()->rotate(0.0, 0.001 * deltaTime);
    } else if (mRotateDownButtonPressed) {
        getCameraController()->rotate(0.0, -0.001 * deltaTime);
    } else if (mRotateLeftButtonPressed) {
        getCameraController()->rotate(-0.001 * deltaTime, 0.0);
    } else if (mRotateRightButtonPressed) {
        getCameraController()->rotate(0.001 * deltaTime, 0.0);
    }
}

QVector3D MapControllerItem::mapMouseGeoLocation() const
{
    osgEarth::GeoPoint geoPos = mCurrentMouseGeoPoint;
    geoPos.makeGeographic();
    return QVector3D(geoPos.x(), geoPos.y(), geoPos.z());
}

QVector3D MapControllerItem::mapMouseLocation() const
{
    return QVector3D(mCurrentMouseGeoPoint.x(), mCurrentMouseGeoPoint.y(), mCurrentMouseGeoPoint.z());
}

void MapControllerItem::frame()
{
    MapItem::frame();
    static auto lastFrameTimePoint = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(10);
    auto now = std::chrono::high_resolution_clock::now();
    const double deltaTime =
        static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrameTimePoint).count())
        * 0.001;
    lastFrameTimePoint = now;
    tickNavigation(deltaTime);
    emit headingAngleChanged();
}

void MapControllerItem::mouseMoveEvent(QMouseEvent *event)
{
    MapItem::mouseMoveEvent(event);
    mCurrentMouseGeoPoint = screenToGeoPoint(event->position().x(), event->position().y());
    emit mouseLocationChanged();
}

void MapControllerItem::mousePressEvent(QMouseEvent *event)
{
    //qDebug()<<event->isAccepted();
    MapItem::mousePressEvent(event);
    //qDebug()<<event->isAccepted();

//    if (event->isAccepted())
//        return;

    if (event->button() == Qt::LeftButton) {
        mLastMousePressTime = QTime::currentTime();
        mLastPressPoint = event->pos();

        if (!mInClickProcess) {
            mMousePressOusideClickProcess = true;
        } else {
            mMousePressOusideClickProcess = false;
        }

    }
//    emit clicked();
}

void MapControllerItem::mouseReleaseEvent(QMouseEvent *event)
{
    MapItem::mouseReleaseEvent(event);
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

void MapControllerItem::hoverMoveEvent(QHoverEvent *event)
{
    MapItem::hoverMoveEvent(event);
//    qDebug() << event->position().x() << ", " << event->position().y() ;
    mCurrentMouseGeoPoint = screenToGeoPoint(event->position().x(), event->position().y());
    emit mouseLocationChanged();
    event->ignore();
}

qreal MapControllerItem::headingAngle() const
{
    return -getCameraController()->getViewpoint().heading()->as(osgEarth::Units::DEGREES);
}

void MapControllerItem::home()
{
    getCameraController()->home(0);
}

void MapControllerItem::setHeadingToNorth()
{
    getCameraController()->setHeadingToNorth();
}
