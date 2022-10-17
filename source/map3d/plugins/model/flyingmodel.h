#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include <osgEarthAnnotation/ModelNode>
#include <QObject>
#include <QMouseEvent>
class MapAnimationPathCallback;
class EventCallback;
class FlyingModel: public QObject, public osgEarth::Annotation::ModelNode
{
    Q_OBJECT
public:
    FlyingModel(osgEarth::MapNode* mapNode, const QString &fileName);
    void setLatLongPosition(const osg::Vec3d &pos);
    void flyTo(const osg::Vec3d& pos, double speed);
    void setPause(bool pause);
    bool getPause() const;
    void setFollowingModel(FlyingModel* followingModel);
    FlyingModel* getFollowingModel() const;
    void collision(FlyingModel *other);
    bool isHit() const;
signals:
    void positionChanged(osgEarth::GeoPoint pos);

    void hit(FlyingModel *other);
private:
    MapAnimationPathCallback* mAnimationPathCallback;
    FlyingModel* mFollowingModel{nullptr};
    bool mIsHit{false};
};

#endif // FLYINGMODEL_H
