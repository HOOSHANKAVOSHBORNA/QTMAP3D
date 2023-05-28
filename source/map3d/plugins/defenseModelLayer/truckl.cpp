
#include "truckl.h"
#include "mapItem.h"
#include "defenseModelLayer.h"


osg::ref_ptr<osg::Node> TruckL::mMeshNodeP1LOD0;
osg::ref_ptr<osg::Node> TruckL::mMeshNodeP2LOD0;
osg::ref_ptr<osg::Node> TruckL::mMeshNodeP3LOD0;

osg::ref_ptr<osg::Node> TruckL::mMeshNodeP1LOD1;
osg::ref_ptr<osg::Node> TruckL::mMeshNodeP2LOD1;
osg::ref_ptr<osg::Node> TruckL::mMeshNodeP3LOD1;

bool TruckL::mMeshNodesLoaded = false;

TruckL::TruckL(class MapItem *mapItem) :
    osgEarth::Annotation::ModelNode(mapItem->getMapNode(), DefenseModelLayer::getDefaultStyle())
{
    if (!mMeshNodesLoaded) {
        mMeshNodeP1LOD0 = osgDB::readNodeFile("../data/models/system/truck_l/LOD0/TruckL_P1.osgb");
        mMeshNodeP2LOD0 = osgDB::readNodeFile("../data/models/system/truck_l/LOD0/TruckL_P2.osgb");
        mMeshNodeP3LOD0 = osgDB::readNodeFile("../data/models/system/truck_l/LOD0/TruckL_P3.osgb");

        mMeshNodeP1LOD1 = osgDB::readNodeFile("../data/models/system/truck_l/LOD1/TruckL_P1.osgb");
        mMeshNodeP2LOD1 = osgDB::readNodeFile("../data/models/system/truck_l/LOD1/TruckL_P2.osgb");
        mMeshNodeP3LOD1 = osgDB::readNodeFile("../data/models/system/truck_l/LOD1/TruckL_P3.osgb");


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


    lodP1->setRange(0, 0, 50);
    lodP1->setRange(1, 50, std::numeric_limits<float>::max());
    lodP2->setRange(0, 0, 50);
    lodP2->setRange(1, 50, std::numeric_limits<float>::max());
    lodP3->setRange(0, 0, 50);
    lodP3->setRange(1, 50, std::numeric_limits<float>::max());


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

    mBodyPAT->addChild(lodP1);
    mWheelAxis1PAT->addChild(lodP2);
    mWheelAxis2PAT->addChild(lodP2);
    mWheelAxis3PAT->addChild(lodP2);
    mWheelAxis4PAT->addChild(lodP2);
    mSpinnerPAT->addChild(lodP3);

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

