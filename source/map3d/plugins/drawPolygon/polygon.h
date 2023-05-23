#ifndef POLYGON_H
#define POLYGON_H

#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"

class Polygon : public osgEarth::Annotation::FeatureNode
{
public:
    Polygon(MapController *mapController);
    double getSize();
    void setLineColor(osgEarth::Color color);
    osgEarth::Color getLineColor();
    void setFillColor(osgEarth::Color color);
    osgEarth::Color getFillColor();
    void setLineWidth(float width);
    float getLineWidth();
    void setClamp(osgEarth::Symbology::AltitudeSymbol::Clamping clamp);
    void addPoints(osgEarth::GeoPoint point);
    void clearPoints();
    void removePoint();
    void setHeight(float height);
    float getHeight();



private:
    MapController* mMapController{nullptr};
    osg::ref_ptr<osgEarth::Features::Geometry> mPolygonGeom;
};



#endif // POLYGON_H
