#include "cylinderProperty.h"
#include "utility.h"
#include <QQmlComponent>
#include <QQuickItem>


CylinderProperty::CylinderProperty(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus  (true);
    setRadiusStatus     (true);
    setLocationStatus   (true);
    setHeightStatus     (true);
    setCenterStatus     (true);
}

void CylinderProperty::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
    if (mCylinder)
        mCylinder->setFillColor(Utility::qColor2osgEarthColor(color));
}

void CylinderProperty::setRadius(double radius)
{

    Property::setRadius(radius);
    if(mCylinder)
        mCylinder->setRadius(radius);
}

void CylinderProperty::setHeight(double height)
{

    Property::setHeight(height);
    if (mCylinder)
        mCylinder->setHeight(height);
}

void CylinderProperty::setLocation(const QVector3D &status)
{

    Property::setLocation(status);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(status, mSRS);
    if (mCylinder)
        mCylinder->setPosition(temp);

}

void CylinderProperty::setLocationRelative(bool relative)
{

    Property::setLocationRelative(relative);
}

void CylinderProperty::setName(const QString &name)
{

    Property::setName(name);
    if(mCylinder)
    {
        mCylinder->setName(name.toStdString());

    }
}


void CylinderProperty::setCenter(const QVector3D &center)
{
    Property::setCenter(center);
    osg::Vec3 temp(center.x(),center.y(),center.z());

    if (mCylinder)
        mCylinder->setCenter(temp);
}
Cylinder *CylinderProperty::getCylinder() const
{
    return mCylinder;
}

void CylinderProperty::setCylinder(Cylinder *newCylinder, const osgEarth::SpatialReference *srs)
{
    mCylinder = newCylinder;
    mSRS = srs;

    if(mCylinder){
        mCylinder->setFillColor(Utility::qColor2osgEarthColor(getFillColor()));
        mCylinder->setRadius(getRadius());
        mCylinder->setHeight(getHeight());
        setLocation(Utility::osgEarthGeoPointToQvector3D(mCylinder->getPosition()));
        setCenter(QVector3D(mCylinder->getCenter().x(),mCylinder->getCenter().y(),mCylinder->getCenter().z()));
        setName(QString::fromStdString(mCylinder->getName()));
    }
}


