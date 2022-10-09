#ifndef VEHICLE_H
#define VEHICLE_H

#include <osgSim/OverlayNode>
#include <osg/AnimationPath>
#include <osg/PositionAttitudeTransform>
#include <osg/Group>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <osgDB/ReadFile>
#include <QObject>
#include <QDebug>
#include <osgAnimation/Sampler>

class vehicle;

class AnimtkUpdateCallback : public osg::NodeCallback
{
public:
    void moveTruck(osg::Vec3d desti, float speedd);
    void spinHolder(osg::Vec3d target);
    osg::Vec3d des;
    osg::Vec3d tar;
    double velocity;
    META_Object(osgAnimation, AnimtkUpdateCallback)

    AnimtkUpdateCallback()
    {
        _truckPositionSampler = new osgAnimation::Vec3LinearSampler;
        _wheelRotationSampler = new osgAnimation::FloatLinearSampler;
        _spinRotationSampler = new osgAnimation::QuatSphericalLinearSampler;
        _holderRotationSampler = new osgAnimation::QuatSphericalLinearSampler;
        _playing = false;
        _lastUpdate = 0;
    }
    AnimtkUpdateCallback(const AnimtkUpdateCallback& val, const osg::CopyOp& copyop = osg::CopyOp::SHALLOW_COPY):
        osg::Object(val, copyop),
        osg::Callback(val, copyop),
        osg::NodeCallback(val, copyop),
        _truckPositionSampler(val._truckPositionSampler),
        _wheelRotationSampler(val._wheelRotationSampler),
        _spinRotationSampler(val._spinRotationSampler),
        _holderRotationSampler(val._holderRotationSampler),
        _startTime(val._startTime),
        _currentTime(val._currentTime),
        _playing(val._playing),
        _lastUpdate(val._lastUpdate)
    {
    }

    void setVehicle(vehicle *vcl)
    {
        m_vcl = vcl;
    }
    /** Callback method called by the NodeVisitor when visiting a node.*/
    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);

    void start() { _startTime = osg::Timer::instance()->tick(); _currentTime = _startTime; _playing = true;}
    void stop() { _currentTime = _startTime; _playing = false;}

    osg::ref_ptr<osgAnimation::Vec3LinearSampler> _truckPositionSampler;
    osg::ref_ptr<osgAnimation::FloatLinearSampler> _wheelRotationSampler;
    osg::ref_ptr<osgAnimation::QuatSphericalLinearSampler> _spinRotationSampler;
    osg::ref_ptr<osgAnimation::QuatSphericalLinearSampler> _holderRotationSampler;
    osg::Timer_t _startTime;
    osg::Timer_t _currentTime;
    bool _playing;
    unsigned int _lastUpdate;


    //osg::MatrixTransform* setupAnimtkNode();
    void setupAnimtkNode();
private:
    double m_duration = 0.0;
    osg::ref_ptr<osg::Node> truck;
    vehicle *m_vcl = nullptr;
    osg::Quat rotate;
    osg::Quat spinRotate;
    osg::Quat currentSpinRotate = osg::Quat();
    osg::Quat holderRotate;
    osg::Quat currentHolderRotate = osg::Quat();

};

class vehicle : public osg::Group
{

public:
    //void movePack(osg::Vec3d desti, float speedd);
    void spinHolder(osg::Vec3d desti);
    //void wheelRot();
    osg::AnimationPath* run(osg::Vec3d& dest, float speed);
    osg::AnimationPath* wheelAnimationPath(osg::Vec3d wheelLoc, float RotDeg);
    osg::AnimationPath* spiner(osg::Vec3d& dest);
    osg::AnimationPath* holderRot(osg::Vec3d& dest);
    void setPause(bool pause);
    osg::PositionAttitudeTransform* carPos;


//    struct anim{
//        double time;
//        osg::Vec3 pos;
//        osg::Quat rotate;
//    };
    explicit vehicle();

//    void setWehicleModel(std::string addr);
//    void setWheelModel(std::string addr);
//    osg::AnimationPath* carAnimationPath(QVector<anim> a);


    osg::MatrixTransform* QuantomMt;

    osg::MatrixTransform* spinerPos;
    osg::MatrixTransform* holderPos;
    osg::MatrixTransform* wheelPosFr;
    osg::MatrixTransform* wheelPosFl;
    osg::MatrixTransform* wheelPosRr1;
    osg::MatrixTransform* wheelPosRl1;
    osg::MatrixTransform* wheelPosRr2;
    osg::MatrixTransform* wheelPosRl2;

    osg::ref_ptr<osg::Node> car;
    osg::ref_ptr<osg::Node> wheel;
    osg::ref_ptr<osg::Node> dualWheel;
    osg::ref_ptr<osg::Node> spinerr;
    osg::ref_ptr<osg::Node> holder;



private:
    float stop;

    //float time;
//    osg::PositionAttitudeTransform* spinerPos;
//    osg::PositionAttitudeTransform* holderPos;
//    osg::PositionAttitudeTransform* wheelPosFr;
//    osg::PositionAttitudeTransform* wheelPosFl;
//    osg::PositionAttitudeTransform* wheelPosRr1;
//    osg::PositionAttitudeTransform* wheelPosRl1;
//    osg::PositionAttitudeTransform* wheelPosRr2;
//    osg::PositionAttitudeTransform* wheelPosRl2;

    osg::AnimationPathCallback* carAnimPC;
    osg::AnimationPath* carPath;

    osg::AnimationPathCallback* wheelAnimPcFr;
    osg::AnimationPathCallback* wheelAnimPcFl;
    osg::AnimationPathCallback* wheelAnimPcRr1;
    osg::AnimationPathCallback* wheelAnimPcRl1;
    osg::AnimationPathCallback* wheelAnimPcRr2;
    osg::AnimationPathCallback* wheelAnimPcRl2;

    osg::AnimationPathCallback* spinAnimPC;
    osg::AnimationPathCallback* holderAnimPC;

signals:

public slots:
};

#endif // VEHICLE_H
