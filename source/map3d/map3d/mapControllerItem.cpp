#include "mapControllerItem.h"
#include "searchNodeModel.h"

#include <QTimer>


MapControllerItem::MapControllerItem():
    MapItem()
{
    qmlRegisterType<SearchNodeModel>("Crystal", 1, 0, "SearchModel");

    setAcceptHoverEvents(true);
    setFlag(ItemAcceptsInputMethod, true);
    mSearchNodeProxyModel = new SearchNodeProxyModel();
    mSearchNodeProxyModel->setSourceModel(new SearchNodeModel(this));
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

void MapControllerItem::setMoveXY(QVector2D &xy)
{
    if (mMoveXY != xy) {
        mMoveXY = xy;
        mMoveTick = true;
    }
}

void MapControllerItem::setRotateXY(QVector2D &xy)
{
    if (mRotateXY != xy) {
        mRotateXY  = xy;
        mRotateTick = true;
    }
    if(mRotateXY.x() == 0 && mRotateXY.y() == 0){
        mRotateTick = false;
    }
}


void MapControllerItem::tickNavigation(double deltaTime)
{
    if (mZoomInButtonPressed) {
       getCameraController()->zoom(0.0018 * deltaTime);
    } else if (mZoomOutButtonPressed) {
        getCameraController()->zoom(-0.0018 * deltaTime);
    }


    if(mMoveTick){
        getCameraController()->pan(mMoveXY.x()/5000  * deltaTime, mMoveXY.y()/5000 * deltaTime);

    }
    if(mRotateTick){
        getCameraController()->rotate(mRotateXY.x()/5000  * deltaTime, -mRotateXY.y()/5000 * deltaTime);
    }

}

void MapControllerItem::calculateNavigationStep()
{
    static auto lastFrameTimePoint = std::chrono::high_resolution_clock::now() - std::chrono::milliseconds(10);
    auto now = std::chrono::high_resolution_clock::now();
    const double deltaTime =
        static_cast<double>(std::chrono::duration_cast<std::chrono::microseconds>(now - lastFrameTimePoint).count())
        * 0.001;
    lastFrameTimePoint = now;
    tickNavigation(deltaTime);
}

void MapControllerItem::calculateFps()
{
    static std::deque<std::chrono::high_resolution_clock::time_point> timePointList;
    const auto nowTimePoint = std::chrono::high_resolution_clock::now();

    timePointList.push_back(nowTimePoint);
    while (timePointList.size() > 2) {
        if (std::chrono::duration_cast<std::chrono::milliseconds>
            (nowTimePoint - timePointList.front()).count() > 2000) {
            timePointList.pop_front();
        } else {
            break;
        }
    }

    if (timePointList.size() >= 2) {
        const qreal duration = qreal(std::chrono::duration_cast<std::chrono::milliseconds>(timePointList.back() - timePointList.front()).count());
        if (duration > 0) {
            const qreal fps = qreal((timePointList.size()-1) * 1000) / duration;
            setFps(fps);
        }
    }
}

SearchNodeProxyModel *MapControllerItem::searchNodeProxyModel() const
{
    return mSearchNodeProxyModel;
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

    calculateFps();
    calculateNavigationStep();
    emit compassDirectionChanged();
}

void MapControllerItem::mouseMoveEvent(QMouseEvent *event)
{
    MapItem::mouseMoveEvent(event);
    mCurrentMouseGeoPoint = screenToGeoPoint(event->position().x(), event->position().y());
    emit mouseLocationChanged();
}

void MapControllerItem::mousePressEvent(QMouseEvent *event)
{
    MapItem::mousePressEvent(event);

    if (getCameraController()->getEventHandled())
        return;

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
    mCurrentMouseGeoPoint = screenToGeoPoint(event->position().x(), event->position().y());
    emit mouseLocationChanged();
    event->ignore();
}

double MapControllerItem::fps() const
{
    return mFps;
}

void MapControllerItem::setFps(double fps)
{
    if (mFps != fps) {
        mFps = fps;
        emit fpsChanged();
    }
}

void MapControllerItem::home()
{
    getCameraController()->home(0);
}

void MapControllerItem::setHeadingToNorth()
{
    getCameraController()->setHeadingToNorth();
}

QVector2D MapControllerItem::compassDirection() const
{
    double headingAngle = getCameraController()->getViewpoint().heading()->as(osgEarth::Units::DEGREES);
    double pitchAngle = 180 - getCameraController()->getViewpoint().pitch()->as(osgEarth::Units::DEGREES);

    return QVector2D(headingAngle, pitchAngle);
}

void MapControllerItem::setCompassDirection(const QVector2D &newCompassDirection)
{
    if (m_compassDirection == newCompassDirection)
        return;
    m_compassDirection = newCompassDirection;
    emit compassDirectionChanged();
}
