#ifndef LINENODE_H
#define LINENODE_H
#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/ModelNode>

class LineNode : public osgEarth::Annotation::FeatureNode
{
public:
    LineNode(MapController *mapController);
    void setLineHeight(float hieght);
    void setLineColor(osgEarth::Color color);
    void setLineWidth(float width);
    void setLineClamp(bool clamp);
    void setPointColor(osgEarth::Color color);
    void setPointSize(float size);
    void setPointClamp(bool clamp);
    osg::ref_ptr<osg::Node> makeLineSphere();



    osg::Group* mCircleGr;

    void addPoint(osgEarth::GeoPoint points);
    void removePoint();
    void clearPath();
    int getSize();


private:
    MapController* mMapController{nullptr};
    osgEarth::Symbology::Geometry* mLinePath;
    osgEarth::Annotation::ModelNode* mCircleModelNode{nullptr};

};

#endif // LINENODE_H
