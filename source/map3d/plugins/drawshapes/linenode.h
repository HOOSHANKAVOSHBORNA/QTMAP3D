#ifndef LINENODE_H
#define LINENODE_H
#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"
#include <osgEarthAnnotation/CircleNode>
#include <osgEarthAnnotation/ModelNode>
#include "spherenode.h"

class LineNode : public osgEarth::Annotation::FeatureNode
{
public:
    LineNode(MapController *mapController, bool point=false);
    void setLineHeight(float hieght);
    void setColor(osgEarth::Color color);
    void setWidth(float width);
    void setClamp(bool clamp);
    void setDashLine(bool dashLine);
    void addPoint(osgEarth::GeoPoint point);
    void removePoint();
    void removeFirstPoint();
    void clearPath();
    int getSize();
    void setPointVisibilty(bool visibility);
    void setPointColor(osgEarth::Color color);
private:
    osg::ref_ptr<osg::Geometry>  createPointGeode(const osg::Vec3 &pos);
private:
    MapController* mMapController{nullptr};
    osgEarth::Symbology::Geometry* mLinePath;
//    osgEarth::Annotation::ModelNode* mCircleModelNode{nullptr};

    bool mIsPoint{false};
    float mWidth{50};
    osgEarth::Color mColor{osgEarth::Color::Red};
    bool mClamp{true};
    bool mIsPointVisible{false};
//    SphereNode* mSphereNode{nullptr};
    osg::ref_ptr<osg::Group> mCircleGr;
    osgEarth::Color mPointColor{osgEarth::Color::DarkGray};
};

#endif // LINENODE_H
