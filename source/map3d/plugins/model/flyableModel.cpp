#include "flyableModel.h"
#include <QRandomGenerator>

FmodelAnimationPathCallback::FmodelAnimationPathCallback(MapItem *mapItem, FlyableModel *flyableModel)
{
    mFlyableModel = flyableModel;
    mMapItem = mapItem;
}

void FmodelAnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    if (_animationPath.valid() &&
        nv->getVisitorType()== osg::NodeVisitor::UPDATE_VISITOR &&
        nv->getFrameStamp())
    {
        double time = nv->getFrameStamp()->getSimulationTime();
        _latestTime = time;
        if (!_pause)
        {
            if (_firstTime >= DBL_MAX) _firstTime = time;
            //------------------------------------------------------------------------------------------------
            osg::AnimationPath::ControlPoint cp;
            double animatTime = getAnimationTime();
            if (getAnimationPath()->getInterpolatedControlPoint(animatTime,cp))
            {
                osgEarth::GeoPoint currentGeoPoint;
                currentGeoPoint.fromWorld(mMapItem->getMapSRS(), cp.getPosition());
                //--terrain-----------------------------------------------------------------------
//                double height = 0;
//                const osgEarth::Terrain* terrain = mMapItem->getMapNode()->getTerrain();
//                terrain->getHeight(mMapItem->getMapNode()->getMapSRS(), currentGeoPoint.x(), currentGeoPoint.y(), &height);
//                double randomHeight = 200 + (QRandomGenerator::global()->generate() % (300 - 200));
//                currentGeoPoint.z() = randomHeight;
                //--set Position-----------------------------------------------------
                mFlyableModel->setPosition(currentGeoPoint);
                //--compute and set rotation-----------------------------------------------
                {
                    osg::Vec3d preWorldPoint;
                    mPreGeoPoint.toWorld(preWorldPoint);
                    osg::Vec3d motionVec = cp.getPosition() - preWorldPoint;
                    motionVec.normalize();
                    //transfer motion vector to object coordination----------------------
                    osg::Matrixd localTransfer;
                    mFlyableModel->getPosition().createWorldToLocal(localTransfer);
                    osg::Quat localRotation;
                    localRotation = localTransfer.getRotate();
                    osg::Matrixd rotateTransfer = osg::Matrixd::rotate(localRotation);
                    osg::Vec3f localMotionVec =  motionVec * rotateTransfer;
                    osg::Vec3f localMotionVecProject;
                    localMotionVecProject.set(localMotionVec);
                    localMotionVecProject.z() = 0;
                    //-------------------------------------------------------------------
                    osg::Vec3f planeAxis = (osg::Y_AXIS);
                    osg::Quat rotate1;
                    rotate1.makeRotate(planeAxis, localMotionVecProject);

                    osg::Quat rotate2;
                    localMotionVec = localMotionVec *  osg::Matrixd::rotate(rotate1);
                    localMotionVecProject = localMotionVecProject *  osg::Matrixd::rotate(rotate1);
                    rotate2.makeRotate(localMotionVecProject, localMotionVec);
                    //-----------------------------------------------------------------
                    osg::Quat rotate = rotate2 * rotate1;
                    mFlyableModel->getPositionAttitudeTransform()->setAttitude(rotate);
                }
                mPreGeoPoint = currentGeoPoint;
            }

            if(mFlyableModel && (_latestTime - _firstTime) > _animationPath->getPeriod()){
                setPause(true);
            }
        }
    }
    NodeCallback::traverse(node,nv);
}

FlyableModel::FlyableModel(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent):
    MoveableModel(mapControler, modelUrl, iconUrl, parent)
{
    mMoveAnimationPathCallback = new FmodelAnimationPathCallback(mapItem(), this);
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
    mMoveAnimationPathCallback->setAnimationPath(path);
    getGeoTransform()->addUpdateCallback(mMoveAnimationPathCallback);
}

void FlyableModel::flyTo(osgEarth::GeoPoint destinationPoint, double velocity)
{
    double randomHeight = 50 + (QRandomGenerator::global()->generate() % (600 - 50));
    destinationPoint.z() = randomHeight;
    mMoveAnimationPathCallback->reset();
    mMoveAnimationPathCallback->getAnimationPath()->clear();
    mMoveAnimationPathCallback->setPause(false);
    osg::Vec3d currentWPoint;
    getPosition().toWorld(currentWPoint);
    osg::Vec3d destinationWPoint;
    destinationPoint.toWorld(destinationWPoint);

    double distance = getPosition().distanceTo(destinationPoint);
    double t = distance / velocity;

    mMoveAnimationPathCallback->getAnimationPath()->insert(0, osg::AnimationPath::ControlPoint(currentWPoint));
    mMoveAnimationPathCallback->getAnimationPath()->insert(t, osg::AnimationPath::ControlPoint(destinationWPoint));
}

double FlyableModel::speed() const
{

}

void FlyableModel::setSpeed(double newSpeed)
{

}

SimpleModelNode *FlyableModel::getNewModel()
{
    return new FlyableModel(mapItem(), modelUrl(), iconUrl());
}
