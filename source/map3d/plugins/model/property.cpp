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

QVector3D PropertyItem::getMoveTo() const
{
    return mMoveTo;
}

void PropertyItem::setMoveTo(const QVector3D &newMoveTo)
{
    if (mMoveTo == newMoveTo)
        return;
    mMoveTo = newMoveTo;
    emit propretyChanged();
}

double PropertyItem::speed() const
{
    return mSpeed;
}

void PropertyItem::setSpeed(double newSpeed)
{
    if (qFuzzyCompare(mSpeed, newSpeed))
        return;
    mSpeed = newSpeed;
    emit speedChanged();
}
