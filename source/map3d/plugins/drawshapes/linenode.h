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
    LineNode(MapController *mapController, bool point=false);
    void setLineHeight(float hieght);
    void setColor(osgEarth::Color color);
    void setWidth(float width);
    void setClamp(bool clamp);
    void setDashLine(bool dashLine);

    osg::ref_ptr<osgEarth::Annotation::ModelNode> mCircleModelNode{nullptr};
    osg::ref_ptr<osg::Group> mCircleGr;

    void addPoint(osgEarth::GeoPoint points);
    void removePoint();
    void removeFirstPoint();
    void clearPath();
    int getSize();
    void setPointVisibilty(bool visibility);
    void setPointColor(osgEarth::Color color);
    osg::ref_ptr<osg::Material> sphereMat;
    osgEarth::Color pointColor{osgEarth::Color::DarkGray};



private:
    MapController* mMapController{nullptr};
    osgEarth::Symbology::Geometry* mLinePath;
//    osgEarth::Annotation::ModelNode* mCircleModelNode{nullptr};

    bool mIsPoint{false};
    float mWidth{50};
    osgEarth::Color mColor{osgEarth::Color::Red};
    bool mClamp{true};
    osg::ref_ptr<osg::Node> sphere;
};

#endif // LINENODE_H
