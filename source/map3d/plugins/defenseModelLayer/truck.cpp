#include "truck.h"
#include "draw.h"

#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgParticle/FireEffect>
#include <osgEarthDrivers/gdal/GDALOptions>
#include <osgEarth/ElevationLayer>

#include <QDebug>

const QString ROCKET = "Rocket";
osg::ref_ptr<osg::Node> truckNode;
osg::ref_ptr<osg::Node> wheelNode;
osg::ref_ptr<osg::Node> dualWheelNode;
osg::ref_ptr<osg::Node> spinerNode;
osg::ref_ptr<osg::Node> holderNode;

Truck::Truck(MapController *mapControler, GeoPositionNode *parent):
    osgEarth::Annotation::ModelNode(mapControler->getMapNode(), osgEarth::Symbology::Style())
{
    mParent = parent;
    //--read nodes-------------------------------------------------------------------------------------------
    if(!truckNode.valid())
        truckNode     = osgDB::readRefNodeFile("../data/models/system/truck/truck-body.osgt");
    if(!wheelNode.valid())
        wheelNode     = osgDB::readRefNodeFile("../data/models/system/truck/wheel.osgt");
    if(!dualWheelNode.valid())
        dualWheelNode = osgDB::readRefNodeFile("../data/models/system/truck/wheel-dual.osgt");
    if(!spinerNode.valid())
        spinerNode    = osgDB::readRefNodeFile("../data/models/system/truck/truck-spiner.osgt");
    if(!holderNode.valid())
        holderNode    = osgDB::readRefNodeFile("../data/models/system/truck/truck-holder.osgt");
    //--create rockets---------------------------------------------------------------------------------------
    mRocket1 = new Rocket(mapControler,nullptr);
    mRocket1->setType(ROCKET);
    mRocket1->setQStringName(ROCKET + 1);
    mRocket1->getPositionAttitudeTransform()->setPosition(osg::Vec3d(6.8, -1.3, 0));
    mRocket1->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(-90.0),osg::Z_AXIS));

    mRocket2 = new Rocket(mapControler,nullptr);
    mRocket2->setType(ROCKET);
    mRocket2->setQStringName(ROCKET + 2);
    mRocket2->getPositionAttitudeTransform()->setPosition(osg::Vec3d(6.8, 0.0, 0));
    mRocket2->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(-90.0),osg::Z_AXIS));

    mRocket3 = new Rocket(mapControler,nullptr);
    mRocket3->setType(ROCKET);
    mRocket3->setQStringName(ROCKET + 3);
    mRocket3->getPositionAttitudeTransform()->setPosition(osg::Vec3d(6.8, 1.3, 0));
    mRocket3->getPositionAttitudeTransform()->setAttitude(osg::Quat(osg::inDegrees(-90.0),osg::Z_AXIS));
    //--create holder and add rockets to i---------------------------------------------------------------------
    mHolder = new osg::PositionAttitudeTransform();
    mHolder->setPosition(osg::Vec3d(-2, 0, 1.3));
    mHolder->addChild(holderNode);
    mHolder->addChild(mRocket1);
    mHolder->addChild(mRocket2);
    mHolder->addChild(mRocket3);
    //--create spiner and add holder to it---------------------------------------------------------------------
    mSpiner = new osg::PositionAttitudeTransform();
    mSpiner->setPosition(osg::Vec3d(-5, 0, 2.6));
    mSpiner->addChild(spinerNode);
    mSpiner->addChild(mHolder);
    //--wheel front left----------------------------------------------------------------------------------------
    osg::ref_ptr<osg::PositionAttitudeTransform> wheelTransformL = new osg::PositionAttitudeTransform();
    wheelTransformL->addChild(wheelNode);

    osg::ref_ptr<osg::PositionAttitudeTransform> wheelFl = new osg::PositionAttitudeTransform();
    wheelFl->addChild(wheelTransformL);
    wheelFl->setPosition(osg::Vec3(3.2f,1.7f,0.8f));
    wheelFl->setScale(osg::Vec3d(2, 2, 2));
    wheelFl->setAttitude(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS));
    //--wheel front right--------------------------------------------------------------------------------------
    osg::ref_ptr<osg::PositionAttitudeTransform> wheelTransformR = new osg::PositionAttitudeTransform();
    wheelTransformR->addChild(wheelNode);

    osg::ref_ptr<osg::PositionAttitudeTransform> wheelFr = new osg::PositionAttitudeTransform();
    wheelFr->addChild(wheelTransformR);
    wheelFr->setPosition(osg::Vec3(3.2f,-1.7f,0.8f));
    wheelFr->setScale(osg::Vec3d(2, 2, 2));
    wheelFr->setAttitude(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS));
    //--wheel rear left---------------------------------------------------------------------------------------
    osg::ref_ptr<osg::PositionAttitudeTransform> doualWheelTransformL = new osg::PositionAttitudeTransform();
    doualWheelTransformL->addChild(dualWheelNode);

    osg::ref_ptr<osg::PositionAttitudeTransform> wheelRl1 = new osg::PositionAttitudeTransform();
    wheelRl1->addChild(doualWheelTransformL);
    wheelRl1->setPosition(osg::Vec3(-3.7f,1.7f,0.8f));
    wheelRl1->setScale(osg::Vec3d(2, 2, 2));
    wheelRl1->setAttitude(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS));

    osg::ref_ptr<osg::PositionAttitudeTransform> wheelRl2 = new osg::PositionAttitudeTransform();
    wheelRl2->addChild(doualWheelTransformL);
    wheelRl2->setPosition(osg::Vec3(-5.7f,1.7f,0.8f));
    wheelRl2->setScale(osg::Vec3d(2, 2, 2));
    wheelRl2->setAttitude(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS));

    osg::ref_ptr<osg::PositionAttitudeTransform> doualWheelTransformR = new osg::PositionAttitudeTransform();
    doualWheelTransformR->addChild(dualWheelNode);
    //--wheel rear right--------------------------------------------------------------------------------------
    osg::ref_ptr<osg::PositionAttitudeTransform> wheelRr1 = new osg::PositionAttitudeTransform();
    wheelRr1->addChild(doualWheelTransformR);
    wheelRr1->setPosition(osg::Vec3(-3.7f,-1.7f,0.8f));
    wheelRr1->setScale(osg::Vec3d(2, 2, 2));
    wheelRr1->setAttitude(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS));

    osg::ref_ptr<osg::PositionAttitudeTransform> wheelRr2 = new osg::PositionAttitudeTransform();
    wheelRr2->addChild(doualWheelTransformR);
    wheelRr2->setPosition(osg::Vec3(-5.7f,-1.7f,0.8f));
    wheelRr2->setScale(osg::Vec3d(2, 2, 2));
    wheelRr2->setAttitude(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS));
    //--create truck group-------------------------------------------------------------------------------------
    osg::ref_ptr<osg::Group> truckGroup = new osg::Group();
    truckGroup->addChild(truckNode);
    truckGroup->addChild(mSpiner);
    truckGroup->addChild(wheelFl);
    truckGroup->addChild(wheelFr);
    truckGroup->addChild(wheelRl1);
    truckGroup->addChild(wheelRr1);
    truckGroup->addChild(wheelRl2);
    truckGroup->addChild(wheelRr2);
    //--create animation for wheels----------------------------------------------------------------------------
    osg::ref_ptr<osg::AnimationPath> leftWheelAnimPath  = new osg::AnimationPath;
    leftWheelAnimPath->setLoopMode(osg::AnimationPath::LoopMode::LOOP);
    leftWheelAnimPath->insert(0, osg::AnimationPath::ControlPoint());
    leftWheelAnimPath->insert(1, osg::AnimationPath::ControlPoint(osg::Vec3d(), osg::Quat(osg::inDegrees(180.0), osg::Y_AXIS)));
    leftWheelAnimPath->insert(2, osg::AnimationPath::ControlPoint(osg::Vec3d(), osg::Quat(osg::inDegrees(359.99), osg::Y_AXIS)));

    osg::ref_ptr<osg::AnimationPath> rightWheelAnimPath = new osg::AnimationPath;
    rightWheelAnimPath->setLoopMode(osg::AnimationPath::LoopMode::LOOP);
    rightWheelAnimPath->insert(0, osg::AnimationPath::ControlPoint());
    rightWheelAnimPath->insert(1, osg::AnimationPath::ControlPoint(osg::Vec3d(), osg::Quat(osg::inDegrees(-180.0), osg::Y_AXIS)));
    rightWheelAnimPath->insert(2, osg::AnimationPath::ControlPoint(osg::Vec3d(), osg::Quat(osg::inDegrees(-359.99), osg::Y_AXIS)));

    mLeftWheelUpdateCallback  = new osg::AnimationPathCallback();
    mLeftWheelUpdateCallback->setAnimationPath(leftWheelAnimPath);

    mRightWheelUpdateCallback = new osg::AnimationPathCallback();
    mRightWheelUpdateCallback->setAnimationPath(rightWheelAnimPath);

    wheelTransformL->setUpdateCallback(mLeftWheelUpdateCallback);
    doualWheelTransformL->setUpdateCallback(mLeftWheelUpdateCallback);

    wheelTransformR->setUpdateCallback(mRightWheelUpdateCallback);
    doualWheelTransformR->setUpdateCallback(mRightWheelUpdateCallback);

    mLeftWheelUpdateCallback->setPause(true);
    mRightWheelUpdateCallback->setPause(true);
    //--create animation for truck----------------------------------------------------------------------------
    mMoveAnimationPathCallback = new ModelAnimationPathCallback();
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
    mMoveAnimationPathCallback->setAnimationPath(path);
    setUpdateCallback(mMoveAnimationPathCallback);
    //--create style------------------------------------------------------------------------------------------
    //mRootNode = new osg::LOD;
    osgEarth::Symbology::Style  style;
    //    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(truckGroup);
    setStyle(style);

    //mRootNode->addChild(truckGroup, 0, std::numeric_limits<float>::max());

}

