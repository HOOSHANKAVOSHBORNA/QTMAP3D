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
    return m_isMovable;
}

void PropertyItem::setIsMovable(bool newIsMovable)
{
    if (m_isMovable == newIsMovable)
        return;
    m_isMovable = newIsMovable;
    emit isMovableChanged();
}
