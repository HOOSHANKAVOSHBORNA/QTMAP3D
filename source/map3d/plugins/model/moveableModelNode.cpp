#include "moveableModelNode.h"
#include "mapItem.h"
#include <QObject>
#include <osg/Geode>
#include <osg/Material>
#include <osg/ShapeDrawable>
#include <osgAnimation/Sampler>
#include <osgEarth/GeoMath>
#include <osgEarth/Viewpoint>
#include <osgEarth/ImageUtils>

//osgEarth::Util::Controls::ImageLayer

ModelAnimationPathCallback::ModelAnimationPathCallback(MapItem *mapItem, MoveableModelNode *moveableModel)
{
    mMoveableModel = moveableModel;
    mMapItem = mapItem;
}

void ModelAnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
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
                double height = 0;
                const osgEarth::Terrain* terrain = mMapItem->getMapNode()->getTerrain();
                terrain->getHeight(mMapItem->getMapNode()->getMapSRS(), currentGeoPoint.x(), currentGeoPoint.y(), &height);
                currentGeoPoint.z() = height;
                //--set Position-----------------------------------------------------
                mMoveableModel->setPosition(currentGeoPoint);
                //--compute and set rotation-----------------------------------------------
                {
                    osg::Vec3d currentWorldPoint;
                    currentGeoPoint.toWorld(currentWorldPoint);
                    osg::Vec3d preWorldPoint;
                    mPreGeoPoint.toWorld(preWorldPoint);
                    osg::Vec3d motionVec = currentWorldPoint - preWorldPoint;
                    motionVec.normalize();
                    //transfer motion vector to object coordination----------------------
                    osg::Matrixd localTransfer;
                    mMoveableModel->getPosition().createWorldToLocal(localTransfer);
                    osg::Quat localRotation;
                    localRotation = localTransfer.getRotate();
                    osg::Matrixd rotateTransfer = osg::Matrixd::rotate(localRotation);
                    osg::Vec3f localMotionVec =  motionVec * rotateTransfer;
                    osg::Vec3f localMotionVecProject;
                    localMotionVecProject.set(localMotionVec);
                    localMotionVecProject.z() = 0;
                    //-------------------------------------------------------------------
                    osg::Vec3f carAxis = osg::Y_AXIS;
                    osg::Quat rotate1;
                    rotate1.makeRotate(carAxis, localMotionVecProject);
                    osg::Quat rotate3;
                    rotate3.makeRotate(localMotionVecProject, carAxis);

                    osg::Quat rotate2;
                    localMotionVec = localMotionVec *  osg::Matrixd::rotate(rotate3);
                    localMotionVecProject = localMotionVecProject *  osg::Matrixd::rotate(rotate3);
                    rotate2.makeRotate(localMotionVecProject, localMotionVec);

                    //-----------------------------------------------------------------
//                    osg::Vec3f  sidePoint = osg::Y_AXIS * 20;
//                    osg::Matrixd worldTransfer;
//                    mMoveableModel->getPosition().createLocalToWorld(worldTransfer);
////                    osg::Quat worldRotation = worldTransfer.getRotate();
////                    osg::Matrixd rotateWorldTransfer = osg::Matrixd::rotate(worldRotation);
//                    osg::Vec3d sidePointWorld =  sidePoint * worldTransfer;
//                    osgEarth::GeoPoint sidePointGeo;
//                    sidePointGeo.fromWorld(mMapItem->getMapSRS(), sidePointWorld);
//                    terrain->getHeight(mMapItem->getMapNode()->getMapSRS(), sidePointGeo.x(), sidePointGeo.y(), &height);
//                    sidePointGeo.z() = height;
//                    sidePointGeo.toWorld(sidePointWorld);
//                    osg::Vec3d sideVec = sidePointWorld - currentWorldPoint;
//                    sideVec.normalize();
//                    osg::Vec3f localSideVec =  sideVec * rotateTransfer;
//                    localSideVec = localSideVec * osg::Matrixd::rotate(rotate2 * rotate1);
//                    osg::Vec3f yRotate = osg::Y_AXIS * osg::Matrixd::rotate(rotate2 * rotate1);
//                    osg::Quat rotate3;
//                    rotate3.makeRotate( yRotate, localSideVec);
                    //-----------------------------------------------------------------
                    osg::Quat rotate = rotate2 * rotate1;
                    mMoveableModel->getPositionAttitudeTransform()->setAttitude(rotate);
                    //--set heading----------------------------------------------------
                    double angleRad;
                    osg::Vec3 vec;
                    mMoveableModel->getPositionAttitudeTransform()->getAttitude().getRotate(angleRad, vec);
                    double angleDeg = osg::RadiansToDegrees(angleRad);
                    angleDeg = vec.z() > 0 ? -angleDeg: angleDeg;
                    mMoveableModel->set2DHeaing(angleDeg);
                }
                mPreGeoPoint = currentGeoPoint;
            }

            if(mMoveableModel && (_latestTime - _firstTime) > _animationPath->getPeriod()){
                setPause(true);
            }
        }
    }
    NodeCallback::traverse(node,nv);
}

