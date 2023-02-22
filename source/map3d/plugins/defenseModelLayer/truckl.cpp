
#include "truckl.h"
#include "mapcontroller.h"
#include "defenseModelLayer.h"


osg::ref_ptr<osg::Node> TruckL::mMeshNodeP1;
osg::ref_ptr<osg::Node> TruckL::mMeshNodeP2;
osg::ref_ptr<osg::Node> TruckL::mMeshNodeP3;
//osg::ref_ptr<osg::Node> TruckL::mMeshNodeP4;
bool TruckL::mMeshNodesLoaded = false;

TruckL::TruckL(class MapController *mapController) :
    osgEarth::Annotation::ModelNode(mapController->getMapNode(), DefenseModelLayer::getDefaultStyle())
{
    if (!mMeshNodesLoaded) {
        mMeshNodeP1 = osgDB::readNodeFile("../data/models/system/truck_l/TruckL_P1.osgb");
        mMeshNodeP2 = osgDB::readNodeFile("../data/models/system/truck_l/TruckL_P2.osgb");
        mMeshNodeP3 = osgDB::readNodeFile("../data/models/system/truck_l/TruckL_P3.osgb");
        mMeshNodesLoaded = true;
    }



    osg::Group *rootGroup = new osg::Group;

    mBodyPAT       = new osg::PositionAttitudeTransform;
    mWheelAxis1PAT = new osg::PositionAttitudeTransform;
    mWheelAxis2PAT = new osg::PositionAttitudeTransform;
    mWheelAxis3PAT = new osg::PositionAttitudeTransform;
    mWheelAxis4PAT = new osg::PositionAttitudeTransform;
    mSpinnerPAT    = new osg::PositionAttitudeTransform;


    mSpinnerAimingPAT    = new osg::PositionAttitudeTransform;

    mWheelAxis1PAT->setPosition(osg::Vec3d(0, 2.3806, 0.5288));
    mWheelAxis2PAT->setPosition(osg::Vec3d(0, 1.2703, 0.5288));
    mWheelAxis3PAT->setPosition(osg::Vec3d(0,-0.8238, 0.5288));
    mWheelAxis4PAT->setPosition(osg::Vec3d(0,-1.9342, 0.5288));
    mSpinnerPAT->setPosition(osg::Vec3d(0,-1.135, 1.6578));
    mSpinnerAimingPAT->setPosition(osg::Vec3d(0,-1.135, 1.6578));

    mBodyPAT->addChild(mMeshNodeP1);
    mWheelAxis1PAT->addChild(mMeshNodeP2);
    mWheelAxis2PAT->addChild(mMeshNodeP2);
    mWheelAxis3PAT->addChild(mMeshNodeP2);
    mWheelAxis4PAT->addChild(mMeshNodeP2);
    mSpinnerPAT->addChild(mMeshNodeP3);

    mBodyPAT->addChild(mWheelAxis1PAT);
    mBodyPAT->addChild(mWheelAxis2PAT);
    mBodyPAT->addChild(mWheelAxis3PAT);
    mBodyPAT->addChild(mWheelAxis4PAT);
    mBodyPAT->addChild(mSpinnerPAT);


    mBodyPAT->addChild(mSpinnerAimingPAT);

    mSpinnerPAT->setAttitude(osg::Quat(osg::inDegrees(90.0), osg::Z_AXIS));

    rootGroup->addChild(mBodyPAT);

    osgEarth::Symbology::Style  style;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(rootGroup);
    setStyle(style);

}

void TruckL::lockOnTarget(const osgEarth::GeoPoint &gpt)
{
    osg::Vec3d gptWorld;
    gpt.toWorld(gptWorld);


    {
        osg::Matrix matLocalToWorld = osg::Matrix::identity();
        for (const auto& item : mSpinnerAimingPAT->getWorldMatrices()) {
            matLocalToWorld *= item;
        }
        const osg::Matrix matWorldToLocal = osg::Matrix::inverse(matLocalToWorld);



        osg::Vec3d gptInTruckLocal = gptWorld * matWorldToLocal;
        gptInTruckLocal.normalize();

        gptInTruckLocal.z() = 0;



        osg::Quat rot;
        rot.makeRotate(osg::Vec3d(osg::Y_AXIS), gptInTruckLocal);

        mSpinnerPAT->setAttitude(rot);
    }

}

