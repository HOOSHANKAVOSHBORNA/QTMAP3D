#include "vehicle.h"
#include <osg/MatrixTransform>
#include <list>
#include <QDebug>

//void vehicle::movePack(osg::Vec3d desti, float speedd)
//{
//    carAnimPC = new osg::AnimationPathCallback;
//    carAnimPC->setAnimationPath(run(desti,speedd));
//    carPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
//    QuantomMt->setUpdateCallback(carAnimPC);
//    wheelRot();


//}

void vehicle::spinHolder(osg::Vec3d desti)
{
    spinAnimPC = new osg::AnimationPathCallback;
    spinAnimPC->setPivotPoint(osg::Vec3d(-5,0,0));
    spinAnimPC->setAnimationPath(spiner(desti));
    spinerPos->setUpdateCallback(spinAnimPC);
}

//void vehicle::wheelRot()
//{
//    wheelAnimPcFr = new osg::AnimationPathCallback;
//    wheelAnimPcFl = new osg::AnimationPathCallback;
//    wheelAnimPcRr1 = new osg::AnimationPathCallback;
//    wheelAnimPcRl1 = new osg::AnimationPathCallback;
//    wheelAnimPcRr2 = new osg::AnimationPathCallback;
//    wheelAnimPcRl2 = new osg::AnimationPathCallback;


    //    wheelAnimPcFr->setAnimationPath(wheelAnimationPath(wheelPosFr->getPosition(),180));

    //    wheelAnimPcFl->setAnimationPath(wheelAnimationPath(wheelPosFl->getPosition(),0));

    //    wheelAnimPcRr1->setAnimationPath(wheelAnimationPath(wheelPosRr1->getPosition(),180));

    //    wheelAnimPcRl1->setAnimationPath(wheelAnimationPath(wheelPosRl1->getPosition(),0));

    //    wheelAnimPcRr2->setAnimationPath(wheelAnimationPath(wheelPosRr2->getPosition(),180));

    //    wheelAnimPcRl2->setAnimationPath(wheelAnimationPath(wheelPosRl2->getPosition(),0));

//    wheelPosFr->setUpdateCallback(wheelAnimPcFr);
//    wheelPosFl->setUpdateCallback(wheelAnimPcFl);
//    wheelPosRr1->setUpdateCallback(wheelAnimPcRr1);
//    wheelPosRl1->setUpdateCallback(wheelAnimPcRl1);
//    wheelPosRr2->setUpdateCallback(wheelAnimPcRr2);
//    wheelPosRl2->setUpdateCallback(wheelAnimPcRl2);
//}

