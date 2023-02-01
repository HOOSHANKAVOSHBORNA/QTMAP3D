#ifndef LINE_H
#define LINE_H
#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
#include "mapcontroller.h"
class Line
{
public:
    Line(MapController *mapController, bool point=false);
    void setLineHeight(float hieght);
    void setColor(osgEarth::Color color);
    void setWidth(float width);
    void setClamp(bool clamp);
    void switchLP(bool lineMode);

    float mWidth{50};
    osgEarth::Color mColor{osgEarth::Color::Red};
    bool mClamp{true};

    void addPoint(const osg::Vec3d& vec);
    void removePoint();
    void clearPoints();
    int getSize();

    osgEarth::Annotation::FeatureNode* getNode();

private:
    osgEarth::Symbology::Geometry* mLinePath;
    osg::ref_ptr<osgEarth::Annotation::FeatureNode> mPathNode;
    MapController* mMapController{nullptr};
    //osgEarth::Symbology::Style pointStyle;
    bool mIsPoint{false};
    
};

#endif // LINE_H
