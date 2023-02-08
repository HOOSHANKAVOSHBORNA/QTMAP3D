#include "sphereproperties.h"
#include <QtDebug>
#include <QVector3D>

SphereProperties::SphereProperties(QObject *parent) : QObject(parent)
{

}

QString SphereProperties::color() const
{
    return mColor;
}
void SphereProperties:: setColor(const QString &value){
    if(value == mColor)
        return;
    mColor = value;
    emit spherePropertiesChanged(Color , value);
//    qDebug() << Color;
}

QVector3D SphereProperties::location() const
{
    return mLocation;
}
void SphereProperties:: setLocation(const QVector3D &value){
    if(value == mLocation)
        return;
    mLocation = value;
    //emit spherePropertiesChanged(Location, QVariant::fromValue<QVector3D>(value));
    emit spherePropertiesChanged(Location , value);
}

QVector3D SphereProperties::center() const
{
    return mCenter;
}
void SphereProperties:: setCenter(const QVector3D &value){
    if(value == mCenter)
        return;
    mCenter = value;
//    emit spherePropertiesChanged(Center, QVariant::fromValue<QVector3D>(value));
    emit spherePropertiesChanged(Center , value);
}

double SphereProperties::radius() const
{
    return mRadius;
}
void SphereProperties::setRadius(const double &value){
    if(value == mRadius)
        return;
    mRadius = value;
    emit spherePropertiesChanged(Radius, value);
}

QString SphereProperties::transparency() const
{
    return mTransparency;
}
void SphereProperties::setTransparency(const QString &value){
    if(value == mTransparency)
        return;
    mTransparency = value;
    emit spherePropertiesChanged(Transparency, value);
}

int SphereProperties::shape() const
{
    return mShape;
}
void SphereProperties::setShape(const int &value){
    if(value == mShape)
        return;
    mShape = value;
    emit spherePropertiesChanged(Shape, value);
}

bool SphereProperties::relative() const
{
    return mRelative;
}
void SphereProperties::setRelative(const bool &value){
    if(value == mRelative)
        return;
    mRelative = value;
    emit spherePropertiesChanged(Relative, value);
}
