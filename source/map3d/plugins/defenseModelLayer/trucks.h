#ifndef TRUCKS_H
#define TRUCKS_H

#include <osgEarthAnnotation/ModelNode>

class TruckS : public osgEarth::Annotation::ModelNode
{
public:
    TruckS(class MapItem *mapController);

    void startSearch();
    void stopSearch();

protected:
    static osg::ref_ptr<osg::Node> mMeshNodeP1LOD0;
    static osg::ref_ptr<osg::Node> mMeshNodeP2LOD0;
    static osg::ref_ptr<osg::Node> mMeshNodeP3LOD0;
    static osg::ref_ptr<osg::Node> mMeshNodeP4LOD0;

    static osg::ref_ptr<osg::Node> mMeshNodeP1LOD1;
    static osg::ref_ptr<osg::Node> mMeshNodeP2LOD1;
    static osg::ref_ptr<osg::Node> mMeshNodeP3LOD1;
    static osg::ref_ptr<osg::Node> mMeshNodeP4LOD1;

    static bool mMeshNodesLoaded;

    osg::ref_ptr<osg::Node> mMeshNodeP1Instance;
    osg::ref_ptr<osg::Node> mMeshNodeP2Instance;
    osg::ref_ptr<osg::Node> mMeshNodeP3Instance;
    osg::ref_ptr<osg::Node> mMeshNodeP4Instance;


    osg::ref_ptr<osg::PositionAttitudeTransform> mBodyPAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis1PAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis2PAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis3PAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis4PAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mHolderPAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mSpinnerPAT;

    osg::ref_ptr<osg::PositionAttitudeTransform> mHolderAimingPAT;
    osg::ref_ptr<osg::PositionAttitudeTransform> mSpinnerAimingPAT;

    osg::ref_ptr<osg::AnimationPathCallback> mAnimPathCallback;

};


#endif // TRUCKS_H
