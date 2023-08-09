#include "sphereProperty.h"
#include <QtDebug>
#include <QVector3D>
#include <QQmlComponent>
#include <QQuickItem>
#include "utility.h"


SphereProperty::SphereProperty(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setRadiusStatus     (true);
    setLocationStatus   (true);

    //    setCenterStatus     (true);

}

void SphereProperty::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if (mSphere)
        mSphere->setColor(Utility::qColor2osgEarthColor(color));
}

//void SphereProperty::setCenter(const QVector3D &center)
//{
//    Property::setCenter(center);
//    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(center, mSRS);
//    if (mSphere)
//        mSphere->setPosition(temp);
//}


void SphereProperty::setRadius(double radius)
{

    Property::setRadius(radius);
    if(mSphere)
        mSphere->setRadius(radius);
}
void SphereProperty::setLocationRelative(bool relative)
{

    Property::setLocationRelative(relative);
}



void SphereProperty::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mSphere)
        mSphere->setPosition(temp);


}

SphereNode *SphereProperty::getSphere() const
{
    return mSphere;
}

void SphereProperty::setSphere(SphereNode *newSphere, const osgEarth::SpatialReference *srs)
{
    mSphere = newSphere;
    mSRS = srs;

    if(mSphere){
        mSphere->setColor(Utility::qColor2osgEarthColor(getFillColor()));
        mSphere->setRadius(getRadius());
//        mSphere->setHeight(getHeight());

        setLocation(Utility::osgEarthGeoPointToQvector3D(mSphere->getPosition()));
    }
}