MoveableModelNode::MoveableModelNode(QQmlEngine *engine,
                                     MapItem *mapControler,
                                     const std::string &modelUrl,
                                     const std::string &iconUrl,
                                     QObject *parent)
    : SimpleModelNode(engine, mapControler, modelUrl, iconUrl, parent)
{
    setQmlEngine(engine);
    mMoveAnimationPathCallback = new ModelAnimationPathCallback(mapItem(), this);
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
    mMoveAnimationPathCallback->setAnimationPath(path);
    getGeoTransform()->addUpdateCallback(mMoveAnimationPathCallback);
}

void MoveableModelNode::moveTo(osgEarth::GeoPoint destinationPoint, double speed)
{
    mSpeed = speed;
//    mMoveAnimationPathCallback->reset();
    mMoveAnimationPathCallback->getAnimationPath()->clear();
    mMoveAnimationPathCallback->setPause(false);
    osg::Vec3d currentWPoint;
    getPosition().toWorld(currentWPoint);
    osg::Vec3d destinationWPoint;
    destinationPoint.toWorld(destinationWPoint);

    double distance = getPosition().distanceTo(destinationPoint);
    double t = distance / speed;

    mMoveAnimationPathCallback->getAnimationPath()->insert(0, osg::AnimationPath::ControlPoint(currentWPoint));
    mMoveAnimationPathCallback->getAnimationPath()->insert(t, osg::AnimationPath::ControlPoint(destinationWPoint));


    //    osg::Vec3d objectDirection  = osg::Vec3d(0,1,0);
    //    qDebug()<<"lenght of model: "<<getMlenght();
    //    double objectLength = getMlenght();

    //    osg::Vec3d pathDirectionVector = wDef / wDef.length();

    //    osg::Vec3d middle_vector = (pathDirectionVector + objectDirection) / (sqrt(1 - (pathDirectionVector * objectDirection)));
    //    osg::Vec3d point2_Direction = (middle_vector + objectDirection) / (sqrt(abs(1 - ((middle_vector/middle_vector.length()) * objectDirection))));
    //    double angle = acos(pathDirectionVector*objectDirection) * 180 / 3.14;

    //    ////////////////////////////////////////////////////////////////
    //    osg::Vec3d point1 ;  //osg::Vec3d(50,50,2); // start point of making arc
    //    point1.x() = currentWPoint.x() + 1*objectLength*objectDirection.x();
    //    point1.y() = currentWPoint.y() + 1*objectLength*objectDirection.y();
    //    point1.z() = currentWPoint.z() /*+ 1*objectLength*objectDirection.z()*/;

    //    ////////////////////////////////////////////////////////////////
    //    osg::Vec3d point3;
    //    point3.x() = wDesPos.x() ;
    //    point3.y() = wDesPos.y() ;
    //    point3.z() = wDesPos.z();
    //    ////////////////////////////////////////////////////////////////
    //    osg::Vec3d point2;
    //    osg::Vec3d rotototo = objectDirection - pathDirectionVector;

    //    point2.x() = currentWPoint.x() + 10*objectLength * rotototo.x() - 10*objectDirection.x();
    //    point2.y() = currentWPoint.y() /*+ 10*objectLength * rotototo.y() - 10*objectDirection.y()*/;
    //    point2.z() = currentWPoint.z() ;


    //    if(165 < angle && angle < 195){
    //        point2.x() = currentWPoint.x() + 10*objectLength*objectDirection.x() ;
    //        point2.y() = currentWPoint.y() + 10*objectLength ;
    //    }




    //    mMoveAnimationPathCallback->getAnimationPath()->insert(0, osg::AnimationPath::ControlPoint(currentWPoint, rotate));
    //    mMoveAnimationPathCallback->getAnimationPath()->insert(t,osg::AnimationPath::ControlPoint(wDesPos,rotate));
    //    osg::PositionAttitudeTransform* node = new osg::PositionAttitudeTransform();
    //    osg::ref_ptr<AnimtkUpdateCallback> callback = new AnimtkUpdateCallback(mapItem(), this);
    //    osg::ref_ptr<osgAnimation::Vec3CubicBezierKeyframeContainer> keys;
    //    keys = callback->_sampler->getOrCreateKeyframeContainer();

    ////    keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(0, osgAnimation::Vec3CubicBezier(
    ////                                                                 currentWPoint
    ////                                                                 )));

    //    keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(0, osgAnimation::Vec3CubicBezier(
    //                                                                 currentWPoint,point2,point3
    //                                                                 )));

    //    keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(5, osgAnimation::Vec3CubicBezier(
    //                                                                 wDesPos
    //                                                                 )));
    //    callback->start();


    //    getPositionAttitudeTransform()->setUpdateCallback(callback);
    //    setUpdateCallback(callback);


    //    node->setUpdateCallback(callback);
    //    node->addChild(getGeoTransform());

    //getGeoTransform()->setTerrain(mapItem()->getMapNode()->getTerrain()->getHeight());
    //    getGeoTransform()->setAutoRecomputeHeights(true);
}

double MoveableModelNode::speed() const
{
    return mSpeed;
}

void MoveableModelNode::setSpeed(double newSpeed)
{
    mSpeed = newSpeed;
}

