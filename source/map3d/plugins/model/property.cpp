#include "property.h"

PropertyItem::PropertyItem()
{
    
}

QString PropertyItem::name() const
{
    return mName;
}

void PropertyItem::setName(const QString &newName)
{
    if (mName == newName)
        return;
    mName = newName;
    emit nameChanged();
}

bool PropertyItem::isMovable() const
{
    return mIsMovable;
}

void PropertyItem::setIsMovable(bool newIsMovable)
{
    if (mIsMovable == newIsMovable)
        return;
    mIsMovable = newIsMovable;
    emit isMovableChanged();
}

QColor PropertyItem::color() const
{
    return mColor;
}

void PropertyItem::setColor(const QColor &newColor)
{
    if (mColor == newColor)
        return;
    mColor = newColor;
    emit colorChanged();
}

QVector3D PropertyItem::getLocation() const
{
    return mLocation;
}

void PropertyItem::setLocation(const QVector3D &newLocation)
{
    if (mLocation == newLocation)
        return;
    mLocation = newLocation;
    emit propretyChanged();
}
