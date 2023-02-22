
#include "truckf.h"
#include "mapcontroller.h"
#include <osgDB/ReadFile>
#include <QDebug>

osg::ref_ptr<osg::Node> TruckF::mMeshNodeP1;
osg::ref_ptr<osg::Node> TruckF::mMeshNodeP2;
osg::ref_ptr<osg::Node> TruckF::mMeshNodeP3;
osg::ref_ptr<osg::Node> TruckF::mMeshNodeP4;
bool TruckF::mMeshNodesLoaded = false;

TruckF::TruckF(class MapController *mapController) :
    osgEarth::Annotation::ModelNode(mapController->getMapNode(), osgEarth::Symbology::Style())
{
    if (!mMeshNodesLoaded) {
        mMeshNodeP1 = osgDB::readNodeFile("/home/client1/Hooshan/Models/TruckF/TruckF_P1.osgb");
        mMeshNodeP2 = osgDB::readNodeFile("/home/client1/Hooshan/Models/TruckF/TruckF_P2.osgb");
        mMeshNodeP3 = osgDB::readNodeFile("/home/client1/Hooshan/Models/TruckF/TruckF_P3.osgb");
        mMeshNodeP4 = osgDB::readNodeFile("/home/client1/Hooshan/Models/TruckF/TruckF_P4.osgb");
        mMeshNodesLoaded = true;
    }

    mRocketModelNode1 = new Rocket(mapController, nullptr);
    mRocketModelNode2 = new Rocket(mapController, nullptr);
    mRocketModelNode3 = new Rocket(mapController, nullptr);


    osg::Group *rootGroup = new osg::Group;

    mBodyPAT       = new osg::PositionAttitudeTransform;
    mWheelAxis1PAT = new osg::PositionAttitudeTransform;
    mWheelAxis2PAT = new osg::PositionAttitudeTransform;
    mWheelAxis3PAT = new osg::PositionAttitudeTransform;
    mWheelAxis4PAT = new osg::PositionAttitudeTransform;
    mHolderPAT     = new osg::PositionAttitudeTransform;
    mSpinnerPAT    = new osg::PositionAttitudeTransform;

    mRocketModelNode1PAT = new osg::PositionAttitudeTransform;
    mRocketModelNode2PAT = new osg::PositionAttitudeTransform;
    mRocketModelNode3PAT = new osg::PositionAttitudeTransform;

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

    mRocketModelNode1PAT->setPosition(osg::Vec3d(-0.82, 5.0, 0.0));
    mRocketModelNode2PAT->setPosition(osg::Vec3d( 0.0, 5.0, 0.0));
    mRocketModelNode3PAT->setPosition(osg::Vec3d( 0.82, 5.0, 0.0));

    mRocketModelNode1PAT->setScale(osg::Vec3d(0.8, 0.7, 0.7));
    mRocketModelNode2PAT->setScale(osg::Vec3d(0.8, 0.7, 0.7));
    mRocketModelNode3PAT->setScale(osg::Vec3d(0.8, 0.7, 0.7));


    mBodyPAT->addChild(mMeshNodeP1);
    mWheelAxis1PAT->addChild(mMeshNodeP2);
    mWheelAxis2PAT->addChild(mMeshNodeP2);
    mWheelAxis3PAT->addChild(mMeshNodeP2);
    mWheelAxis4PAT->addChild(mMeshNodeP2);
    mHolderPAT->addChild(mMeshNodeP3);
    mSpinnerPAT->addChild(mMeshNodeP4);

    mRocketModelNode1PAT->addChild(mRocketModelNode1);
    mRocketModelNode2PAT->addChild(mRocketModelNode2);
    mRocketModelNode3PAT->addChild(mRocketModelNode3);


    mBodyPAT->addChild(mWheelAxis1PAT);
    mBodyPAT->addChild(mWheelAxis2PAT);
    mBodyPAT->addChild(mWheelAxis3PAT);
    mBodyPAT->addChild(mWheelAxis4PAT);
    mBodyPAT->addChild(mHolderPAT);
    mHolderPAT->addChild(mSpinnerPAT);

    mSpinnerPAT->addChild(mRocketModelNode1PAT);
    mSpinnerPAT->addChild(mRocketModelNode2PAT);
    mSpinnerPAT->addChild(mRocketModelNode3PAT);

    mBodyPAT->addChild(mHolderAimingPAT);
    mHolderAimingPAT->addChild(mSpinnerAimingPAT);

    rootGroup->addChild(mBodyPAT);

    osgEarth::Symbology::Style  style;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(rootGroup);
    setStyle(style);

}

void TruckF::aimTarget(const osgEarth::GeoPoint &gpt)
{
    osg::Vec3d gptWorld;
    gpt.toWorld(gptWorld);


    {
        osg::Matrix matLocalToWorld = osg::Matrix::identity();
        for (const auto& item : mHolderAimingPAT->getWorldMatrices()) {
            matLocalToWorld *= item;
        }
        const osg::Matrix matWorldToLocal = osg::Matrix::inverse(matLocalToWorld);

        osg::Vec3d gptInTruckLocal = gptWorld * matWorldToLocal;
        gptInTruckLocal.normalize();
        gptInTruckLocal.z() = 0;


        osg::Quat rot;
        rot.makeRotate(osg::Vec3d(osg::Y_AXIS), gptInTruckLocal);

        mHolderPAT->setAttitude(rot);
    }

    {
        osg::Matrix matLocalToWorld = osg::Matrix::identity();
        for (const auto& item : mSpinnerAimingPAT->getWorldMatrices()) {
            matLocalToWorld *= item;
        }
        const osg::Matrix matWorldToLocal = osg::Matrix::inverse(matLocalToWorld);



        osg::Vec3d gptInTruckLocal = gptWorld * matWorldToLocal;
        gptInTruckLocal.normalize();

        gptInTruckLocal.y() = std::sqrt(std::pow(gptInTruckLocal.x(), 2) + std::pow(gptInTruckLocal.y(), 2));
        gptInTruckLocal.x() = 0;

        if (gptInTruckLocal.z() < 0) {
            gptInTruckLocal.z() = 0;
        }


        osg::Quat rot;
        rot.makeRotate(osg::Vec3d(osg::Y_AXIS), gptInTruckLocal);

        mSpinnerPAT->setAttitude(rot);
    }

}

bool TruckF::shoot(const osg::Vec3d &pos, double speed)
{

}

