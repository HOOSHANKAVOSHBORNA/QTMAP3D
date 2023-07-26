#include "MoveableModel.h"
#include "mapItem.h"
#include <QObject>


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
    mMoveAnimationPathCallback = new osg::AnimationPathCallback();
    osg::ref_ptr<osg::AnimationPath> path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);
    mMoveAnimationPathCallback->setAnimationPath(path);

//    const osg::BoundingSphere& bs = getBound();

//    float size = 1000/bs.radius()*0.3f;
//    osg::MatrixTransform* positioned = new osg::MatrixTransform;
//    positioned->setDataVariance(osg::Object::STATIC);
//    positioned  ->setMatrix(osg::Matrix::translate(-bs.center())*
//                          osg::Matrix::scale(size,size,size)*
//                          osg::Matrix::rotate(osg::inDegrees(-90.0f),0.0f,0.0f,1.0f));

//    positioned->addChild(getGeoTransform());

//    osg::PositionAttitudeTransform* xform = new osg::PositionAttitudeTransform;
//    xform->setUpdateCallback(new osg::AnimationPathCallback(path,0.0,1.0));
//    xform->addChild(positioned);

    getGeoTransform()->addUpdateCallback(mMoveAnimationPathCallback);
    //setDynamic(true);
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

    mMoveAnimationPathCallback->getAnimationPath()->insert(0, osg::AnimationPath::ControlPoint(currentWPoint, rotate, getScale()));
    mMoveAnimationPathCallback->getAnimationPath()->insert(t,osg::AnimationPath::ControlPoint(wDesPos,rotate, getScale()));
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
