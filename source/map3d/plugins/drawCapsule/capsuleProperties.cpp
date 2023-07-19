#include "capsuleProperties.h"

#include <QQmlComponent>
#include <QQuickItem>
#include "utility.h"


CapsuleProperties::CapsuleProperties(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus(true);
    setRadiusStatus(true);
    setLocationStatus(true);
    setHeightStatus(true);
//    setCenterStatus(true);

}

void CapsuleProperties::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if (mCapsule)
        mCapsule->setColor(Utility::qColor2osgEarthColor(color));
}

//void CapsuleProperties::setCenter(const QVector3D &center)
//{
//    Property::setLocation(center);
//    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(position, mSRS);
//    if (mCapsule)
//        mCapsule->setPosition(temp);
//}


void CapsuleProperties::setRadius(const double &radius)
{

    Property::setRadius(radius);
    if(mCapsule)
        mCapsule->setRadius(radius);
}
void CapsuleProperties::setLocationRelative(const bool &relative)
{

    Property::setLocationRelative(relative);
}



void CapsuleProperties::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mCapsule)
        mCapsule->setPosition(temp);


}

void CapsuleProperties::setHeight(const double &height)
{

    Property::setHeight(height);
    if (mCapsule)
        mCapsule->setHeight(height);
}


Capsule *CapsuleProperties::getCapsule() const
{
    return mCapsule;
}

void CapsuleProperties::setCapsule(Capsule *newCapsule, const osgEarth::SpatialReference *srs)
{
    mCapsule = newCapsule;

    setLocation(Utility::osgEarthGeoPointToQvector3D(mCapsule->getPosition()));
}

