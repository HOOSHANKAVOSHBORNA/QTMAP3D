
#include "trucks.h"
#include "mapcontroller.h"
#include "defenseModelLayer.h"

osg::ref_ptr<osg::Node> TruckS::mMeshNodeP1;
osg::ref_ptr<osg::Node> TruckS::mMeshNodeP2;
osg::ref_ptr<osg::Node> TruckS::mMeshNodeP3;
osg::ref_ptr<osg::Node> TruckS::mMeshNodeP4;
bool TruckS::mMeshNodesLoaded = false;

TruckS::TruckS(MapController *mapController) :
    osgEarth::Annotation::ModelNode(mapController->getMapNode(), DefenseModelLayer::getDefaultStyle())
{
    if (!mMeshNodesLoaded) {
        mMeshNodeP1 = osgDB::readNodeFile("../data/models/system/truck_s/TruckS_P1.osgb");
        mMeshNodeP2 = osgDB::readNodeFile("../data/models/system/truck_s/TruckS_P2.osgb");
        mMeshNodeP3 = osgDB::readNodeFile("../data/models/system/truck_s/TruckS_P3.osgb");
        mMeshNodeP4 = osgDB::readNodeFile("../data/models/system/truck_s/TruckS_P4.osgb");
        mMeshNodesLoaded = true;
    }

    osg::Group *rootGroup = new osg::Group;

    mBodyPAT       = new osg::PositionAttitudeTransform;
    mWheelAxis1PAT = new osg::PositionAttitudeTransform;
    mWheelAxis2PAT = new osg::PositionAttitudeTransform;
    mWheelAxis3PAT = new osg::PositionAttitudeTransform;
    mWheelAxis4PAT = new osg::PositionAttitudeTransform;
    mHolderPAT     = new osg::PositionAttitudeTransform;
    mSpinnerPAT    = new osg::PositionAttitudeTransform;

    mHolderAimingPAT     = new osg::PositionAttitudeTransform;
    mSpinnerAimingPAT    = new osg::PositionAttitudeTransform;

    mWheelAxis1PAT->setPosition(osg::Vec3d(0, 2.3806, 0.5288));
    mWheelAxis2PAT->setPosition(osg::Vec3d(0, 1.2703, 0.5288));
    mWheelAxis3PAT->setPosition(osg::Vec3d(0,-0.8238, 0.5288));
    mWheelAxis4PAT->setPosition(osg::Vec3d(0,-1.9342, 0.5288));
    mHolderPAT->setPosition(osg::Vec3d(0, -1.0125, 2.0572));
    mSpinnerPAT->setPosition(osg::Vec3d(0, -1.0136 + 1.0125, 3.6984 - 2.0572));
    mHolderAimingPAT->setPosition(osg::Vec3d(0, -1.0125, 2.0572));
    mSpinnerAimingPAT->setPosition(osg::Vec3d(0, -1.0136 - 1.0125, 3.6984 - 2.0572));


    mSpinnerPAT->setAttitude(osg::Quat(osg::inDegrees(30.0), osg::X_AXIS));

    mBodyPAT->addChild(mMeshNodeP1);
    mWheelAxis1PAT->addChild(mMeshNodeP2);
    mWheelAxis2PAT->addChild(mMeshNodeP2);
    mWheelAxis3PAT->addChild(mMeshNodeP2);
    mWheelAxis4PAT->addChild(mMeshNodeP2);
    mHolderPAT->addChild(mMeshNodeP3);
    mSpinnerPAT->addChild(mMeshNodeP4);

    mBodyPAT->addChild(mWheelAxis1PAT);
    mBodyPAT->addChild(mWheelAxis2PAT);
    mBodyPAT->addChild(mWheelAxis3PAT);
    mBodyPAT->addChild(mWheelAxis4PAT);
    mBodyPAT->addChild(mHolderPAT);
    mHolderPAT->addChild(mSpinnerPAT);

    mBodyPAT->addChild(mHolderAimingPAT);
    mHolderAimingPAT->addChild(mSpinnerAimingPAT);

    rootGroup->addChild(mBodyPAT);

    osgEarth::Symbology::Style  style;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(rootGroup);
    setStyle(style);



    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath;
    path->insert(0,
                 osg::AnimationPath::ControlPoint(
                     osg::Vec3d(0, -1.0125, 2.0572),
                     osg::Quat(osg::inDegrees(0.0), osg::Z_AXIS))
                 );
    path->insert(1,
                 osg::AnimationPath::ControlPoint(
                     osg::Vec3d(0, -1.0125, 2.0572),
                     osg::Quat(osg::inDegrees(180.0), osg::Z_AXIS))
                 );
    path->insert(2,
                 osg::AnimationPath::ControlPoint(
                     osg::Vec3d(0, -1.0125, 2.0572),
                     osg::Quat(osg::inDegrees(360.0), osg::Z_AXIS))
                 );
    path->setLoopMode(osg::AnimationPath::LOOP);

    mAnimPathCallback = new osg::AnimationPathCallback;
    mAnimPathCallback->setAnimationPath(path);

    mHolderPAT->setUpdateCallback(mAnimPathCallback);

    mAnimPathCallback->reset();
    mAnimPathCallback->setPause(false);

}

void TruckS::startSearch()
{
    mAnimPathCallback->setPause(false);
}

void TruckS::stopSearch()
{
    mAnimPathCallback->setPause(true);
}
