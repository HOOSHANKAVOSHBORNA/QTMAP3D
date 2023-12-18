#ifndef MODELAUTOSCALER_H
#define MODELAUTOSCALER_H

#include <osg/NodeCallback>
#include <osgEarthAnnotation/GeoPositionNode>
#include <osgUtil/CullVisitor>
#include <osg/Camera>

class ModelAutoScaler : public osg::NodeCallback
{
public:
    ModelAutoScaler(double defaultScale, double minScale, double maxScale) :
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
    void setScaled(bool newScaled);

protected:
    osg::Vec3d _baseScale = osg::Vec3d(1,1,1);
    double _defaultScale;
    double _minScale;
    double _maxScale;

    bool _enabled = true;
    bool _scaled = true;
};

#endif // MODELAUTOSCALER_H
