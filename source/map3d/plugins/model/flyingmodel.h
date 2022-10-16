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
    void setFollowModel(FlyingModel* followModel);
    FlyingModel* getFollowModel() const;
signals:
    void positionChanged(osgEarth::GeoPoint pos);
    void hit();
private:
    MapAnimationPathCallback* mAnimationPathCallback;
    FlyingModel* mFollowModel{nullptr};
};

#endif // FLYINGMODEL_H
