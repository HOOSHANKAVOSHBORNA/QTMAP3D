#include "MoveableModel.h"
#include "mapItem.h"
#include <QObject>
#include <osg/Geode>
#include <osg/Material>
#include <osg/ShapeDrawable>
#include <osgAnimation/Sampler>
#include <osgEarth/GeoMath>
#include <osgEarth/Viewpoint>
#include "modelAutoScaler.h"

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
                auto terrain = mMapItem->getMapNode()->getTerrain();
                terrain->getHeight(mMapItem->getMapNode()->getMapSRS(),geoPoint.x(),geoPoint.y(),height);
                geoPoint.z() = *height;
//                if (geoPointPre.isValid()){


//                }
//                geoPointPre = geoPoint;
                mMoveableModel->setPosition(geoPoint);
                mMoveableModel->getPositionAttitudeTransform()->setScale(cp.getScale());
                if(mMoveableModel->mIs3D){
//                    mMoveableModel->getPositionAttitudeTransform()->setAttitude(cp.getRotation());
                    double angle;
                    osg::Vec3 vec;
                    cp.getRotation().getRotate(angle, vec);
                    //vec.z() = 0;
                    vec.y() = 0;
                    //vec.x() = 0;
                    mMoveableModel->getPositionAttitudeTransform()->setAttitude(osg::Quat(angle, vec));
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

            if(mMoveableModel && (_latestTime - _firstTime) > _animationPath->getPeriod())
                mMoveableModel->stop();
        }
    }
    NodeCallback::traverse(node,nv);
}
ModelAnimationPathCallback::ModelAnimationPathCallback(MapItem *mapControler, MoveableModel *moveableModel)
{
    mMoveableModel = moveableModel;
    mMapItem = mapControler;
}
//class AnimtkUpdateCallback : public osg::NodeCallback
//{
//private:
//    osg::ref_ptr<LineNode> mTestLine;
//    MoveableModel* mMoveableModel;
//public:

//    AnimtkUpdateCallback(MapItem* mapControler, MoveableModel* moveableModel)
//    {
//        mMoveableModel = moveableModel;
//        _sampler = new osgAnimation::Vec3CubicBezierSampler;
//        _playing = false;
//        _lastUpdate = 0;
//        mTestLine = new LineNode(mapControler);

//    }
//    AnimtkUpdateCallback(const AnimtkUpdateCallback& val, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY):
//        osg::Object(val, copyop),
//        osg::Callback(val, copyop),
//        osg::NodeCallback(val, copyop),
//        _sampler(val._sampler),
//        _startTime(val._startTime),
//        _currentTime(val._currentTime),
//        _playing(val._playing),
//        _lastUpdate(val._lastUpdate)

//    {
//    }

//    /** Callback method called by the NodeVisitor when visiting a node.*/
//    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
//    {
//        if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR &&
//            nv->getFrameStamp() &&
//            nv->getFrameStamp()->getFrameNumber() != _lastUpdate)
//        {

//            _lastUpdate = nv->getFrameStamp()->getFrameNumber();
//            _currentTime = osg::Timer::instance()->tick();

//            if (_playing && _sampler.get() && _sampler->getKeyframeContainer())
//            {
////                MoveableModel* changiz = dynamic_cast<MoveableModel*>(node);;
////                osg::PositionAttitudeTransform* transform = dynamic_cast<osg::PositionAttitudeTransform*>(node);
////                changiz = dynamic_cast<MoveableModel*>(transform);
//                if (mMoveableModel) {
//                    osg::Vec3 result;
//                    osgEarth::GeoPoint geoPoint;
//                    float t = osg::Timer::instance()->delta_s(_startTime, _currentTime);
//                    float duration = _sampler->getEndTime() - _sampler->getStartTime();
//                    t = fmod(t, duration);
//                    t += _sampler->getStartTime();
//                    _sampler->getValueAt(t, result);
//                    geoPoint.fromWorld(mMoveableModel->getMapNode()->getMapSRS(), result);

