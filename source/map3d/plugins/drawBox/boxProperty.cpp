#include "boxProperty.h"
#include "utility.h"
#include <QQmlComponent>
#include <QQuickItem>

BoxProperty::BoxProperty(QQuickItem *parent):
    Property(parent)
{
    setHeightStatus(true);
    setLocationStatus(true);
    //    setLocationRelative(true);
    setFillColorStatus(true);
    setWidthStatus(true);
    setLenghtStatus(true);
    setClampStatus(true);
    setCenterStatus(false);
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

//void BoxProperty::setClamp(const int &clamp)
//{
//    Property::setClamp(clamp);
//    if(mBox){
//        osgEarth::Symbology::AltitudeSymbol::Clamping clampEnum = static_cast<osgEarth::Symbology::AltitudeSymbol::Clamping>(clamp);
//        mBox->setClamp(clampEnum);

//    }
//}

void BoxProperty::setName(const QString &name)
{

    Property::setName(name);
    if(mBox)
    {
        mBox->setName(name.toStdString());

    }
}

//void BoxProperty::setName(const QString &name)
//{
//    Property::setName(name);
//    if(mBox){
//        mBox->setName(name.toStdString());


//    }
//}

//void BoxProperty::setCenter(const QVector3D &center)
//{
//    Property::setCenter(center);
//    osg::Vec3 temp;
//    temp = static_cast<osg::Vec3>(center);
////    temp.y() = dynamic_cast<osg::Vec3fValueObject>(center.y);
////    temp.z() = dynamic_cast<osg::Vec3fValueObject>(center.z);
////    osgEarth::GeoPoint temp = Utility::qVector3DToosgEarthGeoPoint(center, mSRS);
//    if (mBox)
//        mBox->setCenter(temp);
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
        setName(QString::fromStdString(mBox->getName()));
//        setCenter(QVector3D(mBox->getCenter());
    }
}
