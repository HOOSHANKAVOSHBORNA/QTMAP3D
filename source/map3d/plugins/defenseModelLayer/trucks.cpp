
#include "trucks.h"
#include "mapItem.h"
#include "defenseModelLayer.h"

osg::ref_ptr<osg::Node> TruckS::mMeshNodeP1LOD0;
osg::ref_ptr<osg::Node> TruckS::mMeshNodeP2LOD0;
osg::ref_ptr<osg::Node> TruckS::mMeshNodeP3LOD0;
osg::ref_ptr<osg::Node> TruckS::mMeshNodeP4LOD0;

osg::ref_ptr<osg::Node> TruckS::mMeshNodeP1LOD1;
osg::ref_ptr<osg::Node> TruckS::mMeshNodeP2LOD1;
osg::ref_ptr<osg::Node> TruckS::mMeshNodeP3LOD1;
osg::ref_ptr<osg::Node> TruckS::mMeshNodeP4LOD1;

bool TruckS::mMeshNodesLoaded = false;

TruckS::TruckS(MapItem *mapItem) :
    osgEarth::Annotation::ModelNode(mapItem->getMapNode(), DefenseModelLayer::getDefaultStyle())
{
    if (!mMeshNodesLoaded) {

        mMeshNodeP1LOD0 = osgDB::readNodeFile("../data/models/system/truck_s/LOD0/TruckS_P1.osgb");
        mMeshNodeP2LOD0 = osgDB::readNodeFile("../data/models/system/truck_s/LOD0/TruckS_P2.osgb");
        mMeshNodeP3LOD0 = osgDB::readNodeFile("../data/models/system/truck_s/LOD0/TruckS_P3.osgb");
        mMeshNodeP4LOD0 = osgDB::readNodeFile("../data/models/system/truck_s/LOD0/TruckS_P4.osgb");

        mMeshNodeP1LOD1 = osgDB::readNodeFile("../data/models/system/truck_s/LOD1/TruckS_P1.osgb");
        mMeshNodeP2LOD1 = osgDB::readNodeFile("../data/models/system/truck_s/LOD1/TruckS_P2.osgb");
        mMeshNodeP3LOD1 = osgDB::readNodeFile("../data/models/system/truck_s/LOD1/TruckS_P3.osgb");
        mMeshNodeP4LOD1 = osgDB::readNodeFile("../data/models/system/truck_s/LOD1/TruckS_P4.osgb");

        mMeshNodesLoaded = true;
    }

    osg::ref_ptr<osg::LOD> lodP1 = new osg::LOD;
    lodP1->addChild(mMeshNodeP1LOD0);
    lodP1->addChild(mMeshNodeP1LOD1);
    lodP1->setChild(0, mMeshNodeP1LOD0);
    lodP1->setChild(1, mMeshNodeP1LOD1);

    osg::ref_ptr<osg::LOD> lodP2 = new osg::LOD;
    lodP2->addChild(mMeshNodeP2LOD0);
    lodP2->addChild(mMeshNodeP2LOD1);
    lodP2->setChild(0, mMeshNodeP2LOD0);
    lodP2->setChild(1, mMeshNodeP2LOD1);

    osg::ref_ptr<osg::LOD> lodP3 = new osg::LOD;
    lodP3->addChild(mMeshNodeP3LOD0);
    lodP3->addChild(mMeshNodeP3LOD1);
    lodP3->setChild(0, mMeshNodeP3LOD0);
    lodP3->setChild(1, mMeshNodeP3LOD1);

    osg::ref_ptr<osg::LOD> lodP4 = new osg::LOD;
    lodP4->addChild(mMeshNodeP4LOD0);
    lodP4->addChild(mMeshNodeP4LOD1);
    lodP4->setChild(0, mMeshNodeP4LOD0);
    lodP4->setChild(1, mMeshNodeP4LOD1);


    lodP1->setRange(0, 0, 50);
    lodP1->setRange(1, 50, std::numeric_limits<float>::max());
    lodP2->setRange(0, 0, 50);
    lodP2->setRange(1, 50, std::numeric_limits<float>::max());
    lodP3->setRange(0, 0, 50);
    lodP3->setRange(1, 50, std::numeric_limits<float>::max());
    lodP4->setRange(0, 0, 50);
    lodP4->setRange(1, 50, std::numeric_limits<float>::max());


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

    mBodyPAT->addChild(lodP1);
    mWheelAxis1PAT->addChild(lodP2);
    mWheelAxis2PAT->addChild(lodP2);
    mWheelAxis3PAT->addChild(lodP2);
    mWheelAxis4PAT->addChild(lodP2);
    mHolderPAT->addChild(lodP3);
    mSpinnerPAT->addChild(lodP4);

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
