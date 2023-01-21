#ifndef ROCKET_H
#define ROCKET_H

#include "defenseModelLayer.h"

class Rocket: public DefenseModelNode
{
public:
    Rocket(osgEarth::MapNode* mapNode, QObject *parent);
    void shoot(const osg::Vec3d& pos, double speed);
    void stop() override;
private:
    void addEffect(double emitterDuration);
    void removeEffect();
private:
    ModelAnimationPathCallback* mAnimationPathCallback{nullptr};
    osg::ref_ptr<osgParticle::SmokeTrailEffect> mSmoke;
    osg::ref_ptr<osgParticle::FireEffect> mFire;
    bool mIsShoot{false};
};

#endif // ROCKET_H
