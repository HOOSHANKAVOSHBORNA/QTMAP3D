#ifndef FLYINGMODEL_H
#define FLYINGMODEL_H

#include "basemodel.h"

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
class Airplane: public BaseModel
{
public:
    Airplane(osgEarth::MapNode* mapNode, const QString &fileName, QObject* parent = nullptr);
    void flyTo(const osg::Vec3d& pos, double heading, double speed);
    void stop() override;
    void setTruckModel(osgEarth::Annotation::ModelNode* truckModel);
    osgEarth::Annotation::ModelNode *getTruckModel() const;
private:
    void addEffect(double emitterDuration);
    void removeEffect();
private:
    ModelAnimationPathCallback* mAnimationPathCallback{nullptr};
    osgEarth::Annotation::ModelNode* mTruckModel;
//    osg::ref_ptr<osg::Geode> mGeodeParticle;
    osg::ref_ptr<osgParticle::SmokeTrailEffect> mSmoke;
    osg::ref_ptr<osgParticle::FireEffect> mFire;
    bool mIsStop{false};
};

#endif // FLYINGMODEL_H
