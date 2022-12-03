#ifndef TRUCK_H
#define TRUCK_H

#include "basemodel.h"
#include "rocket.h"


class TruckUpdateCallback;

class Truck: public BaseModel
{
public:
    Truck(osgEarth::MapNode* mapNode, QObject* parent = nullptr);
    void moveTo(const osg::Vec3d &pos, double speed);
    void aimTarget(const osg::Vec3d &pos);
    bool shoot(const osg::Vec3d &pos, double speed);
    Rocket* getActiveRocket()const;
    bool hasRocket();
    void stop() override;
private:
    osg::Vec3d computeRocketWorldPosition(Rocket* rocket);
private:
    osg::ref_ptr<Rocket> mRocket1;
    osg::ref_ptr<Rocket> mRocket2;
    osg::ref_ptr<Rocket> mRocket3;

    osg::ref_ptr<osg::AnimationPathCallback> mLeftWheelUpdateCallback{nullptr};
    osg::ref_ptr<osg::AnimationPathCallback> mRightWheelUpdateCallback{nullptr};
    osg::ref_ptr<osg::PositionAttitudeTransform> mHolder{nullptr};
    osg::ref_ptr<osg::PositionAttitudeTransform> mSpiner{nullptr};

    osg::ref_ptr<ModelAnimationPathCallback> mMoveAnimationPathCallback;

    int mRocketIndex{3};
};




#endif // TRUCK_H
