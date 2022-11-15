#include "airplane.h"
#include "draw.h"

#include <QDebug>
#include <osgEarth/Registry>
#include <osgGA/EventVisitor>
#include <osgGA/EventHandler>
#include <osgViewer/View>
#include <osgEarth/IntersectionPicker>
#include <QMouseEvent>


Airplane::Airplane(osgEarth::MapNode* mapNode, const QString &fileName, QObject *parent)
    :BaseModel(mapNode, parent)
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
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale() = 1000;
    style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 0.09;

//    setScale(osg::Vec3(0.09f,0.09f,0.09f));
    setStyle(style);
    //-------------------------------------------------------------------------------------------------------------
    //osg::Vec3d center = getBound().center();
    float radius = getBound().radius();
    float scale = 3;

    mFire = new osgParticle::FireEffect(osg::Vec3f(0, 2*radius,0),scale,100.0);
    mFire->setUseLocalParticleSystem(false);

    mSmoke = new osgParticle::SmokeTrailEffect(osg::Vec3f(0, 2*radius,0),scale/3,100.0);
    mSmoke->setUseLocalParticleSystem(false);
    //    mGeodeParticle = new osg::Geode;
    //    mGeodeParticle->addDrawable(mFire->getParticleSystem());
    //    mGeodeParticle->addDrawable(mSmoke->getParticleSystem());
    //    osgEarth::Registry::shaderGenerator().run(mGeodeParticle);// for textures or lighting
    //    getMapNode()->addChild(mGeodeParticle);
}

void Airplane::flyTo(const osg::Vec3d &pos, double heading, double speed)
{

    if(mIsStop)
        return;
//    heading = 30;
    osgEarth::GeoPoint posGeo(getMapNode()->getMapSRS(), pos);

    osg::Vec3d currentPosW;
    getPosition().toWorld(currentPosW);

    osg::Vec3d posW;
    posGeo.toWorld(posW);
    //move---------------------------------------------------------------------------------------------------
    osg::Vec3d diffW = posW - currentPosW;
    osg::Matrixd currentPoslocalTransfer;
    getPosition().createWorldToLocal(currentPoslocalTransfer);
    osg::Vec3d diffLocal = diffW * osg::Matrixd::rotate(currentPoslocalTransfer.getRotate());
    diffLocal.normalize();

    osg::Quat diffRotate;
    diffRotate.makeRotate(-osg::Vec3d(0, 1, 0), diffLocal);
    //heading----------------------------------------------------------------------------------------------
    osg::Vec3d northVec(0, 1, 0);//in local
    osg::Vec3d headingVecLocal = northVec * osg::Matrixd::rotate(osg::inDegrees(-heading), osg::Z_AXIS);
    headingVecLocal.normalize();

    headingVecLocal.z() = diffLocal.z();

    osg::Matrixd posLocalToWorld;
    posGeo.createLocalToWorld(posLocalToWorld);
    osg::Vec3d headingVecW = headingVecLocal * osg::Matrixd::rotate(posLocalToWorld.getRotate());

    osg::Quat headingRotate;
    headingRotate.makeRotate(-osg::Vec3d(0, 1, 0), headingVecLocal);
    //-------------------------------------------------------------------------------------------------------
    osg::Vec3d posEstimateW1 = posW + (headingVecW * 100.0);
    osg::Vec3d posEstimateW = posW + (headingVecW * 100000.0);
//    qDebug()<<"estimatePos"<<estimatePos.z();
    double timeEstimate = (posEstimateW - posW).length() / speed;

    osg::AnimationPath* path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);

    path->insert(0, osg::AnimationPath::ControlPoint(currentPosW,getPositionAttitudeTransform()->getAttitude(),getScale()));
    path->insert(2,osg::AnimationPath::ControlPoint(posW,diffRotate, getScale()));
    path->insert(3,osg::AnimationPath::ControlPoint(posEstimateW1, headingRotate, getScale()));
    path->insert(timeEstimate,osg::AnimationPath::ControlPoint(posEstimateW, headingRotate, getScale()));

    mAnimationPathCallback = new ModelAnimationPathCallback();
    mAnimationPathCallback->setAnimationPath(path);
    setUpdateCallback(mAnimationPathCallback);



    //    //draw line------------------------------------------------
//            osg::Vec3Array* keyPoint = new osg::Vec3Array;
//            keyPoint->push_back(currentPosW);
//            keyPoint->push_back(posW);
//            keyPoint->push_back(posEstimateW);
//            getMapNode()->getParent(0)->getParent(0)->addChild(drawLine(keyPoint, 1.0));
//            getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(currentPosW));
//            getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(posW));
//            getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(posEstimateW));

}

void Airplane::stop()
{
    mIsStop = true;
    if(mAnimationPathCallback != nullptr)
        mAnimationPathCallback->getAnimationPath()->clear();
    removeEffect();
}


void Airplane::setTruckModel(osgEarth::Annotation::ModelNode *truckModel)
{
    mTruckModel = truckModel;
}
osgEarth::Annotation::ModelNode *Airplane::getTruckModel() const
{
    return mTruckModel;
}

void Airplane::addEffect(double emitterDuration)
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

void Airplane::removeEffect()
{
    //remove fire---------------------------------------------
    getMapNode()->removeChild(mFire->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mFire);
    //remove smoke--------------------------------------------
    getMapNode()->removeChild(mSmoke->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mSmoke);
}

