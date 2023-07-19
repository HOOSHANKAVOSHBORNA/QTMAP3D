#include "property.h"

Property::Property(QQuickItem *parent)
    : QQuickItem{parent}
{
    qDebug()<<"Property construct";
//    QColor color1(0,0, 150, 128 );
//    QColor color2(51,102, 164, 128 );
//    QVector3D location1(85.0,85.0,85.0);
//    setFillColor(color1);
//    setStroke(color2);
//    setLocation(location1);
//    setPoints(color1);

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

void Property::setStrokeWidth(const double &opacity)
{
    mStrokeWidth = opacity;
    emit propretyChanged();

}



QColor Property::getStroke() const
{
    return mStroke;
}

void Property::setStroke(const QColor &strok)
{
    mStroke = strok;
    emit propretyChanged();

}
//------------------- location --------------------//

bool Property::getLocationStatus() const
{
    return mLocationStatus;
}

void Property::setLocationStatus(bool status)
{
    mLocationStatus = status;
    emit propretyChanged();

}

QVector3D Property::getLocation() const
{
    return mLocation;
}

void Property::setLocation(const QVector3D &location)
{
    mLocation = location;
    emit propretyChanged();

}

bool Property::getLocationRelative() const
{
    return mRelative;
}

void Property::setLocationRelative(const bool &relative)
{
    mRelative = relative;
    emit propretyChanged();

}

//------------------ center -------------------//

bool Property::getCenterStatus() const
{
    return mCenterStatus;
}

void Property::setCenterStatus(bool center)
{
    mCenterStatus = center;
    emit propretyChanged();

}

QVector3D Property::getCenter() const
{
    return mCenter;
}

void Property::setCenter(const QVector3D &center)
{
    mCenter = center;
    emit propretyChanged();
}
//-------------------Arc ---------------//
bool Property::getArcStatus() const
{
    return mArcStatus;
}

void Property::setArcStatus(bool arc)
{
    mArcStatus = arc;
    emit propretyChanged();

}

QVector2D Property::getArc() const
{
    return mArc;
}

void Property::setArc(const QVector2D &arc)
{
    mArc=arc;
    emit propretyChanged();

}

//------------------radius ---------------//

bool Property::getRadiusStatus() const
{
    return mRadiusStatus;
}

void Property::setRadiusStatus(bool radius)
{
    mRadiusStatus = radius;
    emit propretyChanged();

}

double Property::getRadius() const
{
    return mRadius;
}

void Property::setRadius(const double &radius)
{
    mRadius = radius;
    emit propretyChanged();

}
//------------------height---------------//
bool Property::getHeightStatus() const
{
    return mHeightStatus;
}

void Property::setHeightStatus( bool height)
{
    mHeightStatus = height;
    emit propretyChanged();

}

double Property::getHeight() const
{
    return mHeight;
}

void Property::setHeight(const double &height)
{
    mHeight = height;
    emit propretyChanged();

}


//------------------Lenght---------------//
bool Property::getLenghtStatus() const
{
    return mLenghtStatus;
}

void Property::setLenghtStatus(bool lenght)
{
    mLenghtStatus = lenght;
    emit propretyChanged();

}

double Property::getLenght() const
{
    return mLenght;
}

void Property::setLenght(const double &lenght)
{
    mLenght = lenght;
    emit propretyChanged();

}



//------------------Width---------------//
bool Property::getWidthStatus() const
{
    return mWidthStatus;
}

void Property::setWidthStatus(bool width)
{
    mWidthStatus = width;
    emit propretyChanged();
}

double Property::getWidth() const
{
    return mWidth;
}

void Property::setWidth(const double &width)
{
    mWidth = width;
    emit propretyChanged();
}

//------------------Tesselation---------------//
bool Property::getTesselationStatus() const
{
    return mTesselationStatus;
}

void Property::setTesselationStatus(bool tesselation)
{
    mTesselationStatus = tesselation;
    emit propretyChanged();
}

double Property::getTesselation() const
{
    return mTesselation;
}

void Property::setTesselation(const double &tesselation)
{
    mTesselation = tesselation;
    emit propretyChanged();

}

//------------------Clamp---------------//
bool Property::getClampStatus() const
{
    return mClampStatus;
}

void Property::setClampStatus(bool clamp)
{
    mClampStatus = clamp;
    emit propretyChanged();

}

double Property::getClamp() const
{
    return mClamp;
}

void Property::setClamp(const double &clamp)
{
    mClamp = clamp;
    emit propretyChanged();

}

//-----------------showlen-------------//

bool Property::getShowLenStatus() const
{
    return mShowLenStatus;
}

void Property::setShowLenStatus(bool len)
{
    mShowLenStatus = len;
    emit propretyChanged();


}

bool Property::getShowLen() const
{
    return mShowLen;
}

void Property::setShowLen(const bool &len)
{
    mShowLen = len;
    emit propretyChanged();

}
//------------------Bearing---------------//
bool Property::getBearingStatus() const
{
    return mBearingStatus;
}



void Property::setBearingStatus(bool bearin)
{
    mBearingStatus = bearin;
    emit propretyChanged();
}

bool Property::getBearing() const
{
    return mBearing;
}

void Property::setBearing(const bool &bearing)
{
    mBearing = bearing;
    emit propretyChanged();

}

//-----------------showSlop-------------//

bool Property::getShowSlopStatus() const
{
    return mShowSlopStatus;
}

void Property::setShowSlopStatus(bool slop)
{
    mShowSlopStatus = slop;
    emit propretyChanged();


}

bool Property::getShowSlop() const
{
    return mShowSlop;
}

void Property::setShowSlop(const bool &slop)
{
    mShowSlop = slop;
    emit propretyChanged();
}


//------------------ points -------------------//

bool Property::getPointsStatus() const
{
    return mPointsStatus;
}

void Property::setPointsStatus(bool point)
{
    mPointsStatus = point;
    emit propretyChanged();

}

QColor Property::getPoints() const
{
    return mPoints;
}

void Property::setPoints(const QColor &point)
{
    mPoints = point;
    emit propretyChanged();

}

bool Property::getPointsSmooth() const
{
    return mPointsSmooth;
}

void Property::setPointsSmooth(const bool &point)
{
    mPointsSmooth = point;
    emit propretyChanged();

}

double Property::getPointsWidth() const
{
    return mPointsWidth;
}

void Property::setPointsWidth(const double &point)
{
    mPointsWidth = point;
    emit propretyChanged();

}