void Truck::moveTo(const osg::Vec3d &pos, double speed)
{
    mMoveAnimationPathCallback->getAnimationPath()->clear();
    mLeftWheelUpdateCallback->setPause(false);
    mRightWheelUpdateCallback->setPause(false);

    osg::Vec3d currentWPoint;
    mParent->getPosition().toWorld(currentWPoint);

    osg::Vec3d wPos;
    osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), pos).toWorld(wPos);


    osg::Vec3d wDef = wPos - currentWPoint;
    double distance = wDef.normalize();
    //transfer def vector to local----------------------------------------
    osg::Matrixd localTransfer;
    mParent->getPosition().createWorldToLocal(localTransfer);
    osg::Quat localRotation;
    localRotation = localTransfer.getRotate();
    osg::Matrixd rotateTransfer = osg::Matrixd::rotate(localRotation);
    osg::Vec3f localDef =  wDef * rotateTransfer;
    //-------------------------------------------------------------------

    osg::Quat rotate;
    rotate.makeRotate(osg::X_AXIS, localDef);
    double t = distance / speed;

    mMoveAnimationPathCallback->getAnimationPath()->insert(0, osg::AnimationPath::ControlPoint(currentWPoint, rotate, getScale()));
    mMoveAnimationPathCallback->getAnimationPath()->insert(t,osg::AnimationPath::ControlPoint(wPos,rotate, getScale()));

}