vehicle::vehicle()
{
    car = osgDB::readRefNodeFile("../map3dlib/data/models/truck/truck-body.osgt");
    wheel = osgDB::readRefNodeFile("../map3dlib/data/models/truck/wheel.osgt");
    dualWheel = osgDB::readRefNodeFile("../map3dlib/data/models/truck/wheel-dual.osgt");
    spinerr = osgDB::readRefNodeFile("../map3dlib/data/models/truck/truck-spin.osgt");
    holder = osgDB::readRefNodeFile("../map3dlib/data/models/truck/truck-hold.osgt");
    QuantomMt = new osg::MatrixTransform;

    carPath = new osg::AnimationPath;

    carPos = new osg::PositionAttitudeTransform();
    carPos->setScale(osg::Vec3(1,1,1));
    carPos->setPosition(osg::Vec3(0,0,0));
    carPos->addChild(car);

    //    holderPos = new osg::PositionAttitudeTransform();
    //    holderPos->setScale(osg::Vec3(1,1,1));
    //    //holderPos->setPosition(osg::Vec3(carPos->getPosition().x(),carPos->getPosition().y(),carPos->getPosition().z()));
    //    holderPos->addChild(holder);
    holderPos = new osg::MatrixTransform();
    holderPos->setMatrix(osg::Matrix::translate(osg::Vec3(-2, 0, 1.3)));
    holderPos->addChild(holder);

    //    spinerPos = new osg::PositionAttitudeTransform();
    //    spinerPos->setScale(osg::Vec3(1,1,1));
    //    spinerPos->setPosition(osg::Vec3(carPos->getPosition().x()-5,carPos->getPosition().y(),carPos->getPosition().z()));
    //    spinerPos->addChild(spinerr);
    //    spinerPos->addChild(holderPos);
    spinerPos = new osg::MatrixTransform();
    spinerPos->setMatrix(osg::Matrix::translate(osg::Vec3(-5, 0, 2.6)));
    spinerPos->addChild(spinerr);
    spinerPos->addChild(holderPos);



    //    wheelPosFr = new osg::PositionAttitudeTransform();
    //    wheelPosFr->addChild(wheel);
    //    wheelPosFr->setScale(osg::Vec3(2,2,2));
    //    wheelPosFr->setPosition(osg::Vec3(3.2f,-1.7f,0.8f));
    //    wheelPosFr->setAttitude(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS));
    wheelPosFr = new osg::MatrixTransform();
    wheelPosFr->addChild(wheel);
    wheelPosFr->setMatrix(osg::Matrix::rotate(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS))*
                          osg::Matrix::scale(2,2,2)*
                          osg::Matrix::translate(osg::Vec3(3.2f,-1.7f,0.8f)));


    //    wheelPosFl = new osg::PositionAttitudeTransform();
    //    wheelPosFl->addChild(wheel);
    //    wheelPosFl->setScale(osg::Vec3(2,2,2));
    //    wheelPosFl->setPosition(osg::Vec3(3.2f,1.7f,0.8f));
    wheelPosFl = new osg::MatrixTransform();
    wheelPosFl->addChild(wheel);
    wheelPosFl->setMatrix(osg::Matrix::scale(2,2,2)*
                          osg::Matrix::translate(osg::Vec3(3.2f,1.7f,0.8f)));


    //    wheelPosRl1 = new osg::PositionAttitudeTransform();
    //    wheelPosRl1->addChild(dualWheel);
    //    wheelPosRl1->setScale(osg::Vec3(2,2,2));
    //    wheelPosRl1->setPosition(osg::Vec3(-3.7f,1.7f,0.8f));
    wheelPosRl1 = new osg::MatrixTransform();
    wheelPosRl1->addChild(dualWheel);
    wheelPosRl1->setMatrix(osg::Matrix::scale(2,2,2)*
                           osg::Matrix::translate(osg::Vec3(-3.7f,1.7f,0.8f)));


    //    wheelPosRr1 = new osg::MatrixTransform();
    //    wheelPosRr1->addChild(dualWheel);
    //    wheelPosRr1->setScale(osg::Vec3(2,2,2));
    //    wheelPosRr1->setPosition(osg::Vec3(-3.7f,-1.7f,0.8f));
    //    wheelPosRr1->setAttitude(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS));
    wheelPosRr1 = new osg::MatrixTransform();
    wheelPosRr1->addChild(dualWheel);
    wheelPosRr1->setMatrix(osg::Matrix::scale(2,2,2)*
                           osg::Matrix::rotate(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS))*
                           osg::Matrix::translate(osg::Vec3(-3.7f,-1.7f,0.8f)));


    //    wheelPosRl2 = new osg::MatrixTransform();
    //    wheelPosRl2->addChild(dualWheel);
    //    wheelPosRl2->setScale(osg::Vec3(2,2,2));
    //    wheelPosRl2->setPosition(osg::Vec3(-5.7f,1.7f,0.8f));
    wheelPosRl2 = new osg::MatrixTransform();
    wheelPosRl2->addChild(dualWheel);
    wheelPosRl2->setMatrix(osg::Matrix::scale(2,2,2)*
                           osg::Matrix::translate(osg::Vec3(-5.7f,1.7f,0.8f)));


    //    wheelPosRr2 = new osg::MatrixTransform();
    //    wheelPosRr2->addChild(dualWheel);
    //    wheelPosRr2->setScale(osg::Vec3(2,2,2));
    //    wheelPosRr2->setPosition(osg::Vec3(-5.7f,-1.7f,0.8f));
    //    wheelPosRr2->setAttitude(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS));
    wheelPosRr2 = new osg::MatrixTransform();
    wheelPosRr2->addChild(dualWheel);
    wheelPosRr2->setMatrix(osg::Matrix::scale(2,2,2)*
                           osg::Matrix::rotate(osg::Quat(osg::inDegrees(180.0),osg::X_AXIS))*
                           osg::Matrix::translate(osg::Vec3(-5.7f,-1.7f,0.8f)));


    QuantomMt->addChild(carPos);
    QuantomMt->addChild(spinerPos);
    QuantomMt->addChild(wheelPosFr);
    QuantomMt->addChild(wheelPosFl);
    QuantomMt->addChild(wheelPosRl1);
    QuantomMt->addChild(wheelPosRr1);
    QuantomMt->addChild(wheelPosRl2);
    QuantomMt->addChild(wheelPosRr2);

    this->addChild(QuantomMt);

}

