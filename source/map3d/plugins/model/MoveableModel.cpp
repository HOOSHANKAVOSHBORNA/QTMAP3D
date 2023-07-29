#include "MoveableModel.h"
#include "mapItem.h"
#include <QObject>
#include <osg/Geode>
#include <osg/Material>
#include <osg/ShapeDrawable>
#include <osgAnimation/Sampler>
#include <osgEarth/GeoMath>


class AnimtkUpdateCallback : public osg::NodeCallback
{
public:
    META_Object(osgAnimation, AnimtkUpdateCallback);

    AnimtkUpdateCallback()
    {
        _sampler = new osgAnimation::Vec3CubicBezierSampler;
        _playing = false;
        _lastUpdate = 0;
    }
    AnimtkUpdateCallback(const AnimtkUpdateCallback& val, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY):
        osg::Object(val, copyop),
        osg::Callback(val, copyop),
        osg::NodeCallback(val, copyop),
        _sampler(val._sampler),
        _startTime(val._startTime),
        _currentTime(val._currentTime),
        _playing(val._playing),
        _lastUpdate(val._lastUpdate)
    {
    }

    /** Callback method called by the NodeVisitor when visiting a node.*/
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR &&
            nv->getFrameStamp() &&
            nv->getFrameStamp()->getFrameNumber() != _lastUpdate)
        {

            _lastUpdate = nv->getFrameStamp()->getFrameNumber();
            _currentTime = osg::Timer::instance()->tick();

            if (_playing && _sampler.get() && _sampler->getKeyframeContainer())
            {
                osg::MatrixTransform* transform = dynamic_cast<osg::MatrixTransform*>(node);
                if (transform) {
                    osg::Vec3 result;
                    float t = osg::Timer::instance()->delta_s(_startTime, _currentTime);
                    float duration = _sampler->getEndTime() - _sampler->getStartTime();
                    t = fmod(t, duration);
                    t += _sampler->getStartTime();
                    _sampler->getValueAt(t, result);
                    transform->setMatrix(osg::Matrix::translate(result));
                }
            }
        }
        // note, callback is responsible for scenegraph traversal so
        // they must call traverse(node,nv) to ensure that the
        // scene graph subtree (and associated callbacks) are traversed.
        traverse(node,nv);
    }

    void start() { _startTime = osg::Timer::instance()->tick(); _currentTime = _startTime; _playing = true;}
    void stop() { _currentTime = _startTime; _playing = false;}

    osg::ref_ptr<osgAnimation::Vec3CubicBezierSampler> _sampler;
    osg::Timer_t _startTime;
    osg::Timer_t _currentTime;
    bool _playing;
    unsigned int _lastUpdate;
};


class AnimtkStateSetUpdateCallback : public osg::StateSet::Callback
{
public:
    META_Object(osgAnimation, AnimtkStateSetUpdateCallback);

    AnimtkStateSetUpdateCallback()
    {
        _sampler = new osgAnimation::Vec4LinearSampler;
        _playing = false;
        _lastUpdate = 0;
    }

    AnimtkStateSetUpdateCallback(const AnimtkStateSetUpdateCallback& val, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY):
        osg::Object(val, copyop),
        osg::Callback(val, copyop),
        osg::StateSet::Callback(val, copyop),
        _sampler(val._sampler),
        _startTime(val._startTime),
        _currentTime(val._currentTime),
        _playing(val._playing),
        _lastUpdate(val._lastUpdate)
    {
    }

    /** Callback method called by the NodeVisitor when visiting a node.*/
    virtual void operator()(osg::StateSet* state, osg::NodeVisitor* nv)
    {
        if (state &&
            nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR &&
            nv->getFrameStamp() &&
            nv->getFrameStamp()->getFrameNumber() != _lastUpdate) {

            _lastUpdate = nv->getFrameStamp()->getFrameNumber();
            _currentTime = osg::Timer::instance()->tick();

            if (_playing && _sampler.get() && _sampler->getKeyframeContainer())
            {
                osg::Material* material = dynamic_cast<osg::Material*>(state->getAttribute(osg::StateAttribute::MATERIAL));
                if (material)
                {
                    osg::Vec4 result;
                    float t = osg::Timer::instance()->delta_s(_startTime, _currentTime);
                    float duration = _sampler->getEndTime() - _sampler->getStartTime();
                    t = fmod(t, duration);
                    t += _sampler->getStartTime();
                    _sampler->getValueAt(t, result);
                    material->setDiffuse(osg::Material::FRONT_AND_BACK, result);
                }
            }
        }
    }

    void start() { _startTime = osg::Timer::instance()->tick(); _currentTime = _startTime; _playing = true;}
    void stop() { _currentTime = _startTime; _playing = false;}

