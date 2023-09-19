#ifndef SMALLMAP_H
#define SMALLMAP_H

#include <QObject>
#include "mapItem.h"

class SmallMap: public MapItem
{
    Q_OBJECT
public:
    SmallMap(QQuickItem *parent = nullptr);
    void initializeOsgEarth() override;
};

#endif // SMALLMAP_H
