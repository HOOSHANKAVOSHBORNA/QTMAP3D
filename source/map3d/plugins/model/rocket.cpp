#include "rocket.h"
#include "draw.h"

Rocket::Rocket(osgEarth::MapNode *mapNode, QObject *parent):
    BaseModel(mapNode, parent)
{
    osg::ref_ptr<osg::Node>  node = osgDB::readRefNodeFile("../map3dlib/data/models/rocket.osgb");

    if (!node)
    {
        //todo show massage here
        return;
    }
    //create style-------------------------------------------------------------------------------------------------
    osgEarth::Symbology::Style  style;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(node);
    setStyle(style);

    //osg::Vec3d center = getBound().center();
    float radius = getBound().radius();
    float scale = 3;

    mFire = new osgParticle::FireEffect(osg::Vec3f(0, 2*radius,0),scale,100.0);
    mFire->setUseLocalParticleSystem(false);

    mSmoke = new osgParticle::SmokeTrailEffect(osg::Vec3f(0, 2*radius,0),scale/3,100.0);
    mSmoke->setUseLocalParticleSystem(false);
}

void Rocket::shoot(const osg::Vec3d &pos, double speed)
{
    if(mIsShoot)
        return;

    mIsShoot = true;

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
    //create first pause
    path->insert(2, osg::AnimationPath::ControlPoint(currentWPoint, rotate, getScale()));
    path->insert(t + 2,osg::AnimationPath::ControlPoint(wPos,rotate, getScale()));

    mAnimationPathCallback = new ModelAnimationPathCallback();
    mAnimationPathCallback->setAnimationPath(path);
    setUpdateCallback(mAnimationPathCallback);
    addEffect(path->getPeriod());
    //draw line for debuge------------------------------------------------
//        osg::Vec3Array* keyPoint = new osg::Vec3Array;
//        keyPoint->push_back(currentWPoint);
//        keyPoint->push_back(wPos);
//        getMapNode()->getParent(0)->getParent(0)->addChild(drawLine(keyPoint, 2.0));
//        getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(currentWPoint));
//        getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(wPos));
}

void Rocket::stop()
{
    removeEffect();
}

void Rocket::addEffect(double emitterDuration)
{
    //add fire-----------------------------------------------------------------------------------------------------
    osgEarth::Registry::shaderGenerator().run(mFire);// for textures or lighting
    getPositionAttitudeTransform()->addChild(mFire);
    mFire->setEmitterDuration(emitterDuration);
    mFire->setParticleDuration(0.2);
    osgEarth::Registry::shaderGenerator().run(mFire->getParticleSystem());// for textures or lighting
    getMapNode()->addChild(mFire->getParticleSystem());
    //add smoke----------------------------------------------------------------------------------------------------
    osgEarth::Registry::shaderGenerator().run(mSmoke);// for textures or lighting
    getPositionAttitudeTransform()->addChild(mSmoke);
    mSmoke->setEmitterDuration(emitterDuration);
    mSmoke->setParticleDuration(5);
    osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());// for textures or lighting
    getMapNode()->addChild(mSmoke->getParticleSystem());
}

void Rocket::removeEffect()
{
    //remove fire---------------------------------------------
    getMapNode()->removeChild(mFire->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mFire);
    //remove smoke--------------------------------------------
    getMapNode()->removeChild(mSmoke->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mSmoke);
}
