#include "flyingmodel.h"
#include "draw.h"

#include <QDebug>
#include <osgParticle/FireEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/SmokeEffect>
#include <osgParticle/ExplosionEffect>
#include <osgParticle/ExplosionDebrisEffect>
#include <osgEarth/Registry>
#include <osgGA/EventVisitor>
#include <osgGA/EventHandler>
#include <osgViewer/View>
#include <osgEarth/IntersectionPicker>
#include <QMouseEvent>

class MapAnimationPathCallback: public osg::AnimationPathCallback
{
public:
    void operator()(osg::Node* node, osg::NodeVisitor* nv) override{

        if (_animationPath.valid() &&
                nv->getVisitorType()== osg::NodeVisitor::UPDATE_VISITOR &&
                nv->getFrameStamp())
        {
            double time = nv->getFrameStamp()->getSimulationTime();
            _latestTime = time;

            if (!_pause)
            {
                // Only update _firstTime the first time, when its value is still DBL_MAX
                if (_firstTime == DBL_MAX) _firstTime = time;
                //------------------------------------------------------------------------------------------------
                FlyingModel* flyNode = dynamic_cast<FlyingModel*>(node);
                osg::AnimationPath::ControlPoint cp;
                if (!flyNode->isHit() && getAnimationPath()->getInterpolatedControlPoint(getAnimationTime(),cp))
                {
                    osgEarth::GeoPoint geoPoint;
                    geoPoint.fromWorld(flyNode->getMapNode()->getMapSRS(), cp.getPosition());
                    flyNode->setPosition(geoPoint);
                    flyNode->getPositionAttitudeTransform()->setScale(cp.getScale());
                    flyNode->getPositionAttitudeTransform()->setAttitude(cp.getRotation());
                    //emit current position----------------------------------------------------------------------
                    geoPoint.makeGeographic();
                    emit flyNode->positionChanged(geoPoint);
                    //check collision----------------------------------------------------------------------------
                    if(flyNode->getFollowingModel() != nullptr)
                    {
                        //qDebug()<<"!= nullptr"<< QString(flyNode->getName().c_str());
                        double distance = (flyNode->getPosition().vec3d() - flyNode->getFollowingModel()->getPosition().vec3d()).length();
                        if(distance < 3)
                        {
                            //qDebug()<<QString(flyNode->getName().c_str());
                            flyNode->collision(flyNode->getFollowingModel());

                        }
                    }
                }

                //qDebug()<<"p:"<<cp.getPosition().x()<<","<<cp.getPosition().y()<<","<<cp.getPosition().z();
            }
        }

        // must call any nested node callbacks and continue subgraph traversal.
        NodeCallback::traverse(node,nv);
    }
};


FlyingModel::FlyingModel(osgEarth::MapNode* mapNode, const QString &fileName)
    :osgEarth::Annotation::ModelNode(mapNode, osgEarth::Symbology::Style())
{
    osg::ref_ptr<osg::Node>  node = osgDB::readRefNodeFile(fileName.toStdString());

    if (!node)
    {
        //todo show massage here
        return;
    }
    //create style-------------------------------------------------------------------------------------------------
    osgEarth::Symbology::Style  style;
    //style.getOrCreate<osgEarth::Symbology::RenderSymbol>()->depthOffset()->enabled() = true;
    //style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_RELATIVE_TO_TERRAIN;
    //style.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);
    setStyle(style);
    //-------------------------------------------------------------------------------------------------------------
    osg::Vec3d center = getBound().center();
    float radius = getBound().radius();
    float scale = 3;
    //add fire-----------------------------------------------------------------------------------------------------
    osgEarth::Registry::shaderGenerator().run(this);
    osgParticle::FireEffect *fire = new osgParticle::FireEffect(center + osg::Vec3f(0, radius,0),scale,100.0);
    getPositionAttitudeTransform()->addChild(fire);
    fire->setUseLocalParticleSystem(false);
    getMapNode()->getParent(0)->getParent(0)->addChild(fire->getParticleSystem());
    fire->setEmitterDuration(360000);
    fire->setParticleDuration(0.2);
    //add smoke----------------------------------------------------------------------------------------------------
    osgParticle::SmokeTrailEffect *smoke = new osgParticle::SmokeTrailEffect(center + osg::Vec3f(0, radius,0),scale/3,100.0);
    getPositionAttitudeTransform()->addChild(smoke);
    smoke->setUseLocalParticleSystem(false);
    getMapNode()->getParent(0)->getParent(0)->addChild(smoke->getParticleSystem());
    smoke->setEmitterDuration(360000);
    smoke->setParticleDuration(5);
}

