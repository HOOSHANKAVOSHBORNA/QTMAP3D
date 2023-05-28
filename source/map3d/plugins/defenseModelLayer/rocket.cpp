#include "rocket.h"
#include "draw.h"
#include "defenseModelNodeAutoScaler.h"

Rocket::Rocket(MapItem *mapControler, QObject *parent):
    DefenseModelNode(mapControler, parent)
{
    osg::ref_ptr<osg::Node>  node = osgDB::readRefNodeFile("../data/models/system/truck/rocket.osgb");
    osg::ref_ptr<osg::PositionAttitudeTransform> pat = new osg::PositionAttitudeTransform;
    pat->setScale(osg::Vec3d(0.7, 0.7, 0.7));

    pat->addChild(node);

    if (!node)
    {
        //todo show massage here
        return;
    }
    mNode3D = new osg::Group;
    mNode3D->addChild(pat);
//    //create style-------------------------------------------------------------------------------------------------
    mRootNode = new osg::LOD;
    mNode2D = new osg::Switch;
    //-------------------------------------------------------------------------------------------------------------
    osgEarth::Symbology::Style  style;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = false;
//    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 1;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale() = 2000 * 3.5;

//    this->setCullingActive(false);
//    this->addCullCallback(
//                new RocketModelNodeAutoScaler( osg::Vec3d(1,1,1),
//                                               style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale().value(),
//                                               style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale().value() ));
    setStyle(style);
    //------------------------------------------------------------------------------------------------------------
    mRootNode->addChild(mNode3D, 0, std::numeric_limits<float>::max());

    //osg::Vec3d center = getBound().center();
//    float radius = getBound().radius();
//    float scale = 3;

//    mFire = new osgParticle::FireEffect(osg::Vec3f(0, -2*radius,0),scale,100.0);
//    mFire->setUseLocalParticleSystem(false);

//    mSmoke = new osgParticle::SmokeTrailEffect(osg::Vec3f(0, -2*radius,0),scale/3,100.0);
//    mSmoke->setUseLocalParticleSystem(false);
    mIs3D = true;

//    mRoot->addChild(node, false);
//    mRoot->addChild(node,true);
//    mRoot->addChild(node,false);
}

void Rocket::shoot(const osg::Vec3d &pos, double speed)
{
    if(mIsShoot)
        return;


    this->setCullingActive(false);
    this->addCullCallback(new DefenseModelNodeAutoScaler(10.5, 1, 600));

    mIsShoot = true;

    osg::Vec3d currentWPoint;
    getPosition().toWorld(currentWPoint);

    osg::Vec3d wPos;
    osgEarth::GeoPoint(getMapNode()->getMapSRS(), pos).toWorld(wPos);

    osg::Vec3d wDiff = wPos - currentWPoint;
    double distance = wDiff.normalize();
    //transfer def vector to local----------------------------------------
    osg::Matrixd localTransfer;
    getPosition().createWorldToLocal(localTransfer);
    osg::Quat localRotation;
    localRotation = localTransfer.getRotate();
    osg::Matrixd rotateTransfer = osg::Matrixd::rotate(localRotation);
    osg::Vec3f localDiff =  wDiff * rotateTransfer;
    //-------------------------------------------------------------------
    osg::Quat rotate;
    rotate.makeRotate(osg::Y_AXIS, localDiff);
    double t = distance / speed;
    osg::AnimationPath* path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);

    path->insert(0, osg::AnimationPath::ControlPoint(currentWPoint, rotate, getScale()));
    //create first pause
//    path->insert(2, osg::AnimationPath::ControlPoint(currentWPoint, rotate, getScale()));
    path->insert(t + 0,osg::AnimationPath::ControlPoint(wPos,rotate, getScale()));

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
    setNodeMask(false);
    removeEffect();
}

void Rocket::setAutoScale()
{
//    auto style = getStyle();
//    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
//    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = value;
//    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 5;
//    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale() = 10000 * 3.5;
//    setStyle(style);
    this->setCullingActive(false);
    this->addCullCallback(new DefenseModelNodeAutoScaler( 15, 1, 60));
}

void Rocket::addEffect(double emitterDuration)
{
    float radius = getBound().radius();
    float scale = 10;

    mFire = new osgParticle::FireEffect(osg::Vec3f(0, -2*radius,0),scale,100.0);
    mFire->setUseLocalParticleSystem(false);

    mSmoke = new osgParticle::SmokeTrailEffect(osg::Vec3f(0, -2*radius,0),scale/3,100.0);
    mSmoke->setUseLocalParticleSystem(false);
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
