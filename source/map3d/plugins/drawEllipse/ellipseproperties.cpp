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


void EllipseProperties::setLocationRelative(bool relative)
{

    Property::setLocationRelative(relative);
}



void EllipseProperties::setStrokeWidth(const double &opacity)
{

    Property::setStrokeWidth(opacity);
    if(mEllipse)
        mEllipse->setStrokeWidth(opacity);

}

void EllipseProperties::setStrokeColor(const QColor &color)
{

    Property::setStrokeColor(color);
    if(mEllipse)
    {
        mEllipse->setStrokeColor(Utility::qColor2osgEarthColor(color));
    }

}
void EllipseProperties::setName(const QString &name)
{

    Property::setName(name);
    if(mEllipse)
    {
        mEllipse->setName(name.toStdString());

    }
}



void EllipseProperties::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mEllipse)
        mEllipse->setPosition(temp);

}

void EllipseProperties::setHeight(double height)
{

    Property::setHeight(height);
    if(mEllipse)
        mEllipse->setHeight(height);
}

void EllipseProperties::setClamp(int clamp)
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
        mEllipse->setStrokeColor(Utility::qColor2osgEarthColor(getStrokeColor()));
        mEllipse->setStrokeWidth(getStrokeWidth());
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(getClamp());
        mEllipse->setClamp(clampEnum);
        setName(QString::fromStdString(mEllipse->getName()));
        setLocation(Utility::osgEarthGeoPointToQvector3D(mEllipse->getPosition()));
    }
}
