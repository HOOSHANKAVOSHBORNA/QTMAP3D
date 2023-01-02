#ifndef LINE_H
#define LINE_H
#include <osgEarth/Color>
#include <osgEarthAnnotation/FeatureNode>
class Line
{
public:
    Line();
    void setLineHeight(float hieght);
    void setLineColor(osgEarth::Color color);
    void setLineWidth(float width);
    void setLineClamp(bool clamp);
    void setPointColor(osgEarth::Color color);
    void setPointSize(float size);
    void setPointClamp(bool clamp);

    void addPoint(const osg::Vec3d& vec);
    void removePoint();
    int getSize();

    osgEarth::Annotation::FeatureNode* getNode();

private:
    osgEarth::Symbology::Geometry* mLinePath;
    osgEarth::Annotation::FeatureNode* mPathNode;
    
};

#endif // LINE_H
