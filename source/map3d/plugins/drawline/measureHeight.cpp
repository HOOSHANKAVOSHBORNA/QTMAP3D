#include "measureHeight.h"

MeasureHeight::MeasureHeight(MapController *mapController)
{
    mMapController = mapController;
    mHLine = new LineNode(mMapController);
    mVLine = new LineNode(mMapController);
    mVLine->setWidth(5);
    mHLine->setWidth(5);
    mHLine->showLenght(true);
    mVLine->showLenght(false);
    addChild(mVLine);
    addChild(mHLine);
    mStarted = false;
}

void MeasureHeight::setFirstPoint(osgEarth::GeoPoint P1)
{
    mFirstPoint = P1;
    mStarted = true;
}

void MeasureHeight::setSecondPoint(osgEarth::GeoPoint P2)
{
    mSecondPoint = P2;
    draw();
}

void MeasureHeight::clear()
{
    mVLine->clear();
    mHLine->clear();
}

void MeasureHeight::draw()
{
    clear();
    if(height()>=0)
    {
        mHLine->addPoint(mFirstPoint);
        mVLine->addPoint(mSecondPoint);
        mJointPoint.set(
                    mMapController->getMapSRS(), mFirstPoint.x(), mFirstPoint.y(),
                    mSecondPoint.z(), osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
    }
    else {
        mHLine->addPoint(mSecondPoint);
        mVLine->addPoint(mFirstPoint);
        mJointPoint.set(
                    mMapController->getMapSRS(), mSecondPoint.x(), mSecondPoint.y(),
                    mFirstPoint.z(), osgEarth::AltitudeMode::ALTMODE_ABSOLUTE);
    }
    mHLine->addPoint(mJointPoint);
    mVLine->addPoint(mJointPoint);
}

double MeasureHeight::height()
{
    return  mSecondPoint.z() - mFirstPoint.z();
}

float MeasureHeight::getWidth() const
{
    return mWidth;
}

void MeasureHeight::setWidth(float width)
{
    mWidth = width;
    mVLine->setWidth(mWidth);
    mHLine->setWidth(mWidth);
}

osgEarth::Color MeasureHeight::getColor() const
{
    return color;
}

void MeasureHeight::setColor(const osgEarth::Color &value)
{
    color = value;
    mVLine->setColor(color);
    mHLine->setColor(color);
}

bool MeasureHeight::started() const
{
    return mStarted;
}
