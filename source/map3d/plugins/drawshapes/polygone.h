#ifndef POLYGONE_H
#define POLYGONE_H
#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"

class Polygone : public osgEarth::Annotation::FeatureNode
{
public:
    Polygone(MapController *mapController, bool clamp);
    void setLineColor(osgEarth::Color color);
    void setFillColor(osgEarth::Color color);
    void setLineWidth(float width);
    void setClamp(bool clamp);
    void addPoints(osg::Vec3d point);
    void clearPoints();
    void setHeight(float height);
    float printHeight();
    unsigned long getSize()const;
private:
    MapController* mMapController{nullptr};
    osg::ref_ptr<osgEarth::Features::Geometry> mPolygonGeom;
};


#endif // POLYGONE_H