    osg::ref_ptr<osgAnimation::Vec4LinearSampler> _sampler;
    osg::Timer_t _startTime;
    osg::Timer_t _currentTime;
    bool _playing;
    unsigned int _lastUpdate;
};

// This won't really give good results in any situation, but it does demonstrate
// on possible "fast" usage...
class MakePathTimeCallback: public AnimtkUpdateCallback
{
    float _lastAdd;
    float _addSeconds;

public:
    MakePathTimeCallback():
        _lastAdd(0.0f),
        _addSeconds(0.08f) {
    }

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        float t = osg::Timer::instance()->delta_s(_startTime, _currentTime);

        if(_lastAdd + _addSeconds <= t && t <= 8.0f)
        {
            osg::Vec3 pos;

            _sampler->getValueAt(t, pos);
            _lastAdd += _addSeconds;
        }

        AnimtkUpdateCallback::operator()(node, nv);
    }
};

// This will give great results if you DO NOT have VSYNC enabled and can generate
// decent FPS.
class MakePathDistanceCallback: public AnimtkUpdateCallback
{
    osg::Vec3 _lastAdd;
    float _threshold;
    unsigned int _count;

public:
    MakePathDistanceCallback():
        _threshold(0.5f),
        _count(0) {}

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv)
    {
        static bool countReported = false;

        float t = osg::Timer::instance()->delta_s(_startTime, _currentTime);

        osg::Vec3 pos;

        _sampler->getValueAt(t, pos);

        osg::Vec3 distance = _lastAdd - pos;

        if(t <= 15.0f && distance.length() >= _threshold)
        {
            _lastAdd = pos;
            _count++;
        }
        else if(t > 15.0f)
        {
            if(!countReported) std::cout << "Created " << _count << " nodes." << std::endl;
            countReported = true;
        }

        AnimtkUpdateCallback::operator()(node, nv);
    }
};

//osg::StateSet* setupStateSet()
//{
//    osg::StateSet* st = new osg::StateSet();

//    st->setAttributeAndModes(new osg::Material(), true);
//    //    st->setMode(GL_BLEND, true);
//    //    st->setMode(GL_BLEND_COLOR,true);

//    AnimtkStateSetUpdateCallback* callback = new AnimtkStateSetUpdateCallback();
//    osgAnimation::Vec4KeyframeContainer* keys = callback->_sampler->getOrCreateKeyframeContainer();
//    keys->push_back(osgAnimation::Vec4Keyframe(0, osg::Vec4(0.95,0.95,1,1)));
//    //    keys->push_back(osgAnimation::Vec4Keyframe(1, osg::Vec4(0,0,0,1)));
//    //    keys->push_back(osgAnimation::Vec4Keyframe(2, osg::Vec4(0,0,1,1)));
//    //    keys->push_back(osgAnimation::Vec4Keyframe(3, osg::Vec4(0,0,1,1)));
//    //    keys->push_back(osgAnimation::Vec4Keyframe(4, osg::Vec4(0,1,0,1)));
//    //    keys->push_back(osgAnimation::Vec4Keyframe(5, osg::Vec4(1,0,0,1)));
//    callback->start();
//    st->setUpdateCallback(callback);

//    return st;
//}











//void ModelAnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
//{
//    osgEarth::GeoPoint geoPoint;
//    MoveableModel* moveableModel;
//    if (_animationPath.valid() &&
//        nv->getVisitorType()== osg::NodeVisitor::UPDATE_VISITOR &&
//        nv->getFrameStamp())
//    {
//        double time = nv->getFrameStamp()->getSimulationTime();
//        _latestTime = time;
//        moveableModel = dynamic_cast<MoveableModel*>(node);
//        if (!_pause)
//        {
//            // Only updat >= DBL_MAX) _firstTime = time;
//            //------------e _firstTime the first time, when its value is still DBL_MAX
//            if (_firstTime------------------------------------------------------------------------------------
//            osg::AnimationPath::ControlPoint cp;
//            double animatTime = getAnimationTime();
//            if (getAnimationPath()->getInterpolatedControlPoint(animatTime,cp))
//            {
//                geoPoint.fromWorld(moveableModel->getMapNode()->getMapSRS(), cp.getPosition());
//                moveableModel->setPosition(geoPoint);
//                moveableModel->getPositionAttitudeTransform()->setScale(cp.getScale());
//                if(moveableModel->mIs3D)
//                    moveableModel->getPositionAttitudeTransform()->setAttitude(cp.getRotation());
//                else
//                {
//                    double angle;
//                    osg::Vec3 vec;
//                    cp.getRotation().getRotate(angle, vec);
//                    vec.x() = 0;
//                    vec.y() = 0;
//                    moveableModel->getPositionAttitudeTransform()->setAttitude(osg::Quat(angle, vec));
//                    //                    qDebug()<<"angle:"<<osg::RadiansToDegrees(angle);
//                    //                    qDebug()<<"vec:"<<vec.x()<<","<<vec.y()<<","<<vec.z();

