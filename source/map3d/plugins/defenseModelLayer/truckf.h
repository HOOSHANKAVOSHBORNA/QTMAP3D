#ifndef TRUCKF_H
#define TRUCKF_H

#include <osgEarthAnnotation/ModelNode>
#include <osg/Node>
#include "rocket.h"

class TruckF : public osgEarth::Annotation::ModelNode
{
public:
    TruckF(class MapController *mapController);
    void aimTarget(const osgEarth::GeoPoint &gpt);
    bool shoot(const osg::Vec3d &pos, double speed);

protected:
    static osg::ref_ptr<osg::Node> mMeshNodeP1;
    static osg::ref_ptr<osg::Node> mMeshNodeP2;
    static osg::ref_ptr<osg::Node> mMeshNodeP3;
    static osg::ref_ptr<osg::Node> mMeshNodeP4;
    static bool mMeshNodesLoaded;

    osg::ref_ptr<osg::Node> mMeshNodeP1Instance;
    osg::ref_ptr<osg::Node> mMeshNodeP2Instance;
    osg::ref_ptr<osg::Node> mMeshNodeP3Instance;
    osg::ref_ptr<osg::Node> mMeshNodeP4Instance;


    osg::ref_ptr<Rocket> mRocketModelNode1;
    osg::ref_ptr<Rocket> mRocketModelNode2;
    osg::ref_ptr<Rocket> mRocketModelNode3;


    osg::PositionAttitudeTransform* mBodyPAT       = nullptr;
    osg::PositionAttitudeTransform* mWheelAxis1PAT = nullptr;
    osg::PositionAttitudeTransform* mWheelAxis2PAT = nullptr;
    osg::PositionAttitudeTransform* mWheelAxis3PAT = nullptr;
    osg::PositionAttitudeTransform* mWheelAxis4PAT = nullptr;
    osg::PositionAttitudeTransform* mHolderPAT     = nullptr;
    osg::PositionAttitudeTransform* mSpinnerPAT    = nullptr;

    osg::PositionAttitudeTransform* mHolderAimingPAT     = nullptr;
    osg::PositionAttitudeTransform* mSpinnerAimingPAT    = nullptr;


    osg::PositionAttitudeTransform* mRocketModelNode1PAT     = nullptr;
    osg::PositionAttitudeTransform* mRocketModelNode2PAT     = nullptr;
    osg::PositionAttitudeTransform* mRocketModelNode3PAT     = nullptr;


};

#endif // TRUCKF_H
