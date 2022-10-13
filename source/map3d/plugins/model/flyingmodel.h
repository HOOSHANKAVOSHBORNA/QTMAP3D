#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include <osgEarthAnnotation/ModelNode>
#include <QObject>

class FlyingModel: public osgEarth::Annotation::ModelNode
{
public:
    FlyingModel(osgEarth::MapNode* mapNode, const QString &fileName);
    void setLatLongPosition(const osg::Vec3d &pos);
    void flyTo(const osg::Vec3d& pos, double speed);    
};

#endif // FLYINGMODEL_H
