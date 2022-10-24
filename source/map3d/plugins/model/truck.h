#ifndef TRUCK_H
#define TRUCK_H

#include <QObject>
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
#include <QTextStream>
#include <array>

#include <osgEarthAnnotation/ModelNode>


class TruckUpdateCallback;

class Truck: public QObject, public osgEarth::Annotation::ModelNode
{
public:
    Truck(osgEarth::MapNode* mapNode);
    void setLatLongPosition(const osg::Vec3d &pos);
    void moveTo(osg::Vec3d desti, double speed);
    void aimTarget(osg::Vec3d target);
    bool shoot();
    bool hasRocket();

private:
    osg::MatrixTransform* _wholeTruckTransform;
    osg::MatrixTransform* _truckTransform;
    osg::MatrixTransform* _spinerTransform;
    osg::MatrixTransform* _holderTransform;
    osg::MatrixTransform* _wheelTransformFr;
    osg::MatrixTransform* _wheelTransformFl;
    osg::MatrixTransform* _wheelTransformRr1;
    osg::MatrixTransform* _wheelTransformRl1;
    osg::MatrixTransform* _wheelTransformRr2;
    osg::MatrixTransform* _wheelTransformRl2;
    osg::MatrixTransform* _rocketTransform_0;
    osg::MatrixTransform* _rocketTransform_1;
    osg::MatrixTransform* _rocketTransform_2;
//    osg::MatrixTransform* _rocketsPackTransform;



    osg::ref_ptr<osg::MatrixTransform> _leftWheelRotation;
    osg::ref_ptr<osg::MatrixTransform> _rightWheelRotation;
    osg::ref_ptr<osg::MatrixTransform> _leftDualWheelRotation;
    osg::ref_ptr<osg::MatrixTransform> _rightDualWheelRotation;

    osg::ref_ptr<osg::Node> _truck;
    osg::ref_ptr<osg::Node> _wheel;
    osg::ref_ptr<osg::Node> _dualWheel;
    osg::ref_ptr<osg::Node> _spiner;
    osg::ref_ptr<osg::Node> _holder;
    osg::ref_ptr<osg::Node> _rocket;
    osg::ref_ptr<osg::Node> _target;

    TruckUpdateCallback *_wholeTruckUpdateCallback = nullptr;
    TruckUpdateCallback *_leftWheelUpdateCallback      = nullptr;
    TruckUpdateCallback *_rightWheelUpdateCallback      = nullptr;
    TruckUpdateCallback *_spinerUpdateCallback     = nullptr;
    TruckUpdateCallback *_holderUpdateCallback     = nullptr;
    //TruckUpdateCallback *_rocketLaunchUpdateCallback = nullptr;

    //osg::AnimationPath *_rocketLaunch = nullptr;
    osg::AnimationPath *_wholeTruckAnimPath = nullptr;
    osg::AnimationPath *_rightWheelAnimPath      = nullptr;
    osg::AnimationPath *_leftWheelAnimPath      = nullptr;
    osg::AnimationPath *_spinerAnimPath     = nullptr;
    osg::AnimationPath *_holderAnimPath     = nullptr;

    osg::Quat curSpinRotate {osg::Quat()};
    osg::Quat curHoldRotate {osg::Quat()};

    std::array<bool, 3> _rocketsExis;
    int rocketNo{3};
};

class TruckUpdateCallback : public QObject, public osg::AnimationPathCallback{

    Q_OBJECT
public:
    TruckUpdateCallback();

    virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) override;

public slots:
    void start();
    void stop();

signals:
    void started();
    void finished();

private:
    osg::Timer_t        _startTime   = 0;
    osg::Timer_t        _currentTime = 0;
    unsigned int        _lastUpdate  = 0;
    bool                _playing     = false;

};



#endif // TRUCK_H
