#include "circleproperties.h"
#include <QtDebug>
#include <QVector3D>
#include "utility.h"
#include <QQmlComponent>
#include <QQuickItem>




CircleProperties::CircleProperties(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setRadiusStatus     (true);
    setArcStatus        (true);
    setStrokeStatus     (true);
    setStrokeStatus     (true);
    setLocationStatus   (true);
    setHeightStatus     (true);
    setClampStatus      (true);
}

void CircleProperties::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if(mCircle)
        mCircle->setColor(Utility::qColor2osgEarthColor(color));
}

void CircleProperties::setRadius(const double &radius)
{

    Property::setRadius(radius);
    if(mCircle)
        mCircle->setRadius(radius);
}
void CircleProperties::setLocationRelative(const bool &relative)
{

    Property::setLocationRelative(relative);
}

void CircleProperties::setArc(const QVector2D &arc)
{

    Property::setArc(arc);
    if(mCircle){
        mCircle->setArcEnd(arc.y());
        mCircle->setArcStart(arc.x());
    }

}

void CircleProperties::setStrokeWidth(const double &opacity)
{

    Property::setStrokeWidth(opacity);
    if(mCircle)
        mCircle->setLineWidth(opacity);

    }

void CircleProperties::setStroke(const QColor &color)
{

    Property::setStroke(color);
    if(mCircle)
    {
        mCircle->setLineColor(Utility::qColor2osgEarthColor(color));
    }

}



void CircleProperties::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mCircle)
        mCircle->setPosition(temp);

}

void CircleProperties::setHeight(const double &height)
{

    Property::setHeight(height);
    if(mCircle)
        mCircle->setCircleHeight(height);
}

void CircleProperties::setClamp(const int &clamp)
{

    Property::setClamp(clamp);
//    if(mCircle){
//        switch(clamp){
//        case 0:
//            mCircle->setClamp(0);
//            break;
//        case 1:
//            mCircle->setClamp(1);
//            break;
//        case 2:
//            mCircle->setClamp(2);
//            break;
//        case 3:
//            mCircle->setClamp(3);
//            break;


//        }
//    }
}


Circle *CircleProperties::getCircle() const
{
    return mCircle;
}

void CircleProperties::setCircle(Circle *newCircle)
{
    mCircle = newCircle;


//    mSRS = srs;
    setLocation(Utility::osgEarthGeoPointToQvector3D(mCircle->getPosition()));
    if(mCircle){
        mCircle->setColor(Utility::qColor2osgEarthColor(getFillColor()));
        mCircle->setRadius(getRadius());
//        mCircle->setHeight(getHeight());
        setLocation(Utility::osgEarthGeoPointToQvector3D(mCircle->getPosition()));
    }
}





