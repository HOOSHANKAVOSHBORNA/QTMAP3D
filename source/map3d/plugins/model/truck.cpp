#include "truck.h"
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgParticle/FireEffect>

Truck::Truck(osgEarth::MapNode *mapNode):
    osgEarth::Annotation::ModelNode(mapNode, osgEarth::Symbology::Style())
{

    _truck     = osgDB::readRefNodeFile("../map3dlib/data/models/truck/truck-body.osgt");
    _wheel     = osgDB::readRefNodeFile("../map3dlib/data/models/truck/wheel.osgt");
    _dualWheel = osgDB::readRefNodeFile("../map3dlib/data/models/truck/wheel-dual.osgt");
    _spiner    = osgDB::readRefNodeFile("../map3dlib/data/models/truck/truck-spiner.osgt");
    _holder    = osgDB::readRefNodeFile("../map3dlib/data/models/truck/truck-holder.osgt");
    _rocket    = osgDB::readRefNodeFile("../map3dlib/data/models/truck/rocket.osgt");

    _wholeTruckTransform = new osg::MatrixTransform();
    //_wholeTruckTransform->setMatrix(osg::Matrix::rotate(osg::inDegrees(-30.0), osg::Z_AXIS));

    _rocketTransform_0 = new osg::MatrixTransform();
    _rocketTransform_0->setMatrix(osg::Matrix::translate(osg::Vec3d(6.8, -1.3, 0)));
    _rocketTransform_0->addChild(_rocket);


    _rocketTransform_1 = new osg::MatrixTransform();
    _rocketTransform_1->setMatrix(osg::Matrix::translate(osg::Vec3d(6.8, 0.0, 0)));
    _rocketTransform_1->addChild(_rocket);


    _rocketTransform_2 = new osg::MatrixTransform();
    _rocketTransform_2->setMatrix(osg::Matrix::translate(osg::Vec3d(6.8, 1.3, 0)));
    _rocketTransform_2->addChild(_rocket);

    //    _rocketsPackTransform = new osg::MatrixTransform();
    //    _rocketsPackTransform->addChild(_rocketTransform_0);
    //    _rocketsPackTransform->addChild(_rocketTransform_1);
    //    _rocketsPackTransform->addChild(_rocketTransform_2);

    _truckTransform = new osg::MatrixTransform();
    _truckTransform->addChild(_truck);

    _holderTransform = new osg::MatrixTransform();
    _holderTransform->setMatrix(osg::Matrix::translate(osg::Vec3d(-2, 0, 1.3)));
    _holderTransform->addChild(_holder);
    _holderTransform->addChild(_rocketTransform_0);
    _holderTransform->addChild(_rocketTransform_1);
    _holderTransform->addChild(_rocketTransform_2);

    _rocketsExis[0] = true;
    _rocketsExis[1] = true;
    _rocketsExis[2] = true;

    _spinerTransform = new osg::MatrixTransform();
    _spinerTransform->setMatrix(osg::Matrix::translate(osg::Vec3d(-5, 0, 2.6)));
    _spinerTransform->addChild(_spiner);
    _spinerTransform->addChild(_holderTransform);

    _rightWheelRotation = new osg::MatrixTransform;
    _rightWheelRotation->addChild(_wheel);
    _leftWheelRotation = new osg::MatrixTransform;
    _leftWheelRotation->addChild(_wheel);

    _wheelTransformFr = new osg::MatrixTransform();
    _wheelTransformFr->addChild(_rightWheelRotation);
    _wheelTransformFr->setMatrix(osg::Matrix::rotate(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS))*
                                 osg::Matrix::scale(2,2,2)*
                                 osg::Matrix::translate(osg::Vec3(3.2f,-1.7f,0.8f)));

    _wheelTransformFl = new osg::MatrixTransform();
    _wheelTransformFl->addChild(_leftWheelRotation);
    _wheelTransformFl->setMatrix(osg::Matrix::scale(2,2,2)*
                                 osg::Matrix::translate(osg::Vec3(3.2f,1.7f,0.8f)));


    _rightDualWheelRotation = new osg::MatrixTransform;
    _rightDualWheelRotation->addChild(_dualWheel);
    _leftDualWheelRotation = new osg::MatrixTransform;
    _leftDualWheelRotation->addChild(_dualWheel);


    _wheelTransformRl1 = new osg::MatrixTransform();
    _wheelTransformRl1->addChild(_leftDualWheelRotation);
    _wheelTransformRl1->setMatrix(osg::Matrix::scale(2,2,2)*
                                  osg::Matrix::translate(osg::Vec3(-3.7f,1.7f,0.8f)));

    _wheelTransformRr1 = new osg::MatrixTransform();
    _wheelTransformRr1->addChild(_rightDualWheelRotation);
    _wheelTransformRr1->setMatrix(osg::Matrix::scale(2,2,2)*
                                  osg::Matrix::rotate(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS))*
                                  osg::Matrix::translate(osg::Vec3(-3.7f,-1.7f,0.8f)));

    _wheelTransformRl2 = new osg::MatrixTransform();
    _wheelTransformRl2->addChild(_leftDualWheelRotation);
    _wheelTransformRl2->setMatrix(osg::Matrix::scale(2,2,2)*
                                  osg::Matrix::translate(osg::Vec3(-5.7f,1.7f,0.8f)));

    _wheelTransformRr2 = new osg::MatrixTransform();
    _wheelTransformRr2->addChild(_rightDualWheelRotation);
    _wheelTransformRr2->setMatrix(osg::Matrix::scale(2,2,2)*
                                  osg::Matrix::rotate(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS))*
                                  osg::Matrix::translate(osg::Vec3(-5.7f,-1.7f,0.8f)));

    _wholeTruckTransform->addChild(_truckTransform);
    _wholeTruckTransform->addChild(_spinerTransform);
    _wholeTruckTransform->addChild(_wheelTransformFr);
    _wholeTruckTransform->addChild(_wheelTransformFl);
    _wholeTruckTransform->addChild(_wheelTransformRl1);
    _wholeTruckTransform->addChild(_wheelTransformRr1);
    _wholeTruckTransform->addChild(_wheelTransformRl2);
    _wholeTruckTransform->addChild(_wheelTransformRr2);

    this->addChild(_wholeTruckTransform);

    _wholeTruckUpdateCallback = new TruckUpdateCallback;
    _leftWheelUpdateCallback  = new TruckUpdateCallback;
    _rightWheelUpdateCallback = new TruckUpdateCallback;
    _spinerUpdateCallback     = new TruckUpdateCallback;
    _holderUpdateCallback     = new TruckUpdateCallback;
    //_rocketLaunchUpdateCallback = new TruckUpdateCallback;


    _wholeTruckAnimPath = new osg::AnimationPath;
    _leftWheelAnimPath  = new osg::AnimationPath;
    _rightWheelAnimPath = new osg::AnimationPath;
    _spinerAnimPath     = new osg::AnimationPath;
    _holderAnimPath     = new osg::AnimationPath;
    //_rocketLaunch       = new osg::AnimationPath;

    _wholeTruckUpdateCallback->setAnimationPath(_wholeTruckAnimPath);
    _leftWheelUpdateCallback->setAnimationPath(_leftWheelAnimPath);
    _rightWheelUpdateCallback->setAnimationPath(_rightWheelAnimPath);
    _spinerUpdateCallback->setAnimationPath(_spinerAnimPath);
    _holderUpdateCallback->setAnimationPath(_holderAnimPath);
    //_rocketLaunchUpdateCallback->setAnimationPath(_rocketLaunch);

    _wholeTruckAnimPath->setLoopMode(osg::AnimationPath::LoopMode::NO_LOOPING);
    _leftWheelAnimPath->setLoopMode(osg::AnimationPath::LoopMode::LOOP);
    _rightWheelAnimPath->setLoopMode(osg::AnimationPath::LoopMode::LOOP);
    _spinerAnimPath->setLoopMode(osg::AnimationPath::LoopMode::NO_LOOPING);
    _holderAnimPath->setLoopMode(osg::AnimationPath::LoopMode::NO_LOOPING);
    //_rocketLaunch->setLoopMode(osg::AnimationPath::LoopMode::NO_LOOPING);

    this->setUpdateCallback(_wholeTruckUpdateCallback);
    _rightWheelRotation->setUpdateCallback(_rightWheelUpdateCallback);
    _leftWheelRotation->setUpdateCallback(_leftWheelUpdateCallback);
    _rightDualWheelRotation->setUpdateCallback(_rightWheelUpdateCallback);
    _leftDualWheelRotation->setUpdateCallback(_leftWheelUpdateCallback);
    _spinerTransform->setUpdateCallback(_spinerUpdateCallback);
    _holderTransform->setUpdateCallback(_holderUpdateCallback);
    //_rocketsPackTransform->setUpdateCallback(_rocketLaunchUpdateCallback);

    osgEarth::Symbology::Style  style;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(_wholeTruckTransform);

    setStyle(style);

}

