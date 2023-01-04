#ifndef LINE_H
#define LINE_H
#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"
class Line
{
public:
    Line(MapController *mapController);
    void setLineHeight(float hieght);
    void setLineColor(osgEarth::Color color);
    void setLineWidth(float width);
    void setLineClamp(bool clamp);
    void setPointColor(osgEarth::Color color);
    void setPointSize(float size);
    void setPointClamp(bool clamp);

    void addPoint(const osg::Vec3d& vec);
    void removePoint();
    void clearPoints();
    int getSize();

    osgEarth::Annotation::FeatureNode* getNode();

private:
    osgEarth::Symbology::Geometry* mLinePath;
    osg::ref_ptr<osgEarth::Annotation::FeatureNode> mPathNode;
    MapController* mMapController{nullptr};
    
};

#endif // LINE_H
