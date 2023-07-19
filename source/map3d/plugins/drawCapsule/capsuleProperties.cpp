#include "capsuleProperties.h"

#include <QQmlComponent>
#include <QQuickItem>


CapsuleProperties::CapsuleProperties(QQuickItem *parent):
    Property(parent)
{
    setFillColorStatus(true);
    setRadiusStatus(true);
    setLocationRelative(true);
    setLocationStatus(true);
    setHeightStatus(true);
}

void CapsuleProperties::setFillColor(const QColor &color)
{

    Property::setFillColor(color);
//    mCapsule->setColor(color);
}

void CapsuleProperties::setRadius(const double &radius)
{

    Property::setRadius(radius);
}
void CapsuleProperties::setLocationRelative(const bool &relative)
{

    Property::setLocationRelative(relative);
}



void CapsuleProperties::setLocation(const QVector3D &status)
{

    Property::setLocation(status);


}

void CapsuleProperties::setHeight(const double &height)
{

    Property::setHeight(height);
}


Capsule *CapsuleProperties::getCapsule() const
{
    return mCapsule;
}

void CapsuleProperties::setCapsule(Capsule *newCapsule)
{
    mCapsule = newCapsule;
}

