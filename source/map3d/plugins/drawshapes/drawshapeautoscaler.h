#ifndef DRAWSHAPEAUTOSCALER_H
#define DRAWSHAPEAUTOSCALER_H


#include <osg/NodeCallback>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgUtil/CullVisitor>
#include <osg/Camera>

class DrawShapeAutoScaler : public osg::NodeCallback
{
public:
    DrawShapeAutoScaler(double defaultScale, double minScale, double maxScale) :
        _defaultScale( defaultScale ),
        _minScale( minScale ),
        _maxScale( maxScale)
    {

    }

public: // osg::NodeCallback

    void operator()(osg::Node* node, osg::NodeVisitor* nv);


protected:
    osg::Vec3d _baseScale = osg::Vec3d(1,1,1);
    double _defaultScale;
    double _minScale;
    double _maxScale;
};


#endif // DRAWSHAPEAUTOSCALER_H
