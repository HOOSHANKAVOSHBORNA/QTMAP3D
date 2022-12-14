#ifndef POLYGONE_H
#define POLYGONE_H
#include <osgEarthAnnotation/FeatureNode>

class Polygone : public osgEarth::Annotation::FeatureNode
{
public:
    Polygone(bool clamp);
    void setColor(osgEarth::Color color);
    void setWidth(float width);
    void setClamp(bool clamp);
    void addPoints(osg::Vec3d point);
    osgEarth::Features::Polygon points();
    osgEarth::Features::Geometry* geom;
private:

};


#endif // POLYGONE_H
