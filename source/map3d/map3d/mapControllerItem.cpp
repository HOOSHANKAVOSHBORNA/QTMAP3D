#include "mapControllerItem.h"


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
       zoom(0.0018 * deltaTime);
    } else if (mZoomOutButtonPressed) {
        zoom(-0.0018 * deltaTime);
    }

    if (mUpButtonPressed) {
        pan(0.0, -0.0015 * deltaTime);
    } else if (mDownButtonPressed) {
        pan(0.0, 0.0015 * deltaTime);
    } else if (mLeftButtonPressed) {
        pan(0.0015 * deltaTime, 0.0);
    } else if (mRightButtonPressed) {
        pan(-0.0015 * deltaTime, 0.0);
    }

    if (mRotateUpButtonPressed) {
        rotate(0.0, 0.001 * deltaTime);
    } else if (mRotateDownButtonPressed) {
        rotate(0.0, -0.001 * deltaTime);
    } else if (mRotateLeftButtonPressed) {
        rotate(-0.001 * deltaTime, 0.0);
    } else if (mRotateRightButtonPressed) {
        rotate(0.001 * deltaTime, 0.0);
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

void MapControllerItem::hoverMoveEvent(QHoverEvent *event)
{
    MapItem::hoverMoveEvent(event);
//    qDebug() << event->position().x() << ", " << event->position().y() ;
    mCurrentMouseGeoPoint = screenToGeoPoint(event->position().x(), event->position().y());
    emit mouseLocationChanged();
    event->ignore();
}
