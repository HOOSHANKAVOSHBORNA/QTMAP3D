#include "coneProperties.h"
#include "mainwindow.h"
#include <QQmlComponent>
#include <QQuickItem>
#include "utility.h"




ConeProperties::ConeProperties(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus      (true);
    setRadiusStatus          (true);
    setHeightStatus         (true);
    setLocationStatus       (true);
}

void ConeProperties::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if(mCone)
        mCone->setColor(Utility::qColor2osgEarthColor(color));

}

void ConeProperties::setRadius(const double &radius)
{

    Property::setRadius(radius);
    if(mCone)
        mCone->setRadius(radius);
}


void ConeProperties::setHeight(const double &height)
{

    Property::setHeight(height);
    if(mCone)
        mCone->setHeight(height);
}

void ConeProperties::setLocationRelative(const bool &relative)
{

    Property::setLocationRelative(relative);
}
void ConeProperties::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mCone)
        mCone->setPosition(temp);




}



Cone *ConeProperties::getCone() const
{
    return mCone;
}

void ConeProperties::setCone(Cone *newCone, const osgEarth::SpatialReference *srs)
{
    mCone = newCone;
    mSRS = srs;

    if(mCone){
        mCone->setColor(Utility::qColor2osgEarthColor(getFillColor()));
        mCone->setRadius(getRadius());
        mCone->setHeight(getHeight());
        setLocation(Utility::osgEarthGeoPointToQvector3D(mCone->getPosition()));
    }
}
