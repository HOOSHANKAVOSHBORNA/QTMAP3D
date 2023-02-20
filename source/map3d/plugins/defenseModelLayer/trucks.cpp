
#include "trucks.h"
#include "mapcontroller.h"

TruckS::TruckS(MapController *mapController) :
    osgEarth::Annotation::ModelNode(mapController->getMapNode(), osgEarth::Symbology::Style())
{
    if (!mMeshNodesLoaded) {
        mMeshNodeP1 = osgDB::readNodeFile("/home/client1/Hooshan/Models/TruckS/TruckS_P1.osgb");
        mMeshNodeP2 = osgDB::readNodeFile("/home/client1/Hooshan/Models/TruckS/TruckS_P2.osgb");
        mMeshNodeP3 = osgDB::readNodeFile("/home/client1/Hooshan/Models/TruckS/TruckS_P3.osgb");
        mMeshNodeP4 = osgDB::readNodeFile("/home/client1/Hooshan/Models/TruckS/TruckS_P4.osgb");
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
    mHolderPAT->setPosition(osg::Vec3d(0, -0.8222, 1.9093));
    mSpinnerPAT->setPosition(osg::Vec3d(0,-1.2712, 0.8134));
    mHolderAimingPAT->setPosition(osg::Vec3d(0, -0.8222, 1.9093));
    mSpinnerAimingPAT->setPosition(osg::Vec3d(0,-1.2712, 0.8134));


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

}
