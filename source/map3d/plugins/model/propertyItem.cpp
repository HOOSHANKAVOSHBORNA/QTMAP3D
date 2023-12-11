#include "propertyItem.h"

propertyItem::propertyItem()
{
    
}

QString propertyItem::name() const
{
    return mName;
}

void propertyItem::setName(const QString &newName)
{
    if (mName == newName)
        return;
    mName = newName;
    emit nameChanged();
}
