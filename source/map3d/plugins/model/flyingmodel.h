#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include <osgEarthAnnotation/ModelNode>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>

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
    void shoot(const osg::Vec3d& pos, double speed);
    void setPause(bool pause);
    bool getPause() const;
    void setFollowingModel(FlyingModel* followingModel);
    FlyingModel* getFollowingModel() const;
    void setTruckModel(osgEarth::Annotation::ModelNode* truckModel);
    osgEarth::Annotation::ModelNode *getTruckModel() const;
    void collision(FlyingModel *other);
    bool isHit() const;

    void playExplosionEffect(float scale);

signals:
    void positionChanged(osgEarth::GeoPoint pos);

    void hit(FlyingModel *other);
private:
    MapAnimationPathCallback* mAnimationPathCallback{nullptr};
    FlyingModel* mFollowingModel{nullptr};
    osgEarth::Annotation::ModelNode* mTruckModel;
    bool mIsHit{false};
//    osg::ref_ptr<osg::Geode> mGeodeParticle;
    osg::ref_ptr<osgParticle::SmokeTrailEffect> mSmoke;
    osg::ref_ptr<osgParticle::FireEffect> mFire;
};

#endif // FLYINGMODEL_H