void Truck::setLatLongPosition(const osg::Vec3d &pos)
{
    osgEarth::GeoPoint  pointLatLong(osgEarth::SpatialReference::get("wgs84"), pos.x(), pos.y(), pos.z());
    osgEarth::GeoPoint  mapPoint;
    pointLatLong.transform(getMapNode()->getMapSRS(), mapPoint);
    setPosition(mapPoint);

    auto abbas = getPosition();
    qDebug()<<"abbas z: "<<abbas.z();
    abbas.z() = 0;
    qDebug()<<"abbas z: "<<abbas.z();
    abbas.makeRelative(getMapNode()->getTerrain());
    qDebug()<<"abbas z: "<<abbas.z();

}
void Truck::moveTo(osg::Vec3d desti, double speed)
{

    _wholeTruckUpdateCallback->stop();
    _leftWheelUpdateCallback->stop();
    _rightWheelUpdateCallback->stop();

    _wholeTruckAnimPath->clear();
    _leftWheelAnimPath->clear();
    _rightWheelAnimPath->clear();

    osg::AnimationPath::ControlPoint truck_cp0;
    osg::AnimationPath::ControlPoint truck_cp1;

    osg::AnimationPath::ControlPoint r_wheel_cp0;
    osg::AnimationPath::ControlPoint r_wheel_cp1;
    osg::AnimationPath::ControlPoint r_wheel_cp2;

    osg::AnimationPath::ControlPoint l_wheel_cp0;
    osg::AnimationPath::ControlPoint l_wheel_cp1;
    osg::AnimationPath::ControlPoint l_wheel_cp2;


    osgEarth::GeoPoint destiG (osgEarth::SpatialReference::get("wgs84"),desti);
    destiG.makeRelative(getMapNode()->getTerrain());




    osg::Vec3d destination;
    destiG.toWorld(destination,getMapNode()->getTerrain());
    auto mapPosition = getPosition();
    osg::Matrixd sampleM;
    mapPosition.createWorldToLocal(sampleM);
    osg::Quat rot;
    rot = sampleM.getRotate();

    osg::Vec3d currentTruckPos;
    mapPosition.toWorld(currentTruckPos);
    //std::cout<<"xm: "<< desti.x()<<"ym: "<<desti.y()<<"zm: "<<desti.z()<< std::endl;
    osg::Vec3d axisf = destination - currentTruckPos;
    osg::Vec3d axis =axisf*osg::Matrixd::rotate(rot);

    osg::Quat rotate;
//    currentTruckPos.z() = 0;
//    desti.z() = 0;
//    axisf.z() = 0;
    rotate.makeRotate(osg::Vec3d(osg::X_AXIS), axis);

    const double time = axisf.length()/speed;


    truck_cp0.setPosition(currentTruckPos);
    truck_cp1.setPosition(destination);

    truck_cp0.setScale(_wholeTruckTransform->getMatrix().getScale());
    truck_cp1.setScale(_wholeTruckTransform->getMatrix().getScale());

    truck_cp0.setRotation(rotate);
    truck_cp1.setRotation(rotate);

    _wholeTruckAnimPath->insert(0.0 ,truck_cp0);
    _wholeTruckAnimPath->insert(time,truck_cp1);



    r_wheel_cp0.setRotation(osg::Quat());
    r_wheel_cp1.setRotation(osg::Quat(osg::inDegrees(-180.0), osg::Y_AXIS));
    r_wheel_cp2.setRotation(osg::Quat(osg::inDegrees(-359.99), osg::Y_AXIS));

    _rightWheelAnimPath->insert(0.0, r_wheel_cp0);
    _rightWheelAnimPath->insert(1, r_wheel_cp1);
    _rightWheelAnimPath->insert(2, r_wheel_cp2);



    l_wheel_cp0.setRotation(osg::Quat());
    l_wheel_cp1.setRotation(osg::Quat(osg::inDegrees(180.0), osg::Y_AXIS));
    l_wheel_cp2.setRotation(osg::Quat(osg::inDegrees(359.99), osg::Y_AXIS));

    _leftWheelAnimPath->insert(0.0, l_wheel_cp0);
    _leftWheelAnimPath->insert(1, l_wheel_cp1);
    _leftWheelAnimPath->insert(2, l_wheel_cp2);


    _wholeTruckUpdateCallback->start();
    _rightWheelUpdateCallback->start();
    _leftWheelUpdateCallback->start();


    QObject::connect(_wholeTruckUpdateCallback,
                     &TruckUpdateCallback::finished,
                     _leftWheelUpdateCallback,
                     &TruckUpdateCallback::stop);

    QObject::connect(_wholeTruckUpdateCallback,
                     &TruckUpdateCallback::finished,
                     _rightWheelUpdateCallback,
                     &TruckUpdateCallback::stop);
}

