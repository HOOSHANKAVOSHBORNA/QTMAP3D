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
    setClampStatus(true);
    setCenterStatus(true);
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

void BoxProperty::setClamp(const int &clamp)
{
    if(mBox){
    switch(clamp){
    case 0:
        mBox->setClamp(0);
        break;
    case 1:
        mBox->setClamp(1);
        break;
    case 2:
        mBox->setClamp(2);
        break;
    case 3:
        mBox->setClamp(3);


    }
    }
}

//void BoxProperty::setCenter(const QVector3D &center)
//{
//    Property::setCenter(center);
//    if(mBox)
//        mBox->setCenter(center);
//}


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