void FlyingModel::setLatLongPosition(const osg::Vec3d &pos)
{
    osgEarth::GeoPoint  pointLatLong(osgEarth::SpatialReference::get("wgs84"), pos.x(), pos.y(), pos.z());
    osgEarth::GeoPoint  mapPoint;
    pointLatLong.transform(getMapNode()->getMapSRS(), mapPoint);
    setPosition(mapPoint);

    //draw line------------------------------------------------
    //    osg::Vec3d worldpos;
    //    mapPoint.toWorld(worldpos);
    //    getMapNode()->addChild(drawCordination(worldpos + osg::Vec3d(0,2,0)));

}

void FlyingModel::flyTo(const osg::Vec3d &pos, double speed)
{
    if(getPause())
        return;

    osg::Vec3d currentWPoint;
    getPosition().toWorld(currentWPoint);

    osg::Vec3d wPos;
    osgEarth::GeoPoint(getMapNode()->getMapSRS(), pos).toWorld(wPos);

    osg::Vec3d wDef = wPos - currentWPoint;
    double len = wDef.normalize();
    //transfer def vector to local----------------------------------------
    osg::Matrixd localTransfer;
    getPosition().createWorldToLocal(localTransfer);
    osg::Quat localRotation;
    localRotation = localTransfer.getRotate();
    osg::Matrixd rotateTransfer = osg::Matrixd::rotate(localRotation);
    osg::Vec3f localDef = wDef * rotateTransfer;
    //--------------------------------------------------------------------
    osg::Quat rotate;
    rotate.makeRotate(-osg::Y_AXIS, localDef);

    osg::Vec3d estimatePos = wPos + wDef*100*len;
    double t = (estimatePos - wPos).length() / speed;

    osg::AnimationPath* path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);

    path->insert(0, osg::AnimationPath::ControlPoint(currentWPoint,getPositionAttitudeTransform()->getAttitude(),getScale()));
    path->insert(2,osg::AnimationPath::ControlPoint(wPos,rotate, getScale()));
    path->insert(t,osg::AnimationPath::ControlPoint(estimatePos,rotate, getScale()));

    mAnimationPathCallback = new MapAnimationPathCallback();
    mAnimationPathCallback->setAnimationPath(path);
    setUpdateCallback(mAnimationPathCallback);

    //draw line------------------------------------------------
    //    osg::Vec3Array* keyPoint = new osg::Vec3Array;
    //    keyPoint->push_back(currentWPoint);
    //    keyPoint->push_back(wPos);
    //    keyPoint->push_back(estimatePos);
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawLine(keyPoint, 1.0));
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(currentWPoint));
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(wPos));
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(estimatePos));
}