void Truck::aimTarget(const osg::Vec3d &pos)
{
    osg::Vec3d currentWPoint;
    mParent->getPosition().toWorld(currentWPoint);

    osg::Vec3d wPos;
    osgEarth::GeoPoint(getMapNode()->getMapSRS(), pos).toWorld(wPos);

    osg::Vec3d wDef = wPos - currentWPoint;
    //--transfer def vector to local----------------------------------------
    osg::Matrixd localTransfer;
    mParent->getPosition().createWorldToLocal(localTransfer);
    osg::Quat localRotation;
    localRotation = localTransfer.getRotate();
    osg::Matrixd rotateTransfer = osg::Matrixd::rotate(localRotation);
    osg::Vec3f localDef =  wDef * rotateTransfer;
    //--set spiner rotation-------------------------------------------------------------------
    //    currentSpinPos += _wholeTruckTransform->getMatrix().getTrans();
    //    currentSpinPos += _spinerTransform->getMatrix().getTrans();
    osg::Quat truckRot = getPositionAttitudeTransform()->getAttitude();
    osg::Matrixd truckRotMat = osg::Matrixd::rotate(truckRot);
    osg::Quat rotateSpiner;
    rotateSpiner.makeRotate(osg::X_AXIS * truckRotMat , osg::Vec3f(localDef.x(),localDef.y(), 0));
    mSpiner->setAttitude(rotateSpiner);
    //--set holder rotation--------------------------------------------------------------------
    osg::Vec3f axisH;
    axisH.x() = std::sqrt(std::pow(localDef.x(), 2.0f) + std::pow(localDef.y(),2.0f));
    axisH.y() = 0;
    axisH.z() = localDef.z();
    osg::Quat rotateHolder;
    rotateHolder.makeRotate(osg::X_AXIS, axisH);
    mHolder->setAttitude(rotateHolder);
}

