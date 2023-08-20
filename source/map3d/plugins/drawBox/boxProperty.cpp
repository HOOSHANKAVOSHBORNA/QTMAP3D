#include "boxProperty.h"
#include "utility.h"
#include <QQmlComponent>
#include <QQuickItem>

BoxProperty::BoxProperty(QQuickItem *parent):
    Property(parent)
{
    setHeightStatus(true);
    setLocationStatus(true);
    setFillColorStatus(true);
    setWidthStatus(true);
    setLenghtStatus(true);
    setCenterStatus(true);
}

void BoxProperty::setFillColor(const QColor &color)
{
    Property::setFillColor(color);
    if (mBox)
        mBox->setFillColor(Utility::qColor2osgEarthColor(color));
}

void BoxProperty::setLenght(double lenght)
{
    Property::setLenght(lenght);
    if (mBox)
        mBox->setLength(lenght);
}

void BoxProperty::setWidth(double width)
{
    Property::setWidth(width);
    if (mBox)
        mBox->setWidth(width);
}


void BoxProperty::setName(const QString &name)
{

    Property::setName(name);
    if(mBox)
    {
        mBox->setName(name.toStdString());

    }
}



void BoxProperty::setCenter(const QVector3D &center)
{

    osg::Vec3 temp(center.x(),center.y(),center.z());

    if (mBox)
        mBox->setCenter(temp);
}


void BoxProperty::setLocationRelative(bool relative)
{
    Property::setLocationRelative(relative);
}



void BoxProperty::setLocation(const QVector3D &position)
{
    Property::setLocation(position);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(position, mSRS);
    if (mBox)
        mBox->setPosition(temp);
}

void BoxProperty::setHeight(double height)
{
    Property::setHeight(height);
    if (mBox)
        mBox->setHeight(height);
}

Box *BoxProperty::getBox() const
{
    return mBox;
}

void BoxProperty::setBox(Box *newBox, const osgEarth::SpatialReference* srs)
{
    mBox = newBox;
    mSRS = srs;
    if(mBox){
        mBox->setFillColor(Utility::qColor2osgEarthColor(getFillColor()));
        mBox->setLength(getLenght());
        mBox->setWidth(getWidth());
        mBox->setHeight(getHeight());

        setLocation(Utility::osgEarthGeoPointToQvector3D(mBox->getPosition()));
        setName(QString::fromStdString(mBox->getName()));
        setCenter(QVector3D(mBox->getCenter().x(),mBox->getCenter().y(),mBox->getCenter().z()));
    }
}
