#include "getdata.h"
#include <QDebug>
GetData::GetData(QObject *parent) : QObject(parent)
{

}

void GetData::setZoomStep(int zoomstep)
{
    mZoomStep=zoomstep;
}

void GetData::setUpDownStep(int updownstep)
{
    mUpDownstep=updownstep;
}

void GetData::setLeftRightStep(int leftrightstep)
{
    mLeftRightStep=leftrightstep;
}

void GetData::setPitchStep(int pitch)
{
    mPitchStep=pitch;

}

void GetData::setHeadStep(int head)
{
    mHeadStep =head;
}

void GetData::onZoomClicked(int zoom)
{
    emit zoomChanged(zoom);
}

void GetData::onUPDownClicked(int updown)
{
    emit upDownChanged(updown);
}

void GetData::onPitchClicked(int pitch)
{
    emit pitchChanged(pitch);
}

void GetData::onHeadClicked(int head)
{
    emit headChanged(head);
}

void GetData::onHomeClicked()
{
    emit homeClicked();

}

void GetData::onLeftRightClicked(int left_right)
{
    emit leftRightChanged(left_right);
}



 int GetData::getZoomStep() const
{
     return mZoomStep;
 }

 int GetData::getUpDownStep() const
 {
     return mUpDownstep;
 }

 int GetData::getLeftRightStep() const
 {
     return  mLeftRightStep;
 }

 int GetData::getPitchStep() const
 {
     return mPitchStep;
 }

 int GetData::getHeadStep() const
{
     return mHeadStep;
}




