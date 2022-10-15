#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include <osgEarthAnnotation/ModelNode>
#include <QObject>

class MapAnimationPathCallback;

class FlyingModel: public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    FlyingModel(osgEarth::MapNode* mapNode, const QString &fileName);
    void setLatLongPosition(const osg::Vec3d &pos);
    void flyTo(const osg::Vec3d& pos, double speed);
    void setPause(bool pause);
    bool getPause() const;
signals:
    void positionChanged(osgEarth::GeoPoint pos);
private:
    MapAnimationPathCallback* mAnimationPathCallback;
};

#endif // FLYINGMODEL_H