//                }

//                //                double angel;
//                //                osg::Vec3d vec;
//                //                cp.getRotation().getRotate(angel, vec);
//                //                osgEarth::Symbology::Style pm = baseModel->getPlaceNode()->getStyle();
//                //                pm.getOrCreate<osgEarth::Symbology::IconSymbol>()->heading() = osg::RadiansToDegrees(angel);
//                //                baseModel->getPlaceNode()->setStyle(pm);
//                //emit current position----------------------------------------------------------------------
//                //                positionCanged = true;
//                //if(static_cast<int>(animatTime) % 3 == 0)
//                //                defenseModelNode->curentPosition(geoPoint);
//            }

//            if(moveableModel && (_latestTime - _firstTime) > _animationPath->getPeriod())
//                moveableModel->stop();
//        }
//    }

//    // must call any nested node callbacks and continue subgraph traversal.
//    NodeCallback::traverse(node,nv);
//}


//void osg::AnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
//{
//if (_animationPath.valid() &&
//        nv->getVisitorType()== osg::NodeVisitor::UPDATE_VISITOR &&
//        nv->getFrameStamp())
//    {
//    MoveableModel* x;
//    x = dynamic_cast<MoveableModel*>(node);
//    NodeCallback::traverse(x,nv);

//}
//}





MoveableModel::MoveableModel(MapItem *mapControler, const std::string &modelUrl, const std::string &iconUrl, QObject *parent):
    simpleModelNode(mapControler, modelUrl, iconUrl, parent)
{
//    mMoveAnimationPathCallback = new osg::AnimationPathCallback();
//    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
//    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
//    mMoveAnimationPathCallback->setAnimationPath(path);

//    getGeoTransform()->addUpdateCallback(mMoveAnimationPathCallback);




//getGeoTransform()->setUpdateCallback(callback);

}

void MoveableModel::moveTo(osgEarth::GeoPoint destinationPoint, double mSpeed)
{
    //mMoveAnimationPathCallback->getAnimationPath()->clear();
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

    double objectlenght = 4;
    qDebug()<<"x: " << currentWPoint.x();
    osg::Vec3d pI (currentWPoint.x()+6, currentWPoint.y()-6, currentWPoint.z());


    double xPII = currentWPoint.x() + (wDef.length()/12)*(wDesPos.x()-currentWPoint.x());
    double yPII = currentWPoint.y() + (wDef.length()/12)*(wDesPos.y()-currentWPoint.y());
    double zPII = currentWPoint.z() + (wDef.length()/12)*(wDesPos.z()-currentWPoint.z());

    osg::Vec3d pII (xPII, yPII, zPII);

    osg::Quat rotate;
    rotate.makeRotate(osg::X_AXIS, localDef);
    double t = distance / mSpeed;

    osg::ref_ptr<AnimtkUpdateCallback> callback = new MakePathDistanceCallback();
    osg::ref_ptr<osgAnimation::Vec3CubicBezierKeyframeContainer> keys;
//    AnimtkUpdateCallback* callback = new MakePathDistanceCallback(abbas);
//    osgAnimation::Vec3CubicBezierKeyframeContainer* keys;
    keys = callback->_sampler->getOrCreateKeyframeContainer();
    keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(0, osgAnimation::Vec3CubicBezier(
                                                                 currentWPoint
                                                                 )));

    keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(1, osgAnimation::Vec3CubicBezier(
                                                                 currentWPoint,pI,pII
                                                                 )));

    keys->push_back(osgAnimation::Vec3CubicBezierKeyframe(4, osgAnimation::Vec3CubicBezier(
                                                                 wDesPos
                                                                 )));\
        callback->start();



        getGeoTransform()->setUpdateCallback(callback);

//    mMoveAnimationPathCallback->getAnimationPath()->insert(0, osg::AnimationPath::ControlPoint(currentWPoint, rotate, osg::Vec3d(1,1,1)));
//    mMoveAnimationPathCallback->getAnimationPath()->insert(2, osg::AnimationPath::ControlPoint(pI, rotate, osg::Vec3d(1,1,1)));
//    mMoveAnimationPathCallback->getAnimationPath()->insert(5, osg::AnimationPath::ControlPoint(pII, rotate, osg::Vec3d(1,1,1)));
//    mMoveAnimationPathCallback->getAnimationPath()->insert(t,osg::AnimationPath::ControlPoint(wDesPos,rotate, osg::Vec3d(1,1,1)));
    //setUpdateCallback(mMoveAnimationPathCallback);
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
