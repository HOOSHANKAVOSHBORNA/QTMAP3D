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
        _maxScale( maxScale )
    {

    }

public: // osg::NodeCallback

    void operator()(osg::Node* node, osg::NodeVisitor* nv);

    void setDefaultScale(double defaultScale);
    void setMinScale(double minScale);
    void setMaxScale(double maxScale);
    void setEnabled(bool enabled);



protected:
    osg::Vec3d _baseScale = osg::Vec3d(1,1,1);
    double _defaultScale;
    double _minScale;
    double _maxScale;

    bool _enabled = true;
};
#endif // DEFENSEMODELNODEAUTOSCALER_H
