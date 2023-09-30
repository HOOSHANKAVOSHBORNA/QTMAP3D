#ifndef SMALLMAP_H
#define SMALLMAP_H

#include <QObject>
#include <osg/Shape>
#include <osgEarthAnnotation/RectangleNode>>
#include "mapItem.h"

class SmallMap: public MapItem
{
    Q_OBJECT
public:
    SmallMap(QQuickItem *parent = nullptr);
    void initializeOsgEarth() override;
    void setLocation(const osgEarth::GeoPoint &geoPos);
private:
    osg::ref_ptr<osgEarth::Annotation::RectangleNode> mLocation;
};

#endif // SMALLMAP_H
