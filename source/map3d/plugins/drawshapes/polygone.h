#ifndef POLYGONE_H
#define POLYGONE_H
#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"

class Polygone : public osgEarth::Annotation::FeatureNode
{
public:
    Polygone(MapController *mapController, bool clamp);
    void setColor(osgEarth::Color color);
    void setWidth(float width);
    void setClamp(bool clamp);
    void addPoints(osg::Vec3d point);
    osgEarth::Features::Geometry* geom;
private:
    MapController* mMapController{nullptr};
};


#endif // POLYGONE_H
