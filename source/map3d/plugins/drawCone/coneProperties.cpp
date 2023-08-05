#include "coneProperties.h"
#include "mainwindow.h"
#include <QQmlComponent>
#include <QQuickItem>
#include "utility.h"




ConeProperties::ConeProperties(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus      (true);
    setRadiusStatus         (true);
    setHeightStatus         (true);
    setLocationStatus       (true);
    setCenterStatus         (true);
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



void ConeProperties::setCenter(const QVector3D &center)
{

    osg::Vec3 temp(center.x(),center.y(),center.z());

    if (mCone)
        mCone->setCenter(temp);
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


void ConeProperties::setName(const QString &name)
{

    Property::setName(name);
    if(mCone)
    {
        mCone->setName(name.toStdString());

    }
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
        setCenter(QVector3D(mCone->getCenter().x(),mCone->getCenter().y(),mCone->getCenter().z()));
        setLocation(Utility::osgEarthGeoPointToQvector3D(mCone->getPosition()));
        setName(QString::fromStdString(mCone->getName()));
    }
}
