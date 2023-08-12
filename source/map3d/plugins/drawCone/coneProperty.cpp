#include "coneProperty.h"
#include "mainwindow.h"
#include <QQmlComponent>
#include <QQuickItem>
#include "utility.h"




ConeProperty::ConeProperty(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus      (true);
    setRadiusStatus         (true);
    setHeightStatus         (true);
    setLocationStatus       (true);
    setCenterStatus         (true);
}

void ConeProperty::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if(mCone)
        mCone->setColor(Utility::qColor2osgEarthColor(color));

}

void ConeProperty::setRadius(double radius)
{

    Property::setRadius(radius);
    if(mCone)
        mCone->setRadius(radius);
}


void ConeProperty::setHeight(double height)
{

    Property::setHeight(height);
    if(mCone)
        mCone->setHeight(height);
}



void ConeProperty::setCenter(const QVector3D &center)
{

    osg::Vec3 temp(center.x(),center.y(),center.z());

    if (mCone)
        mCone->setCenter(temp);
}

void ConeProperty::setLocationRelative(bool relative)
{

    Property::setLocationRelative(relative);
}
void ConeProperty::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mCone)
        mCone->setPosition(temp);
}


void ConeProperty::setName(const QString &name)
{

    Property::setName(name);
    if(mCone)
    {
        mCone->setName(name.toStdString());

    }
}

Cone *ConeProperty::getCone() const
{
    return mCone;
}

void ConeProperty::setCone(Cone *newCone, const osgEarth::SpatialReference *srs)
{
    mCone = newCone;
    mSRS = srs;

    if(mCone){
        mCone->setColor(Utility::qColor2osgEarthColor(getFillColor()));
        mCone->setRadius(getRadius());
        mCone->setHeight(getHeight());
        setCenter(QVector3D(mCone->getCenter().x(),mCone->getCenter().y(),mCone->getCenter().z()));
        setLocation(Utility::osgEarthGeoPointToQvector3D(mCone->getPosition()));
        setName(QString::fromStdString(mCone->getName()));
    }
}
