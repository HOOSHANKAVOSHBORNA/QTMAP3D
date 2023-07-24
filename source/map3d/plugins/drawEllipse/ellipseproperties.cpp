#include "ellipseproperties.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>

#include "utility.h"



EllipseProperties::EllipseProperties(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setStrokeStatus     (true);
    setLocationStatus   (true);
    setHeightStatus     (true);
    setClampStatus      (true);
}

void EllipseProperties::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if(mEllipse)
        mEllipse->setColor(Utility::qColor2osgEarthColor(color));
}


void EllipseProperties::setLocationRelative(const bool &relative)
{

    Property::setLocationRelative(relative);
}



void EllipseProperties::setStrokeWidth(const double &opacity)
{

    Property::setStrokeWidth(opacity);
    if(mEllipse)
        mEllipse->setStrokeWidth(opacity);

}

void EllipseProperties::setStroke(const QColor &color)
{

    Property::setStroke(color);
    if(mEllipse)
    {
        mEllipse->setStrokeColor(Utility::qColor2osgEarthColor(color));
    }

}



void EllipseProperties::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mEllipse)
        mEllipse->setPosition(temp);

}

void EllipseProperties::setHeight(const double &height)
{

    Property::setHeight(height);
    if(mEllipse)
        mEllipse->setHeight(height);
}

void EllipseProperties::setClamp(const int &clamp)
{

    Property::setClamp(clamp);
    if(mEllipse){
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(clamp);
        mEllipse->setClamp(clampEnum);

    }
}


Ellipse *EllipseProperties::getEllipse() const
{
    return mEllipse;
}

void EllipseProperties::setEllipse(Ellipse *newEllipse, const osgEarth::SpatialReference *srs)
{
    mEllipse = newEllipse;


    mSRS = srs;

    if(mEllipse){
        mEllipse->setColor(Utility::qColor2osgEarthColor(getFillColor()));

        mEllipse->setHeight(getHeight());
        mEllipse->setStrokeColor(Utility::qColor2osgEarthColor(getStroke()));
        mEllipse->setStrokeWidth(getStrokeWidth());
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(getClamp());
        mEllipse->setClamp(clampEnum);
        setLocation(Utility::osgEarthGeoPointToQvector3D(mEllipse->getPosition()));
    }
}
