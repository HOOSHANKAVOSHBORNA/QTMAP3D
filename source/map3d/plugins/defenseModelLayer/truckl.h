#ifndef TRUCKL_H
#define TRUCKL_H

#include <osgEarthAnnotation/ModelNode>
#include <osg/Node>
#include "rocket.h"

class TruckL : public osgEarth::Annotation::ModelNode
{
public:
    TruckL(class MapController *mapController);
    void lockOnTarget(const osgEarth::GeoPoint &gpt);

protected:
    static osg::ref_ptr<osg::Node> mMeshNodeP1;
    static osg::ref_ptr<osg::Node> mMeshNodeP2;
    static osg::ref_ptr<osg::Node> mMeshNodeP3;
//    static osg::ref_ptr<osg::Node> mMeshNodeP4;
    static bool mMeshNodesLoaded;

    osg::ref_ptr<osg::Node> mMeshNodeP1Instance;
    osg::ref_ptr<osg::Node> mMeshNodeP2Instance;
    osg::ref_ptr<osg::Node> mMeshNodeP3Instance;
//    osg::ref_ptr<osg::Node> mMeshNodeP4Instance;


    osg::PositionAttitudeTransform* mBodyPAT       = nullptr;
    osg::PositionAttitudeTransform* mWheelAxis1PAT = nullptr;
    osg::PositionAttitudeTransform* mWheelAxis2PAT = nullptr;
    osg::PositionAttitudeTransform* mWheelAxis3PAT = nullptr;
    osg::PositionAttitudeTransform* mWheelAxis4PAT = nullptr;
//    osg::PositionAttitudeTransform* mHolderPAT     = nullptr;
    osg::PositionAttitudeTransform* mSpinnerPAT    = nullptr;

//    osg::PositionAttitudeTransform* mHolderAimingPAT     = nullptr;
    osg::PositionAttitudeTransform* mSpinnerAimingPAT    = nullptr;



};

#endif // TRUCKL_H
