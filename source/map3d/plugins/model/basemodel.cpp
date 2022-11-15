#include "basemodel.h"
#include "draw.h"

#include <QDebug>

void ModelAnimationPathCallback::operator()(osg::Node *node, osg::NodeVisitor *nv)
{
    BaseModel* baseModel;
    bool hit = false;
    bool positionCanged = false;
    osgEarth::GeoPoint geoPoint;

    if (_animationPath.valid() &&
            nv->getVisitorType()== osg::NodeVisitor::UPDATE_VISITOR &&
            nv->getFrameStamp())
    {
        double time = nv->getFrameStamp()->getSimulationTime();
        _latestTime = time;

        baseModel = dynamic_cast<BaseModel*>(node);
        if (!baseModel->hasHit())
            //check collision----------------------------------------------------------------------------
            if(baseModel->getFollowModel() != nullptr)
            {
                osg::Vec3d wPosition;
                baseModel->getPosition().toWorld(wPosition);
                osg::Vec3d wFolowPosition;
                baseModel->getFollowModel()->getPosition().toWorld(wFolowPosition);
                double distance = (wPosition - wFolowPosition).length();
                if(distance < 3)
                    hit = true;
            }

        if (!_pause)
        {
            // Only update _firstTime the first time, when its value is still DBL_MAX
            if (_firstTime == DBL_MAX) _firstTime = time;
            //------------------------------------------------------------------------------------------------
            osg::AnimationPath::ControlPoint cp;
            if (getAnimationPath()->getInterpolatedControlPoint(getAnimationTime(),cp))
            {
                geoPoint.fromWorld(baseModel->getMapNode()->getMapSRS(), cp.getPosition());
                baseModel->setPosition(geoPoint);
                baseModel->getPositionAttitudeTransform()->setScale(cp.getScale());
                baseModel->getPositionAttitudeTransform()->setAttitude(cp.getRotation());
                //emit current position----------------------------------------------------------------------
                positionCanged = true;
            }

            if((_latestTime - _firstTime) > _animationPath->getPeriod())
                baseModel->stop();
        }
    }

    // must call any nested node callbacks and continue subgraph traversal.
    NodeCallback::traverse(node,nv);
    if(hit)
        baseModel->collision(baseModel->getFollowModel());
    if(positionCanged)
        emit baseModel->positionChanged(geoPoint);
}

BaseModel::BaseModel(osgEarth::MapNode *mapNode, QObject *parent):
    QObject(parent),
    osgEarth::Annotation::ModelNode(mapNode, osgEarth::Symbology::Style())
{

}

QString BaseModel::getType() const
{
    return mType;
}

void BaseModel::setType(const QString &value)
{
    mType = value;
}

void BaseModel::setQStringName(QString name)
{
    setName(name.toStdString());
}

QString BaseModel::getQStringName()
{
    return QString(getName().c_str());
}

void BaseModel::setGeographicPosition(const osg::Vec3d &pos)
{
    osgEarth::GeoPoint  geoPoint(osgEarth::SpatialReference::get("wgs84"), pos);
    //transfer to map srs
    geoPoint.transformInPlace(getMapNode()->getMapSRS());
    setPosition(geoPoint);

    //draw line------------------------------------------------
//        osg::Vec3d worldpos;
//        geoPoint.toWorld(worldpos);
//        getMapNode()->addChild(drawCordination(worldpos + osg::Vec3d(0,2,0)));
}

osg::Vec3d BaseModel::getGeographicPosition() const
{
    osgEarth::GeoPoint position = getPosition();
    position.makeGeographic();
    return position.vec3d();
}

void BaseModel::playExplosionEffect(float scale)
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

//    osgEarth::Registry::shaderGenerator().run(explosion3);
    pSphereGroup->addChild(explosion3);
//    osgEarth::Registry::shaderGenerator().run(explosion4);
    pSphereGroup->addChild(explosion4);

//    osgEarth::Registry::shaderGenerator().run(debris1);
    pSphereGroup->addChild(debris1);

//    osgEarth::Registry::shaderGenerator().run(smoke);
    pSphereGroup->addChild(smoke);
    pSphereGroup->setPosition(worldPosition);

    osgEarth::Registry::shaderGenerator().run(pSphereGroup);// for textures or lighting
    getMapNode()->addChild(pSphereGroup);
}

void BaseModel::collision(BaseModel *collidedWith)
{
//    qDebug()<<QString(getQStringName());
    mHasHit = true;
//    setPause(true);
//    if(other != nullptr)
//    {
//        playExplosionEffect(1.0f);
//        other->collision(nullptr);
//    }
    playExplosionEffect(1.0f);
    setNodeMask(false);
//    mSmoke->setNodeMask(false);
//    mSmoke->getParticleSystem()->setNodeMask(false);
//    mFire->setNodeMask(false);
//    mFire->getParticleSystem()->setNodeMask(false);
//    //    getMapNode()->removeChild(this);

    emit hit(collidedWith);
    stop();
}

BaseModel *BaseModel::getFollowModel() const
{
    return mFollowModel;
}

void BaseModel::setFollowModel(BaseModel *followModel)
{
    mFollowModel = followModel;
}


bool BaseModel::hasHit() const
{
    return mHasHit;
}
