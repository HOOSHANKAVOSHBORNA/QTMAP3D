#include "property.h"
#include <QDebug>

Property::Property(QQuickItem *parent)
    : QQuickItem{parent}
{
//    qDebug()<<"Property construct";
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

void Property::setLocationRelative(bool relative)
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

void Property::setRadius(double radius)
{
    mRadius = radius;
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


//------------------Lenght---------------//
bool Property::getLenghtStatus() const
{
    return mLenghtStatus;
}

void Property::setLenghtStatus(bool status)
{
    mLenghtStatus = status;
    emit propretyChanged();

}

double Property::getLenght() const
{
    return mLenght;
}

void Property::setLenght(double lenght)
{
    mLenght = lenght;
    emit propretyChanged();

}



//------------------Width---------------//
bool Property::getWidthStatus() const
{
    return mWidthStatus;
}

void Property::setWidthStatus(bool status)
{
    mWidthStatus = status;
    emit propretyChanged();
}

double Property::getWidth() const
{
    return mWidth;
}

void Property::setWidth(double width)
{
    mWidth = width;
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

void Property::createProperty(QString name, QVariant property, QQmlEngine *qmlEngine)
{
    QQmlComponent* comp = new QQmlComponent(qmlEngine);
    connect(comp, &QQmlComponent::statusChanged, [comp, property, name, this](){
        if (comp->status() == QQmlComponent::Status::Error) {
            qDebug() << comp->errorString();
        }
        //            QQmlContext *context = new QQmlContext(qmlEngine(), this);
        mItem = qobject_cast<QQuickItem*>(comp->create());
        mItem->setProperty("model", property);

    });


    comp->loadUrl(QUrl("qrc:/Properties.qml"));
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
