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
    return m_color;
}

void PropertyItem::setColor(const QColor &newColor)
{
    if (m_color == newColor)
        return;
    m_color = newColor;
    emit colorChanged();
}
