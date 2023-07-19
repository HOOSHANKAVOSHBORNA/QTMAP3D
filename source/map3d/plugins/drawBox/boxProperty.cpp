#include "boxProperty.h"
#include "utility.h"
#include <QQmlComponent>
#include <QQuickItem>

BoxProperty::BoxProperty(QQuickItem *parent):
    Property(parent)
{
    qDebug()<<"BoxProperty construct";
    setHeightStatus(true);
    setLocationStatus(true);
    //    setLocationRelative(true);
    setFillColorStatus(true);
    setWidthStatus(true);
    setLenghtStatus(true);
    //    setFillColor(getFillColor());
    //    setLocation(getLocation());

}

void BoxProperty::setFillColor(const QColor &color)
{
    Property::setFillColor(color);
    if (mBox)
        mBox->setColor(Utility::qColor2osgEarthColor(color));
}

void BoxProperty::setLenght(const double &lenght)
{
    Property::setLenght(lenght);
    if (mBox)
        mBox->setLength(lenght);
}

void BoxProperty::setWidth(const double &width)
{
    Property::setWidth(width);
    if (mBox)
        mBox->setWidth(width);
}

void BoxProperty::setLocationRelative(const bool &relative)
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

void BoxProperty::setHeight(const double &height)
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
        mBox->setColor(Utility::qColor2osgEarthColor(getFillColor()));
        mBox->setLength(getLenght());
        mBox->setWidth(getWidth());
        mBox->setHeight(getHeight());
        setLocation(Utility::osgEarthGeoPointToQvector3D(mBox->getPosition()));
    }
}
