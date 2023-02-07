#ifndef DEFENSEMODELNODEAUTOSCALER_H
#define DEFENSEMODELNODEAUTOSCALER_H

#include <osg/NodeCallback>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgUtil/CullVisitor>
#include <osg/Camera>

class DefenseModelNodeAutoScaler : public osg::NodeCallback
{
public:
    DefenseModelNodeAutoScaler(double defaultScale, double minScale, double maxScale) :
        _defaultScale( defaultScale ),
        _minScale( minScale ),
        _maxScale( maxScale * defaultScale)
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
#endif // DEFENSEMODELNODEAUTOSCALER_H
