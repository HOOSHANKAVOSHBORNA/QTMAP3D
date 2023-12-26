#include "property.h"
#include <QDebug>

Property::Property(QQuickItem *parent)
    : QQuickItem{parent}
{

}
//---------------------- fillcolor ---------------------//
bool Property::getFillColorStatus() const
{
    return mFillColorStatus;
}

void Property::setFillColorStatus(bool status)
{
    mFillColorStatus = status;
    emit propretyChanged();
}

QColor Property::getFillColor() const
{
    return mFillColor;
}

void Property::setFillColor(const QColor &color)
{
    mFillColor = color;
    emit propretyChanged();

}

QString Property::getName() const
{
    return mName;
}

void Property::setName(const QString &name)
{
    mName = name;
    emit propretyChanged();
}
//-------------------- stroke -------------------- //

bool Property::getStrokeStatus() const
{
    return mStrokeStatus;
}

void Property::setStrokeStatus(bool status)
{
    mStrokeStatus = status;
    emit propretyChanged();
}

double Property::getStrokeWidth() const
{
    return mStrokeWidth;
}

void Property::setStrokeWidth(double width)
{
    mStrokeWidth = width;
    emit propretyChanged();

}



QColor Property::getStrokeColor() const
{
    return mStroke;
}

void Property::setStrokeColor(const QColor &color)
{
    mStroke = color;
    emit propretyChanged();

}

//------------------height---------------//
bool Property::getHeightStatus() const
{
    return mHeightStatus;
}

void Property::setHeightStatus(bool status)
{
    mHeightStatus = status;
    emit propretyChanged();

}

double Property::getHeight() const
{
    return mHeight;
}

void Property::setHeight(double height)
{
    mHeight = height;
    emit propretyChanged();

}

//------------------Tesselation---------------//
bool Property::getTesselationStatus() const
{
    return mTesselationStatus;
}

void Property::setTesselationStatus(bool status)
{
    mTesselationStatus = status;
    emit propretyChanged();
}

double Property::getTesselation() const
{
    return mTesselation;
}

void Property::setTesselation(double tesselation)
{
    mTesselation = tesselation;
    emit propretyChanged();

}

//------------------Clamp---------------//
bool Property::getClampStatus() const
{
    return mClampStatus;
}

void Property::setClampStatus(bool status)
{
    mClampStatus = status;
    emit propretyChanged();

}

int Property::getClamp() const
{
    return mClamp;
}

void Property::setClamp(int clamp)
{
    mClamp = clamp;
    emit propretyChanged();

}

//-----------------showlen-------------//

bool Property::getShowLenStatus() const
{
    return mShowLenStatus;
}

void Property::setShowLenStatus(bool status)
{
    mShowLenStatus = status;
    emit propretyChanged();


}

bool Property::getShowLen() const
{
    return mShowLen;
}

void Property::setShowLen(bool showLen)
{
    mShowLen = showLen;
    emit propretyChanged();

}
//------------------Bearing---------------//
bool Property::getBearingStatus() const
{
    return mBearingStatus;
}



void Property::setBearingStatus(bool status)
{
    mBearingStatus = status;
    emit propretyChanged();
}

bool Property::getShowBearing() const
{
    return mShowBearing;
}

void Property::setShowBearing(bool showBearing)
{
    mShowBearing = showBearing;
    emit propretyChanged();

}

//------------------Altitude---------------//
bool Property::getAltitudeStatus() const
{
    return mAltitudeStatus;
}



void Property::setAltitudeStatus(bool status)
{
    mAltitudeStatus = status;
    emit propretyChanged();
}

bool Property::getShowAltitude() const
{
    return mShowAltitude;
}

void Property::setShowAltitude(bool showAltitude)
{
    mShowAltitude = showAltitude;
    emit propretyChanged();

}

//-----------------showSlop-------------//

bool Property::getShowSlopStatus() const
{
    return mShowSlopStatus;
}

void Property::setShowSlopStatus(bool status)
{
    mShowSlopStatus = status;
    emit propretyChanged();


}

bool Property::getShowSlop() const
{
    return mShowSlop;
}

void Property::setShowSlop(bool showSlop)
{
    mShowSlop = showSlop;
    emit propretyChanged();
}


//------------------ points -------------------//

bool Property::getPointsStatus() const
{
    return mPointsStatus;
}

void Property::setPointsStatus(bool status)
{
    mPointsStatus = status;
    emit propretyChanged();

}

bool Property::getPointsVisible() const
{
    return mPointsVisible;
}

void Property::setPointsVisible(bool visible)
{
    mPointsVisible = visible;
    emit propretyChanged();
}

QColor Property::getPointsColor() const
{
    return mPointsColor;
}

void Property::setPointsColor(const QColor &pointColor)
{
    mPointsColor = pointColor;
    emit propretyChanged();

}

bool Property::getPointsSmooth() const
{
    return mPointsSmooth;
}

void Property::setPointsSmooth(bool point)
{
    mPointsSmooth = point;
    emit propretyChanged();

}


QQuickItem *Property::item() const
{
    return mItem;
}

double Property::getPointsWidth() const
{
    return mPointsWidth;
}

void Property::setPointsWidth(double pointWidth)
{
    mPointsWidth = pointWidth;
    emit propretyChanged();
}
