#ifndef TRUCKF_H
#define TRUCKF_H

#include <osgEarthAnnotation/ModelNode>
#include <osg/Node>
#include "rocket.h"

class TruckF : public osgEarth::Annotation::ModelNode
{
public:
    TruckF(class MapItem *mapController);
    void aimTarget(const osgEarth::GeoPoint &gpt);
    bool shoot(const osg::Vec3d &pos, double speed);
    Rocket* getActiveRocket()const;

    void setRocketsCapacity(int capacity);
    void setAvailableRocketsCount();


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

    QList<osg::ref_ptr<Rocket>> mRocketModelNodeList;


    osg::ref_ptr<osg::PositionAttitudeTransform> mBodyPAT       = nullptr;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis1PAT = nullptr;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis2PAT = nullptr;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis3PAT = nullptr;
    osg::ref_ptr<osg::PositionAttitudeTransform> mWheelAxis4PAT = nullptr;
    osg::ref_ptr<osg::PositionAttitudeTransform> mHolderPAT     = nullptr;
    osg::ref_ptr<osg::PositionAttitudeTransform> mSpinnerPAT    = nullptr;

    osg::ref_ptr<osg::PositionAttitudeTransform> mHolderAimingPAT     = nullptr;
    osg::ref_ptr<osg::PositionAttitudeTransform> mSpinnerAimingPAT    = nullptr;

    QList<osg::ref_ptr<osg::PositionAttitudeTransform>> mRocketModelNodePatList;


    int mAvailableRockets = 3;
    int mNextRocketIndex = 0;

};

#endif // TRUCKF_H