bool Truck::shoot(const osg::Vec3d &pos, double speed)
{
    aimTarget(pos);
    switch(mRocketIndex) {
    case 1:
    {
        osgEarth::GeoPoint rocketPosition;
        rocketPosition.fromWorld(getMapNode()->getMapSRS(), computeRocketWorldPosition(mRocket1));
        mRocket1->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0, 0, 0));
        mRocket1->setPosition(rocketPosition);

        mHolder->removeChild(mRocket1);
        getMapNode()->addChild(mRocket1);

        mRocket1->shoot(pos, speed);
        break;
    }
    case 2:
    {
        osgEarth::GeoPoint rocketPosition;
        rocketPosition.fromWorld(getMapNode()->getMapSRS(), computeRocketWorldPosition(mRocket2));
        mRocket2->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0, 0, 0));
        mRocket2->setPosition(rocketPosition);

        mHolder->removeChild(mRocket2);
        getMapNode()->addChild(mRocket2);
        mRocket2->shoot(pos, speed);
        break;
    }
    case 3:
    {
        osgEarth::GeoPoint rocketPosition;
        rocketPosition.fromWorld(getMapNode()->getMapSRS(), computeRocketWorldPosition(mRocket3));
        mRocket3->getPositionAttitudeTransform()->setPosition(osg::Vec3d(0, 0, 0));
        mRocket3->setPosition(rocketPosition);
//        auto er = getPosition();
//        mRocket3->setPosition(getPosition());

        mHolder->removeChild(mRocket3);
        getMapNode()->addChild(mRocket3);

        mRocket3->shoot(pos, speed);
        break;
    }
    default:
        return false;
    }
    mRocketIndex -= 1;
    return true;
}

Rocket *Truck::getActiveRocket() const
{
    switch(mRocketIndex) {
    case 1:
        return mRocket1;
        break;
    case 2:
        return mRocket2;
        break;

    case 3:
        return mRocket3;
        break;
    default:
        return nullptr;
    }
}

bool Truck::hasRocket()
{
    if(mRocketIndex < 1)
        return false;
    return true;
}

//void Truck::stop()
//{
//    mLeftWheelUpdateCallback->setPause(true);
//    mRightWheelUpdateCallback->setPause(true);
//}

osg::Vec3d Truck::computeRocketWorldPosition(Rocket *rocket)
{
    osg::Vec3d localPosition = rocket->getPositionAttitudeTransform()->getPosition();
    osg::Matrix toHolderMatrix;
    mHolder->computeLocalToWorldMatrix(toHolderMatrix,mHolder->asNodeVisitor());
    osg::Matrix toSpinnerMatrix;
    mSpiner->computeLocalToWorldMatrix(toSpinnerMatrix,mSpiner->asNodeVisitor());
    osg::Matrix toTruckMatrix;
    getPositionAttitudeTransform()->computeLocalToWorldMatrix(toTruckMatrix, mSpiner->asNodeVisitor());
    osg::Matrix toWorldMatrix;
    mParent->getPosition().createLocalToWorld(toWorldMatrix);
    osg::Vec3d worldPosition = localPosition * toHolderMatrix * toSpinnerMatrix * toTruckMatrix * toWorldMatrix;
    //draw line for debuge------------------------------------------------
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(worldPosition));
    return worldPosition;
}