osg::AnimationPath *vehicle::run(osg::Vec3d& dest, float speed)
{

    //    osg::Vec3d currentPos = carPos->getPosition();
    //    osg::Quat rotate;
    //    osg::Vec3f angle = dest - currentPos;
    //    rotate.makeRotate(osg::X_AXIS, angle);
    //    time = (dest-carPos->getPosition()).length()/speed;
    //    osg::AnimationPath::ControlPoint cp0;
    //    osg::AnimationPath::ControlPoint cp1;
    //
    //    cp0.setPosition(carPos->getPosition());
    //    cp1.setPosition(dest);
    //
    //    cp0.setScale(carPos->getScale());
    //    cp1.setScale(carPos->getScale());
    //
    //    cp0.setRotation(carPos->getAttitude());
    //    cp1.setRotation(rotate);
    //
    //    carPath->insert(0.0,cp0);
    //    carPath->insert(time,cp1);
    //
    //   qDebug() <<carPath->getPeriod();
    return carPath;

}

osg::AnimationPath *vehicle::wheelAnimationPath(osg::Vec3d wheelLoc, float RotDeg)
{
    osg::AnimationPath* wheelPath = new osg::AnimationPath;
    wheelPath->setLoopMode(osg::AnimationPath::LOOP);

    osg::AnimationPath::ControlPoint cp0;
    osg::AnimationPath::ControlPoint cp1;
    osg::AnimationPath::ControlPoint cp2;

    if (RotDeg==180){
        cp0.setRotation(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inDegrees(0.0),osg::Y_AXIS,osg::inDegrees(RotDeg),osg::Z_AXIS));
        cp1.setRotation(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inDegrees(-180.0),osg::Y_AXIS,osg::inDegrees(RotDeg),osg::Z_AXIS));
        cp2.setRotation(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inDegrees(-359.9),osg::Y_AXIS,osg::inDegrees(RotDeg),osg::Z_AXIS));
    }
    else {
        cp0.setRotation(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inDegrees(0.0),osg::Y_AXIS,osg::inDegrees(RotDeg),osg::Z_AXIS));
        cp1.setRotation(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inDegrees(180.0),osg::Y_AXIS,osg::inDegrees(RotDeg),osg::Z_AXIS));
        cp2.setRotation(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inDegrees(359.9),osg::Y_AXIS,osg::inDegrees(RotDeg),osg::Z_AXIS));
    }


    cp0.setScale(osg::Vec3(2.f,2.f,2.f));
    cp0.setPosition(wheelLoc);
    cp1.setScale(osg::Vec3(2.f,2.f,2.f));
    cp1.setPosition(wheelLoc);
    cp2.setScale(osg::Vec3(2.f,2.f,2.f));
    cp2.setPosition(wheelLoc);

    wheelPath->insert(0,cp0);
    wheelPath->insert(0.5,cp1);
    wheelPath->insert(1,cp2);

    return wheelPath;

}

osg::AnimationPath *vehicle::spiner(osg::Vec3d &dest)
{
    osg::AnimationPath* spinPath = new osg::AnimationPath;

    //    osg::Vec3d currentPos = spinerPos->getPosition();
    //    osg::Quat rotate;
    //    osg::Vec3f angle = dest - currentPos;
    //    rotate.makeRotate(osg::Y_AXIS, angle);

    //    osg::AnimationPath::ControlPoint cp0;
    //    osg::AnimationPath::ControlPoint cp1;

    //    cp0.setPosition(spinerPos->getPosition());
    //    cp1.setPosition(spinerPos->getPosition());

    //    cp0.setScale(spinerPos->getScale());
    //    cp1.setScale(spinerPos->getScale());

    //    cp0.setRotation(spinerPos->getAttitude());
    //    cp1.setRotation(rotate);
    //    //cp1.setRotation(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inDegrees(0.0),osg::Y_AXIS,osg::inDegrees(90.0),osg::Z_AXIS));

    //    spinPath->insert(0,cp0);
    //    spinPath->insert(1,cp1);

    //    spinPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
    return spinPath;
}