void FlyingModel::shoot(const osg::Vec3d &pos, double speed)
{
    if(getPause())
        return;

    osg::Vec3d currentWPoint;
    getPosition().toWorld(currentWPoint);

    osg::Vec3d wPos;
    osgEarth::GeoPoint(getMapNode()->getMapSRS(), pos).toWorld(wPos);

    osg::Vec3d wDef = wPos - currentWPoint;
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
    rotate.makeRotate(-osg::Y_AXIS, localDef);
    double t = distance / speed;

    osg::AnimationPath* path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);

    path->insert(0, osg::AnimationPath::ControlPoint(currentWPoint, rotate, getScale()));
    path->insert(t,osg::AnimationPath::ControlPoint(wPos,rotate, getScale()));

    mAnimationPathCallback = new MapAnimationPathCallback();
    mAnimationPathCallback->setAnimationPath(path);
    setUpdateCallback(mAnimationPathCallback);

    //draw line for debuge------------------------------------------------
    //    osg::Vec3Array* keyPoint = new osg::Vec3Array;
    //    keyPoint->push_back(currentWPoint);
    //    keyPoint->push_back(wPos);
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawLine(keyPoint, 2.0));
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(currentWPoint));
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(wPos));
}

void FlyingModel::setPause(bool pause)
{
    if(mAnimationPathCallback != nullptr)
        mAnimationPathCallback->setPause(pause);
}

bool FlyingModel::getPause() const
{
    if(mAnimationPathCallback != nullptr)
        return mAnimationPathCallback->getPause();
    return false;
}

void FlyingModel::setFollowingModel(FlyingModel *followingModel)
{
    mFollowingModel = followingModel;
}

FlyingModel *FlyingModel::getFollowingModel() const
{
    return mFollowingModel;
}

void FlyingModel::collision(FlyingModel *other)
{
    qDebug()<<QString(getName().c_str());
    mIsHit = true;
    setPause(true);
    if(other != nullptr)
    {
        //playExplosionEffect(1.0f);
        other->collision(nullptr);
    }

    emit hit(other);
    setNodeMask(false);
    //    getMapNode()->removeChild(this);
}

bool FlyingModel::isHit() const
{
    return mIsHit;
}

void FlyingModel::playExplosionEffect(float scale)
{
    osg::Vec3d worldPosition;
    getPosition().toWorld(worldPosition);

    osg::ref_ptr<osg::PositionAttitudeTransform> pSphereGroup = new osg::PositionAttitudeTransform;

    osg::ref_ptr<osgParticle::ExplosionEffect> explosion1 = new osgParticle::ExplosionEffect(worldPosition, 8.0f * scale, 0.128f);
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion2 = new osgParticle::ExplosionEffect(worldPosition, 4.0f * scale, 0.25f);
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion3 = new osgParticle::ExplosionEffect(worldPosition, 2.0f * scale, 0.5f);
    osg::ref_ptr<osgParticle::ExplosionEffect> explosion4 = new osgParticle::ExplosionEffect(worldPosition, 1.0f * scale, 1.0f);

    explosion1->setParticleDuration(1.0);
    explosion2->setParticleDuration(1.2);
    explosion3->setParticleDuration(1.4);
    explosion4->setParticleDuration(1.6);

    osg::ref_ptr<osgParticle::ExplosionDebrisEffect> debris1 = new osgParticle::ExplosionDebrisEffect(worldPosition, 4.0f * scale, 0.03125f);

    osg::ref_ptr<osgParticle::SmokeEffect> smoke = new osgParticle::SmokeEffect(worldPosition, 10.0f * scale,2.0);
    smoke->setEmitterDuration(0.6);
    smoke->setParticleDuration(1.6);

    explosion1->setTextureFileName("../map3dlib/data/images/fire_p1.png");
    explosion2->setTextureFileName("../map3dlib/data/images/fire_p2.png");
    explosion3->setTextureFileName("../map3dlib/data/images/fire_p3.png");
    explosion4->setTextureFileName("../map3dlib/data/images/fire_p4.png");

    debris1->setTextureFileName("../map3dlib/data/images/debris_p1.png");

    smoke->setTextureFileName("../map3dlib/data/images/smoke_p1.png");

    pSphereGroup->addChild(explosion3);
    pSphereGroup->addChild(explosion4);

    pSphereGroup->addChild(debris1);

    pSphereGroup->addChild(smoke);
    pSphereGroup->setPosition(worldPosition);

    getMapNode()->getParent(0)->getParent(0)->addChild(pSphereGroup);


}

