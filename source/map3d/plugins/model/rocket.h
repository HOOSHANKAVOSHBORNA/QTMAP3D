#ifndef ROCKET_H
#define ROCKET_H

#include "basemodel.h"

class Rocket: public BaseModel
{
public:
    Rocket(osgEarth::MapNode* mapNode, QObject* parent = nullptr);
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