osg::AnimationPath *vehicle::holderRot(osg::Vec3d &dest)
{
    osg::AnimationPath* holderPath = new osg::AnimationPath;

    //    osg::Vec3d currentPos = holderPos->getPosition();
    //    osg::Quat rotate;
    //    osg::Vec3f angle = dest - currentPos;
    //    rotate.makeRotate(osg::Y_AXIS, angle);

    //    osg::AnimationPath::ControlPoint cp0;
    //    osg::AnimationPath::ControlPoint cp1;

    //    cp0.setPosition(holderPos->getPosition());
    //    cp1.setPosition(holderPos->getPosition());

    //    cp0.setScale(holderPos->getScale());
    //    cp1.setScale(holderPos->getScale());

    //    cp0.setRotation(holderPos->getAttitude());
    //    cp1.setRotation(rotate);
    //    //cp1.setRotation(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inDegrees(0.0),osg::Y_AXIS,osg::inDegrees(90.0),osg::Z_AXIS));

    //    holderPath->insert(0,cp0);
    //    holderPath->insert(1,cp1);

    //    holderPath->setLoopMode(osg::AnimationPath::NO_LOOPING);
    return holderPath;
}

void vehicle::setPause(bool pause)
{
    wheelAnimPcFr->setPause(pause);
    wheelAnimPcFl->setPause(pause);
    wheelAnimPcRl1->setPause(pause);
    wheelAnimPcRr1->setPause(pause);
    wheelAnimPcRr2->setPause(pause);
    wheelAnimPcRl2->setPause(pause);

    carAnimPC->setPause(pause);
}


void AnimtkUpdateCallback::moveTruck(osg::Vec3d desti, float speedd)
{
    des = desti;
    velocity = speedd;
    osg::Vec3d currentPos = m_vcl->carPos->getPosition();
    osg::Vec3f rotAxis = des - currentPos;
    rotAxis.z() = 0;
    rotate.makeRotate(osg::X_AXIS, rotAxis);

    osgAnimation::Vec3KeyframeContainer* _truckPositionKeys = _truckPositionSampler->getOrCreateKeyframeContainer();
    osgAnimation::FloatKeyframeContainer* _wheelRotationKeys = _wheelRotationSampler->getOrCreateKeyframeContainer();



    m_duration = (des-m_vcl->carPos->getPosition()).length()/velocity;
    _truckPositionKeys->push_back(osgAnimation::Vec3Keyframe(0, currentPos));
    _truckPositionKeys->push_back(osgAnimation::Vec3Keyframe(m_duration, des));

    _wheelRotationKeys->push_back(osgAnimation::FloatKeyframe(0, 0));
    _wheelRotationKeys->push_back(osgAnimation::FloatKeyframe(1, 2.0 * 3.14159265359));


}

void AnimtkUpdateCallback::spinHolder(osg::Vec3d target)
{
    osg::Matrix c = m_vcl->spinerPos->getMatrix() ;
    osg::Vec3d currentPos = c.getTrans();
    osg::Vec3f rotAxis = target - currentPos;
    rotAxis.z() = 0;
    spinRotate.makeRotate(osg::X_AXIS, rotAxis);


    osg::Matrix f = m_vcl->holderPos->getMatrix() ;
    osg::Vec3d currentHPos = f.getTrans();
    osg::Vec3f rotHAxis = target - currentHPos;
    rotHAxis.y() = 0;
    holderRotate.makeRotate(osg::X_AXIS, rotHAxis);



    osgAnimation::QuatKeyframeContainer* _spinRotationKeys = _spinRotationSampler->getOrCreateKeyframeContainer();
    _spinRotationKeys->push_back(osgAnimation::QuatKeyframe(0, currentSpinRotate));
    _spinRotationKeys->push_back(osgAnimation::QuatKeyframe(5, spinRotate));
    currentSpinRotate = spinRotate;

    osgAnimation::QuatKeyframeContainer* _holderRotationKeys = _holderRotationSampler->getOrCreateKeyframeContainer();
    _holderRotationKeys->push_back(osgAnimation::QuatKeyframe(0, currentHolderRotate));
    _holderRotationKeys->push_back(osgAnimation::QuatKeyframe(5, holderRotate));
    currentHolderRotate = holderRotate;
}


void AnimtkUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR &&
            nv->getFrameStamp() &&
            nv->getFrameStamp()->getFrameNumber() != _lastUpdate) {


        _lastUpdate = nv->getFrameStamp()->getFrameNumber();
        _currentTime = osg::Timer::instance()->tick();

        float t = osg::Timer::instance()->delta_s(_startTime, _currentTime);

        osg::MatrixTransform* transformSpin = m_vcl->spinerPos;
        osg::MatrixTransform* transformHolder = m_vcl->holderPos;



        osg::Quat newSpinRotation;
        _spinRotationSampler->getValueAt(t, newSpinRotation);

            transformSpin->setMatrix(
                                     osg::Matrix::rotate(newSpinRotation)*
                        osg::Matrix::translate(osg::Vec3(-5, 0, 2.6)));

        osg::Quat newHolderRotation;
        _holderRotationSampler->getValueAt(t, newHolderRotation);
        transformHolder->setMatrix(
                    osg::Matrix::rotate(newHolderRotation)*
                    osg::Matrix::translate(osg::Vec3(-2, 0, 1.3))
                    );

        if (_playing) {
            osg::MatrixTransform* transform   = m_vcl->QuantomMt;


            osg::MatrixTransform* wheelPosFl  = m_vcl->wheelPosFl;
            osg::MatrixTransform* wheelPosFr  = m_vcl->wheelPosFr;
            osg::MatrixTransform* wheelPosRl1 = m_vcl->wheelPosRl1;
            osg::MatrixTransform* wheelPosRr1 = m_vcl->wheelPosRr1;
            osg::MatrixTransform* wheelPosRl2 = m_vcl->wheelPosRl2;
            osg::MatrixTransform* wheelPosRr2 = m_vcl->wheelPosRr2;

            osg::Matrix mat = transform->getMatrix();

            //                t = fmod(t, m_duration);


            osg::Vec3 newTruckPosition;
            _truckPositionSampler->getValueAt(t, newTruckPosition);
            transform->setMatrix(osg::Matrix::rotate(rotate)*osg::Matrix::translate(newTruckPosition)
                                 );





            float newWheelRotation;
            _wheelRotationSampler->getValueAt(fmod(t, 1), newWheelRotation);



            osg::Matrix matFl = osg::Matrix::rotate(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inRadians(newWheelRotation),osg::Y_AXIS,osg::inDegrees(0.0),osg::Z_AXIS))*
                    osg::Matrix::scale(2,2,2)*
                    osg::Matrix::translate(osg::Vec3(3.2f,1.7f,0.8f));

            osg::Matrix matFr = osg::Matrix::rotate(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inRadians(-newWheelRotation),osg::Y_AXIS,osg::inDegrees(180.0),osg::Z_AXIS))*
                    osg::Matrix::scale(2,2,2)*
                    osg::Matrix::translate(osg::Vec3(3.2f,-1.7f,0.8f));

            osg::Matrix matRl1 = osg::Matrix::rotate(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inRadians(newWheelRotation),osg::Y_AXIS,osg::inDegrees(0.0),osg::Z_AXIS))*
                    osg::Matrix::scale(2,2,2)*
                    osg::Matrix::translate(osg::Vec3(-3.7f,1.7f,0.8f));

            osg::Matrix matRr1 = osg::Matrix::rotate(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inRadians(-newWheelRotation),osg::Y_AXIS,osg::inDegrees(180.0),osg::Z_AXIS))*
                    osg::Matrix::scale(2,2,2)*
                    osg::Matrix::translate(osg::Vec3(-3.7f,-1.7f,0.8f));

            osg::Matrix matRl2 = osg::Matrix::rotate(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inRadians(newWheelRotation),osg::Y_AXIS,osg::inDegrees(0.0),osg::Z_AXIS))*
                    osg::Matrix::scale(2,2,2)*
                    osg::Matrix::translate(osg::Vec3(-5.7f,1.7f,0.8f));

            osg::Matrix matRr2 = osg::Matrix::rotate(osg::Quat(osg::inDegrees(0.0),osg::X_AXIS,osg::inRadians(-newWheelRotation),osg::Y_AXIS,osg::inDegrees(180.0),osg::Z_AXIS))*
                    osg::Matrix::scale(2,2,2)*
                    osg::Matrix::translate(osg::Vec3(-5.7f,-1.7f,0.8f));

            wheelPosFl->setMatrix(matFl);
            wheelPosFr->setMatrix(matFr);
            wheelPosRl1->setMatrix(matRl1);
            wheelPosRr1->setMatrix(matRr1);
            wheelPosRl2->setMatrix(matRl2);
            wheelPosRr2->setMatrix(matRr2);


            if (t > m_duration)
                _playing = false;


        }
    }

    traverse(node,nv);
}

void AnimtkUpdateCallback::setupAnimtkNode()
{
    this->start();
    m_vcl->setUpdateCallback(this);
}
