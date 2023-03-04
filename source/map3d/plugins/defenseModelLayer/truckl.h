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
    static osg::ref_ptr<osg::Node> mMeshNodeP1LOD0;
    static osg::ref_ptr<osg::Node> mMeshNodeP2LOD0;
    static osg::ref_ptr<osg::Node> mMeshNodeP3LOD0;
    static bool mMeshNodesLoaded;

    static osg::ref_ptr<osg::Node> mMeshNodeP1LOD1;
    static osg::ref_ptr<osg::Node> mMeshNodeP2LOD1;
    static osg::ref_ptr<osg::Node> mMeshNodeP3LOD1;

//    osg::ref_ptr<osg::Node> mMeshNodeP1InstanceLOD0;
//    osg::ref_ptr<osg::Node> mMeshNodeP2InstanceLOD0;
//    osg::ref_ptr<osg::Node> mMeshNodeP3InstanceLOD0;

//    osg::ref_ptr<osg::Node> mMeshNodeP1InstanceLOD1;
//    osg::ref_ptr<osg::Node> mMeshNodeP2InstanceLOD1;
//    osg::ref_ptr<osg::Node> mMeshNodeP3InstanceLOD1;


    osg::ref_ptr<osg::PositionAttitudeTransform> mBodyPAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis1PAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis2PAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis3PAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis4PAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mSpinnerPAT;

    osg::ref_ptr<osg::PositionAttitudeTransform> mSpinnerAimingPAT;


};

#endif // TRUCKL_H
