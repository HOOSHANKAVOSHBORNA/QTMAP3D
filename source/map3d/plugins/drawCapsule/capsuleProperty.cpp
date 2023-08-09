#include "capsuleProperty.h"

#include <QQmlComponent>
#include <QQuickItem>
#include "utility.h"


CapsuleProperty::CapsuleProperty(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setRadiusStatus     (true);
    setLocationStatus   (true);
    setHeightStatus     (true);
    setCenterStatus     (true);

}

void CapsuleProperty::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if (mCapsule)
        mCapsule->setColor(Utility::qColor2osgEarthColor(color));
}

void CapsuleProperty::setCenter(const QVector3D &center)
{

    osg::Vec3 temp(center.x(),center.y(),center.z());

    if (mCapsule)
        mCapsule->setCenter(temp);
}

void CapsuleProperty::setRadius(double radius)
{

    Property::setRadius(radius);
    if(mCapsule)
        mCapsule->setRadius(radius);
}
void CapsuleProperty::setLocationRelative(bool relative)
{

    Property::setLocationRelative(relative);
}
void CapsuleProperty::setName(const QString &name)
{

    Property::setName(name);
    if(mCapsule)
    {
        mCapsule->setName(name.toStdString());

    }
}


void CapsuleProperty::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mCapsule)
        mCapsule->setPosition(temp);


}

void CapsuleProperty::setHeight(double height)
{

    Property::setHeight(height);
    if (mCapsule)
        mCapsule->setHeight(height);
}


Capsule *CapsuleProperty::getCapsule() const
{
    return mCapsule;
}

void CapsuleProperty::setCapsule(Capsule *newCapsule, const osgEarth::SpatialReference *srs)
{
    mCapsule = newCapsule;
    mSRS = srs;

    if(mCapsule){
        mCapsule->setColor(Utility::qColor2osgEarthColor(getFillColor()));
        mCapsule->setRadius(getRadius());
        mCapsule->setHeight(getHeight());
        setName(QString::fromStdString(mCapsule->getName()));
        setCenter(QVector3D(mCapsule->getCenter().x(),mCapsule->getCenter().y(),mCapsule->getCenter().z()));
        setLocation(Utility::osgEarthGeoPointToQvector3D(mCapsule->getPosition()));
    }
}