void Truck::aimTarget(osg::Vec3d target)
{
    _spinerUpdateCallback->stop();
    _holderUpdateCallback->stop();
    //_rocketLaunchUpdateCallback->stop();

    _spinerAnimPath->clear();
    _holderAnimPath->clear();
    //_rocketLaunch->clear();

    osg::AnimationPath::ControlPoint rocket_cp0;
    osg::AnimationPath::ControlPoint rocket_cp1;
    osg::AnimationPath::ControlPoint spiner_cp0;
    osg::AnimationPath::ControlPoint spiner_cp1;
    osg::AnimationPath::ControlPoint holder_cp0;
    osg::AnimationPath::ControlPoint holder_cp1;


    //    rocket_cp0.setPosition(_rocketsPackTransform->getMatrix().getTrans());
    //    rocket_cp1.setPosition(target);

    //_rocketLaunch->insert(0,rocket_cp0);
    //_rocketLaunch->insert(3,rocket_cp1);
    std::cout<<"x: "<< target.x()<<"y: "<<target.y()<<"z: "<<target.z()<< std::endl;

    auto mapPosition = getPosition();
    osg::Matrixd sample;
    mapPosition.createWorldToLocal(sample);
    osg::Quat rot;
    rot = sample.getRotate();

    osg::Vec3d currentSpinPos /*= _spinerTransform->getMatrix().getTrans()*/;
    mapPosition.toWorld(currentSpinPos);


    osg::Quat truckRot = this->getPositionAttitudeTransform()->getAttitude();
    osg::Matrixd truckRotMat = osg::Matrixd::rotate(truckRot);


    currentSpinPos += _wholeTruckTransform->getMatrix().getTrans();
    currentSpinPos += _spinerTransform->getMatrix().getTrans();
    osg::Vec3d axisP = target - currentSpinPos;
    axisP.normalize();
//    axisP = axisP * (truckRotMat);
    osg::Vec3d axis =axisP*osg::Matrixd::rotate(rot);

//    std::cout << axisP.x() << " , " << axisP.y() << " , " << axisP.z() << std::endl;
//    std::cout << axis.x() << " , " << axis.y() << " , " << axis.z() << std::endl;


    osg::Quat rotate;
    axis.z() = 0;
    rotate.makeRotate(osg::Vec3d(osg::X_AXIS)*truckRotMat, axis);


    spiner_cp0.setPosition(_spinerTransform->getMatrix().getTrans());
    spiner_cp1.setPosition(_spinerTransform->getMatrix().getTrans());

    spiner_cp0.setRotation(_spinerTransform->getMatrix().getRotate());
    spiner_cp1.setRotation(rotate);
    //    curSpinRotate = rotate;

    _spinerAnimPath->insert(0.0, spiner_cp0);
    _spinerAnimPath->insert(3, spiner_cp1);


    osg::Vec3d currentHoldPos/* = _holderTransform->getMatrix().getTrans()*/;
    mapPosition.toWorld(currentHoldPos);
    currentHoldPos += _wholeTruckTransform->getMatrix().getTrans();
    currentHoldPos += _spinerTransform->getMatrix().getTrans();
    currentHoldPos += _holderTransform->getMatrix().getTrans();
    osg::Vec3d axisHP = target - currentHoldPos;
    axisHP.normalize();
//    axisHP = truckRotMat * axisHP;
    osg::Vec3d axisH =axisHP*osg::Matrixd::rotate(rot);

    std::cout << "target.z() : " << target.z() << std::endl;
    std::cout << axisH.x() << " , " << axisH.y() << " , " << axisH.z() << std::endl;


    osg::Quat rotateH;
    axisH.x() = std::sqrt(std::pow(axisH.x(), 2.0) + std::pow(axisH.y(),2.0));
    axisH.y() = 0;
    rotateH.makeRotate(osg::Vec3d(osg::X_AXIS), axisH);


    holder_cp0.setPosition(_holderTransform->getMatrix().getTrans());
    holder_cp1.setPosition(_holderTransform->getMatrix().getTrans());

    holder_cp0.setRotation(_holderTransform->getMatrix().getRotate());
    holder_cp1.setRotation(rotateH);
    //    curHoldRotate = rotateH;

    _holderAnimPath->insert(0.0, holder_cp0);
    _holderAnimPath->insert(1, holder_cp1);

    _spinerUpdateCallback->start();
    _holderUpdateCallback->start();
    //_rocketLaunchUpdateCallback->start();
    //    QObject::connect(_holderUpdateCallback,
    //                     &TruckUpdateCallback::finished,
    //                     _rocketLaunchUpdateCallback,
    //                     &TruckUpdateCallback::start);
}

