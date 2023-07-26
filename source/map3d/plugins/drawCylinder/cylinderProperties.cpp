#include "cylinderProperties.h"
#include "utility.h"
#include <QQmlComponent>
#include <QQuickItem>


CylinderProperties::CylinderProperties(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus(true);
    setRadiusStatus(true);
    setLocationStatus(true);
    setHeightStatus(true);
}

void CylinderProperties::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if (mCylinder)
        mCylinder->setColor(Utility::qColor2osgEarthColor(color));
}



void CylinderProperties::setRadius(const double &radius)
{

    Property::setRadius(radius);
    if(mCylinder)
        mCylinder->setRadius(radius);
}


void CylinderProperties::setHeight(const double &height)
{

    Property::setHeight(height);
    if (mCylinder)
        mCylinder->setHeight(height);
}
void CylinderProperties::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mCylinder)
        mCylinder->setPosition(temp);

}
void CylinderProperties::setLocationRelative(const bool &relative)
{

    Property::setLocationRelative(relative);
}

Cylinder *CylinderProperties::getCylinder() const
{
    return mCylinder;
}

void CylinderProperties::setCylinder(Cylinder *newCylinder, const osgEarth::SpatialReference *srs)
{
    mCylinder = newCylinder;
    mSRS = srs;

    if(mCylinder){
        mCylinder->setColor(Utility::qColor2osgEarthColor(getFillColor()));
        mCylinder->setRadius(getRadius());
        mCylinder->setHeight(getHeight());
        setLocation(Utility::osgEarthGeoPointToQvector3D(mCylinder->getPosition()));
    }
}