//                    mTestLine->addPoint(geoPoint);
//                    mMoveableModel->addChild(mTestLine);
//                    mMoveableModel->setPosition(geoPoint);
//                    double angleX;
//                    double angleY;
//                    double angleZ;
//                    double angle;
//                    mMoveableModel->getPositionAttitudeTransform()->getAttitude().getRotate(angle, angleX, angleY, angleZ);
//                    //qDebug()<< "angle: "<< angle<< "angleX: "<< angleX<<"angleY: "<< angleY<<"angleZ: "<< angleZ;
//                    osg::Vec3d rotVec =    result - lastPosition;
//                    osg::Vec3d headVec = osg::Vec3d(0,-1,0);
//                    //                    std::cout << rotVec << std::endl;
//                    rotVec = rotVec/rotVec.length();
//                    //                    if(rotVec.z() > 0.001){
//                    //                        rotVec.z() = 0.0009;
//                    //                    }
//                    //                    std::cout << rotVec << std::endl;
//                    osg::Quat headingRotate;
//                    //                    headingRotate.makeRotate(osg::inDegrees(), headVec);
//                    //                    rotVec.y() = 0;
//                    headingRotate.makeRotate(headVec, rotVec);
//                    //changiz->getPositionAttitudeTransform()->setAttitude(headingRotate);
//                    //                    std::cout << result << "   "<<lastPosition<< "    "<< rotVec <<std::endl;
//                    //                    transform->setMatrix(osg::Matrix::translate(result));
//                    lastPosition = result;
//                }
//            }
//        }
//        // note, callback is responsible for scenegraph traversal so
//        // they must call traverse(node,nv) to ensure that the
//        // scene graph subtree (and associated callbacks) are traversed.
//        traverse(node,nv);
//    }

//    void start() { _startTime = osg::Timer::instance()->tick(); _currentTime = _startTime; _playing = true;}
//    void stop() { _currentTime = _startTime; _playing = false;}

//    osg::ref_ptr<osgAnimation::Vec3CubicBezierSampler> _sampler;
//    osg::Timer_t _startTime;
//    osg::Timer_t _currentTime;
//    bool _playing;
//    unsigned int _lastUpdate;
//    osg::Vec3 lastPosition;
//    osg::ref_ptr<osg::Geode> _geode;
//};

MoveableModel::MoveableModel(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent):
    simpleModelNode(mapControler, modelUrl, iconUrl, parent)
{

    mMoveAnimationPathCallback = new ModelAnimationPathCallback(mapItem(), this);
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
    mMoveAnimationPathCallback->setAnimationPath(path);
    getGeoTransform()->addUpdateCallback(mMoveAnimationPathCallback);

//getGeoTransform()->setUpdateCallback(callback);

}

void MoveableModel::moveTo(osgEarth::GeoPoint destinationPoint, double mSpeed)
{
    //mMoveAnimationPathCallback->getAnimationPath()->clear();
    osg::Vec3d currentWPoint;
    getPosition().toWorld(currentWPoint);
    osg::Vec3d wDesPos;
    osgEarth::GeoPoint(mapItem()->getMapSRS(), destinationPoint).toWorld(wDesPos);
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

    osg::Quat rotate;
    rotate.makeRotate(-(osg::Y_AXIS), localDef);
    double t = distance / mSpeed;


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
    getGeoTransform()->setAutoRecomputeHeights(true);

    mMoveAnimationPathCallback->getAnimationPath()->insert(0, osg::AnimationPath::ControlPoint(currentWPoint, rotate, osg::Vec3d(10,10,10)));
//    mMoveAnimationPathCallback->getAnimationPath()->insert(2, osg::AnimationPath::ControlPoint(pI, rotate, osg::Vec3d(1,1,1)));
//    mMoveAnimationPathCallback->getAnimationPath()->insert(5, osg::AnimationPath::ControlPoint(pII, rotate, osg::Vec3d(1,1,1)));
    mMoveAnimationPathCallback->getAnimationPath()->insert(t,osg::AnimationPath::ControlPoint(wDesPos,rotate, osg::Vec3d(10,10,10)));
    getGeoTransform()->setUpdateCallback(mMoveAnimationPathCallback);

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

