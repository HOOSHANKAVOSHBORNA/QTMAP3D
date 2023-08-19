#include "MoveableModel.h"
#include "mapItem.h"
#include <QObject>
#include <osg/Geode>
#include <osg/Material>
#include <osg/ShapeDrawable>
#include <osgAnimation/Sampler>
#include <osgEarth/GeoMath>
#include <osgEarth/Viewpoint>


ModelAnimationPathCallback::ModelAnimationPathCallback(MapItem *mapControler, MoveableModel *moveableModel)
{
    mMoveableModel = moveableModel;
    mMapItem = mapControler;
}

void ModelAnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    osgEarth::GeoPoint geoPoint;
//    osgEarth::GeoPoint geoPointPre;

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
                geoPoint.fromWorld(mMapItem->getMapSRS(), cp.getPosition());
                double *height;
                const osgEarth::Terrain* terrain = mMapItem->getMapNode()->getTerrain();
                terrain->getHeight(mMapItem->getMapNode()->getMapSRS(),geoPoint.x(),geoPoint.y(),height);
                geoPoint.z() = *height;
                mMoveableModel->setPosition(geoPoint);
                mMoveableModel->getPositionAttitudeTransform()->setScale(cp.getScale());
                if(mMoveableModel->mIs3D){
                    osg::Vec3d geoPointW;
                    geoPoint.toWorld(geoPointW);
                    osg::Vec3d preGeoPointW;
                    mPreGeoPoint.toWorld(preGeoPointW);
                    osg::Vec3d slopeVec = geoPointW - preGeoPointW;
                    slopeVec.normalize();

//                    slopeVec.z() = 0;
                    //transfer def vector to local----------------------------------------
                    osg::Matrixd localTransfer;
                    mMoveableModel->getPosition().createWorldToLocal(localTransfer);
                    osg::Quat localRotation;
                    localRotation = localTransfer.getRotate();
                    osg::Matrixd rotateTransfer = osg::Matrixd::rotate(localRotation);
                    osg::Vec3f localDef =  slopeVec * rotateTransfer;
                    //-------------------------------------------------------------------
                    osg::Quat rotate1;
                    rotate1.makeRotate(-(osg::X_AXIS), localDef);
                    double angle;
                    osg::Vec3 vec;
                    rotate1.getRotate(angle, vec);
                    osg::Quat rotate2 = osg::Quat(0, osg::Z_AXIS);

                    if(angle > osg::PI/2.0 ){
//                        qDebug()<<"angle: "<<angle;
                        rotate1.makeRotate(-(osg::Y_AXIS), localDef);
                        rotate2.makeRotate(osg::PI/2.0, osg::Z_AXIS);
                    }

                    osg::Quat rotate = rotate1 * rotate2;
                    mMoveableModel->getPositionAttitudeTransform()->setAttitude(rotate);

                    mPreGeoPoint = geoPoint;

                }
                else
                {
                    double angle;
                    osg::Vec3 vec;
                    cp.getRotation().getRotate(angle, vec);
                    vec.x() = 0;
                    vec.y() = 0;
                    mMoveableModel->getPositionAttitudeTransform()->setAttitude(osg::Quat(angle, vec));
                }
            }

            if(mMoveableModel && (_latestTime - _firstTime) > _animationPath->getPeriod()){
//                mMoveableModel->stop();
                setPause(true);
            }
        }
    }
    NodeCallback::traverse(node,nv);
}

MoveableModel::MoveableModel(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent):
    simpleModelNode(mapControler, modelUrl, iconUrl, parent)
{

    mMoveAnimationPathCallback = new ModelAnimationPathCallback(mapItem(), this);
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
    mMoveAnimationPathCallback->setAnimationPath(path);
    getGeoTransform()->addUpdateCallback(mMoveAnimationPathCallback);
}

void MoveableModel::moveTo(osgEarth::GeoPoint destinationPoint, double mSpeed)
{
    mMoveAnimationPathCallback->reset();
    mMoveAnimationPathCallback->getAnimationPath()->clear();
    mMoveAnimationPathCallback->setPause(false);

    osg::Vec3d currentWPoint;
    getPosition().toWorld(currentWPoint);
    osg::Vec3d destinationWPoint;
    destinationPoint.toWorld(destinationWPoint);

    double distance = getPosition().distanceTo(destinationPoint);
    double t = distance / mSpeed;

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