bool Truck::shoot()
{


    osg::Vec3 offset;
    bool doFire = false;

    switch(rocketNo) {
    case 1:
        offset = osg::Vec3(0.0,-1.0, 0.0);
        _holderTransform->removeChild(_rocketTransform_0);

        _rocketsExis[0] = false;
        doFire = true;
        break;

    case 2:
        offset = osg::Vec3(0.0, 0.0, 0.0);
        _holderTransform->removeChild(_rocketTransform_1);

        _rocketsExis[1] = false;
        doFire = true;
        break;

    case 3:
        offset = osg::Vec3(0.0, 1.0, 0.0);
        _holderTransform->removeChild(_rocketTransform_2);
        _rocketsExis[2] = false;
        doFire = true;
        break;
    default:
        return false;
    }


//    if (doFire) {
//        osgParticle::ExplosionEffect *explode = new osgParticle::ExplosionEffect(offset, 1.0f, 20.0f);
//        explode->setUseLocalParticleSystem(false);
//        //explode->setTextureFileName("/home/client111/Downloads/fire.png");
//        _holderTransform->addChild(explode);
//        getMapNode()->getParent(0)->getParent(0)->addChild(explode->getParticleSystem());

//    }

    rocketNo -= 1;
    return true;
}




TruckUpdateCallback::TruckUpdateCallback()
{
}

void TruckUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    auto _path = getAnimationPath();
    if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR &&
            nv->getFrameStamp() &&
            nv->getFrameStamp()->getFrameNumber() != _lastUpdate) {

        if (_playing && _path) {

            _currentTime = osg::Timer::instance()->tick();
            const double t = osg::Timer::instance()->delta_s(_startTime, _currentTime);

            if (_path->getLoopMode() == osg::AnimationPath::LoopMode::NO_LOOPING) {
                if (t >= _path->getPeriod()) {
                    stop();
                    traverse(node,nv);
                    return;
                }
            }

            osg::AnimationPath::ControlPoint cp;
            _path->getInterpolatedControlPoint(t, cp);
            osg::Matrix m;
            cp.getMatrix(m);
            osg::MatrixTransform *mt = dynamic_cast<osg::MatrixTransform*>(node);
            if (mt) {

                mt->setMatrix(m);
            }
            else
            {
                Truck *tp = dynamic_cast<Truck*>(node);
                if (tp) {
                    osgEarth::GeoPoint gp;
                    gp.fromWorld(osgEarth::SpatialReference::get("wgs84"), m.getTrans());
                    tp->setPosition(gp);
                    tp->getPositionAttitudeTransform()->setAttitude(m.getRotate());
                }
            }

        }
    }
        traverse(node,nv);
//    AnimationPathCallback::operator()(node,nv);
}

void TruckUpdateCallback::start()
{
    if (_playing == false) {
        setPause(false);
        _startTime = osg::Timer::instance()->tick();
        _currentTime = _startTime;
        _playing = true;
        AnimationPathCallback::reset();

        emit started();
    }
}

void TruckUpdateCallback::stop()
{
    if (_playing == true) {
        _currentTime = _startTime;
        _playing = false;
        setPause(true);

        emit finished();
    }
}
