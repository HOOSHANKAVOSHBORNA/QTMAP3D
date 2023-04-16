#ifndef POLYGON_H
#define POLYGON_H

#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"

class Polygon : public osgEarth::Annotation::FeatureNode
{
public:
    Polygon(MapController *mapController, bool clamp);
    double getSize();
    void setLineColor(osgEarth::Color color);
    void setFillColor(osgEarth::Color color);
    void setLineWidth(float width);
    void setClamp(bool clamp);
    void addPoints(osgEarth::GeoPoint point);
    void clearPoints();
    void removePoint();
    void setHeight(float height);
    osgEarth::Color getFillColor();
    osgEarth::Color getLineColor();
private:
    MapController* mMapController{nullptr};
    osg::ref_ptr<osgEarth::Features::Geometry> mPolygonGeom;
};



#endif // POLYGON_H
