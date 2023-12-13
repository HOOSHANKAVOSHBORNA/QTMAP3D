#include "property.h"

PropertyItem::PropertyItem()
{
    
}

QString PropertyItem::name()
{
    return mName;
}

PropertyItem *PropertyItem::setName(const QString &newName)
{
    if (mName == newName)
        return this;
    mName = newName;
    emit propretyChanged();

    return this;
}

bool PropertyItem::isMovable() const
{
    return mIsMovable;
}

PropertyItem *PropertyItem::setIsMovable(bool newIsMovable)
{
    if (mIsMovable == newIsMovable)
        return this;
    mIsMovable = newIsMovable;
    emit propretyChanged();

    return this;
}

QColor PropertyItem::color() const
{
    return mColor;
}

PropertyItem *PropertyItem::setColor(const QColor &newColor)
{
    if (mColor == newColor)
        return this;
    mColor = newColor;
    emit propretyChanged();

    return this;
}

QVector3D PropertyItem::getLocation() const
{
    return mLocation;
}

PropertyItem *PropertyItem::setLocation(const QVector3D &newLocation)
{
    if (mLocation == newLocation)
        return this;
    mLocation = newLocation;
    emit propretyChanged();

    return this;
}

QVector3D PropertyItem::getMoveTo() const
{
    return mMoveTo;
}

PropertyItem *PropertyItem::setMoveTo(const QVector3D &newMoveTo)
{
    if (mMoveTo == newMoveTo)
        return this;
    mMoveTo = newMoveTo;
    emit propretyChanged();

    return this;
}

double PropertyItem::speed() const
{
    return mSpeed;
}

PropertyItem *PropertyItem::setSpeed(double newSpeed)
{
    if (qFuzzyCompare(mSpeed, newSpeed))
        return this;
    mSpeed = newSpeed;
    emit propretyChanged();

    return this;
}
