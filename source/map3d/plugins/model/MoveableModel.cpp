#include "MoveableModel.h"
#include "mapItem.h"
#include <QObject>

MoveableModel::MoveableModel(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent):
    simpleModelNode(mapControler, modelUrl, iconUrl, parent)
{
    mMoveAnimationPathCallback = new osg::AnimationPathCallback();
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
    mMoveAnimationPathCallback->setAnimationPath(path);
    getGeoTransform()->addUpdateCallback(mMoveAnimationPathCallback);
}

void MoveableModel::moveTo(osgEarth::GeoPoint destinationPoint, double mSpeed)
{
    mMoveAnimationPathCallback->getAnimationPath()->clear();
    osg::Vec3d currentWPoint;
    getPosition().toWorld(currentWPoint);
    osg::Vec3d wDesPos;
    osgEarth::GeoPoint(osgEarth::SpatialReference::get("wgs84"), destinationPoint).toWorld(wDesPos);
    osg::Vec3d wDef = wDesPos - currentWPoint;
    double distance = wDef.normalize();
    //transfer def vector to local----------------------------------------
    osg::Matrixd localTransfer;
    getPosition().createWorldToLocal(localTransfer);
    osg::Quat localRotation;
    localRotation = localTransfer.getRotate();
    osg::Matrixd rotateTransfer = osg::Matrixd::rotate(localRotation);
    osg::Vec3f localDef =  wDef * rotateTransfer;
    //-------------------------------------------------------------------

    osg::Quat rotate;
    rotate.makeRotate(osg::X_AXIS, localDef);
    double t = distance / mSpeed;

    mMoveAnimationPathCallback->getAnimationPath()->insert(0, osg::AnimationPath::ControlPoint(currentWPoint, rotate));
    mMoveAnimationPathCallback->getAnimationPath()->insert(t,osg::AnimationPath::ControlPoint(wDesPos,rotate));
}

simpleModelNode *MoveableModel::getNewModel()
{
    return new MoveableModel(mapItem(), modelUrl(), iconUrl());
}

double MoveableModel::speed() const
{
    return mSpeed;
}

void MoveableModel::setSpeed(double newSpeed)
{
    mSpeed = newSpeed;
}
