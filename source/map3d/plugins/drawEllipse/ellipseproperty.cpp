#include "ellipseproperty.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>

#include "utility.h"



EllipseProperty::EllipseProperty(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setStrokeStatus     (true);
    setLocationStatus   (true);
    setHeightStatus     (true);
    setClampStatus      (true);
}

void EllipseProperty::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if(mEllipse)
        mEllipse->setColor(Utility::qColor2osgEarthColor(color));
}


void EllipseProperty::setLocationRelative(bool relative)
{

    Property::setLocationRelative(relative);
}



void EllipseProperty::setStrokeWidth(const double &opacity)
{

    Property::setStrokeWidth(opacity);
    if(mEllipse)
        mEllipse->setStrokeWidth(opacity);

}

void EllipseProperty::setStrokeColor(const QColor &color)
{

    Property::setStrokeColor(color);
    if(mEllipse)
    {
        mEllipse->setStrokeColor(Utility::qColor2osgEarthColor(color));
    }

}
void EllipseProperty::setName(const QString &name)
{

    Property::setName(name);
    if(mEllipse)
    {
        mEllipse->setName(name.toStdString());

    }
}



void EllipseProperty::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mEllipse)
        mEllipse->setPosition(temp);

}

void EllipseProperty::setHeight(double height)
{

    Property::setHeight(height);
    if(mEllipse)
        mEllipse->setHeight(height);
}

void EllipseProperty::setClamp(int clamp)
{

    Property::setClamp(clamp);
    if(mEllipse){
        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(clamp);
        mEllipse->setClamp(clampEnum);

    }
}


Ellipse *EllipseProperty::getEllipse() const
{
    return mEllipse;
}

void EllipseProperty::setEllipse(Ellipse *newEllipse, const osgEarth::SpatialReference *srs)
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
