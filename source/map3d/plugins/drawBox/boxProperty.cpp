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
    setLocationRelative(true);
    setFillColorStatus(true);
    setWidthStatus(true);
    setLenghtStatus(true);
}

void BoxProperty::setFillColor(const QColor &color)
{
    Property::setFillColor(color);
    mBox->setColor(Utility::qColor2osgEarthColor(color));
}

//void BoxProperties::setLenght(const QVector3D &len)
//{
//    Property::setLenght(len);
//}

//void BoxProperties::setWidth(const double &width)
//{
//    Property::setWidth(width);
//}

void BoxProperty::setLocationRelative(const bool &relative)
{
    Property::setLocationRelative(relative);
}



void BoxProperty::setLocation(const QVector3D &position)
{
    Property::setLocation(position);
    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(position);
    mBox->setPosition(temp);
}

void BoxProperty::setHeight(const double &height)
{
    Property::setHeight(height);
}

void BoxProperty::setStatuses()
{
//    setHeightStatus(true);
//    setLocationStatus(true);
//    setLocationRelative(true);
//    setFillColorStatus(true);
//    setWidthStatus(true);
//    setLenghtStatus(true);
}


Box *BoxProperty::getBox() const
{
    return mBox;
}

void BoxProperty::setBox(Box *newBox)
{
    mBox = newBox;
}
