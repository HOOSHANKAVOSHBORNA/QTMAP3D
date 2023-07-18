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
    qDebug() <<color;
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
    qDebug() << opacity;
}



QColor Property::getStroke() const
{
    return mStroke;
}

void Property::setStroke(const QColor &strok)
{
    mStroke = strok;
    emit propretyChanged();
    qDebug() <<strok;
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
    qDebug () << status;
}

QVector3D Property::getLocation() const
{
    return mLocation;
}

void Property::setLocation(const QVector3D &location)
{
    mLocation = location;
    emit propretyChanged();
    qDebug() << location;
}

bool Property::getLocationRelative() const
{
    return mRelative;
}

void Property::setLocationRelative(const bool &relative)
{
    mRelative = relative;
    emit propretyChanged();
    qDebug() << relative;
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
    qDebug() << center;
}

QVector3D Property::getCenter() const
{
    return mCenter;
}

void Property::setCenter(const QVector3D &center)
{
    mCenter = center;
    emit propretyChanged();
    qDebug() << center;
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
    qDebug () << arc ;
}

QVector2D Property::getArc() const
{
    return mArc;
}

void Property::setArc(const QVector2D &arc)
{
    mArc=arc;
    emit propretyChanged();
    qDebug() << arc  ;
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
    qDebug()<<radius;
}

double Property::getRadius() const
{
    return mRadius;
}

void Property::setRadius(const double &radius)
{
    mRadius = radius;
    emit propretyChanged();
    qDebug()<<radius;
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
    qDebug()<< height;
}

double Property::getHeight() const
{
    return mHeight;
}

void Property::setHeight(const double &height)
{
    mHeight = height;
    emit propretyChanged();
    qDebug()<<height;
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
    qDebug()<< lenght;
}

double Property::getLenght() const
{
    return mLenght;
}

void Property::setLenght(const double &lenght)
{
    mLenght = lenght;
    emit propretyChanged();
    qDebug()<<lenght;
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
    qDebug()<< width;
}

double Property::getWidth() const
{
    return mWidth;
}

void Property::setWidth(double width)
{
    mWidth = width;
    emit propretyChanged();
    qDebug()<< width;
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
    qDebug()<< tesselation;
}

double Property::getTesselation() const
{
    return mTesselation;
}

void Property::setTesselation(const double &tesselation)
{
    mTesselation = tesselation;
    emit propretyChanged();
    qDebug()<<tesselation;
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
    qDebug()<< clamp;
}

double Property::getClamp() const
{
    return mClamp;
}

void Property::setClamp(const double &clamp)
{
    mClamp = clamp;
    emit propretyChanged();
    qDebug()<<clamp;
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
    qDebug() << len;

}

bool Property::getShowLen() const
{
    return mShowLen;
}

void Property::setShowLen(const bool &len)
{
    mShowLen = len;
    emit propretyChanged();
    qDebug () << len;
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
    qDebug()<< bearin;
}

bool Property::getBearing() const
{
    return mBearing;
}

void Property::setBearing(const bool &bearing)
{
    mBearing = bearing;
    emit propretyChanged();
    qDebug()<<bearing;
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
    qDebug() << slop;

}

bool Property::getShowSlop() const
{
    return mShowSlop;
}

void Property::setShowSlop(const bool &slop)
{
    mShowSlop = slop;
    emit propretyChanged();
    qDebug () << slop;
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
    qDebug() << point;
}

QColor Property::getPoints() const
{
    return mPoints;
}

void Property::setPoints(const QColor &point)
{
    mPoints = point;
    emit propretyChanged();
    qDebug() << point;
}

bool Property::getPointsSmooth() const
{
    return mPointsSmooth;
}

void Property::setPointsSmooth(const bool &point)
{
    mPointsSmooth = point;
    emit propretyChanged();
    qDebug() << point;
}

double Property::getPointsWidth() const
{
    return mPointsWidth;
}

void Property::setPointsWidth(const double &point)
{
    mPointsWidth = point;
    emit propretyChanged();
    qDebug() << point;
}
