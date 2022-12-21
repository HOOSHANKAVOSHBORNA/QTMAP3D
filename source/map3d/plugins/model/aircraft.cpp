﻿#include "aircraft.h"
#include "draw.h"

#include <QDebug>
#include <osgEarth/Registry>
#include <osgGA/EventVisitor>
#include <osgGA/EventHandler>
#include <osgViewer/View>
#include <osgEarth/IntersectionPicker>
#include <QMouseEvent>
#include <QTimer>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarthAnnotation/ImageOverlay>
#include <osg/Material>
#include<osg/Switch>



Aircraft::Aircraft(MapController *value, UIHandle *uiHandle, osgEarth::MapNode *mapNode, osg::Node *node, QObject *parent)
    :BaseModel(mapNode, parent)
{
    mMapController = value;
    mIs3d = mMapController->getMode();

    mUIHandle = uiHandle;
    if (!node)
    {
        //todo show massage here
        return;
    }

    //create switch node for root--------------------------------------------------------------------
    mRoot = new osg::Switch;
    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRoot);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 1;
//    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale() = 100;

//    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
//    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_MAP;

    setStyle(rootStyle);
    //--create icon Nodes---------------------------------------------------------------------------
    osg::Image* redIcon = osgDB::readImageFile("../data/models/aircraft/aircraft-red.png");
    redIcon->scaleImage(64, 64, redIcon->r());
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(redIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
    //    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    //    geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(1.0), osg::StateAttribute::ON);
    redGeode->addDrawable(redImageDrawable);
//    osgEarth::Annotation::ImageOverlay*  redImageOverlay = new osgEarth::Annotation::ImageOverlay(getMapNode(), redIcon);
//    redImageOverlay->setBounds(osgEarth::Bounds(0.0, 0.0, 64.0, 64.0));

    osg::Image* yellowIcon = osgDB::readImageFile("../data/models/aircraft/aircraft-yellow.png");
    yellowIcon->scaleImage(64, 64, yellowIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(yellowIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
    yellowGeode->addDrawable(yellowImageDrawable);
    //--create text-----------------------------------------------------------------------------
//    osgText::Text* text = new osgText::Text;
//    text->setFont();
//    text->setColor(osg::Vec4(1.0, 0.0, 0.0, 1.0));
//    text->setCharacterSize(2);
//    text->setPosition(osg::Vec3d(-5,0,0));

//    // the default layout is left to right, typically used in languages
//    // originating from europe such as English, French, German, Spanish etc..
//    text->setLayout(osgText::Text::LEFT_TO_RIGHT);
//    text->setText("Aircraft");
//    osg::Geode* textGeode = new osg::Geode;
//    textGeode->addDrawable(text);
//    osgEarth::Annotation::ImageOverlay*  yellowImageOverlay = new osgEarth::Annotation::ImageOverlay(getMapNode(), yellowIcon);
    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::Green;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;
    osg::ref_ptr<osgEarth::Annotation::PlaceNode> lable = new osgEarth::Annotation::PlaceNode("Aircraft",labelStyle, yellowIcon);
    lable->getPositionAttitudeTransform()->setPosition(osg::Vec3(0, 0, 1));
    lable->setScale(osg::Vec3(1,1,1));
    //--add nods--------------------------------------------------------------------------------
    //mRoot->addChild(node, mIs3d);
    mRoot->addChild(redGeode, !mIs3d);
    mRoot->addChild(yellowGeode,false);
    mRoot->addChild(lable, true);
    //----------------------------------------------------------------------------------------
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

    mCameraRangeChangeable = true;
    mLocationPoints = new osg::Vec3Array();
    mTempLocationPoints = new osg::Vec3Array();

    //map mode changed
    connect(mMapController, &MapController::modeChanged, this, &Aircraft::onModeChanged);
}

void Aircraft::flyTo(const osg::Vec3d &pos, double heading, double speed)
{

    if(mIsStop)
        return;
    //    heading = 30;
    osgEarth::GeoPoint posGeo(getMapNode()->getMapSRS(), pos);

    osg::Vec3d currentPosW;
    getPosition().toWorld(currentPosW);

    osg::Vec3d posW;
    posGeo.toWorld(posW);
    //---------------------------------------
    if(mLocationPoints->empty())
        mLocationPoints->push_back(currentPosW);
    mLocationPoints->push_back(posW);
    //move---------------------------------------------------------------------------------------------------
    osg::Vec3d diffW = posW - currentPosW;
    osg::Matrixd currentPoslocalTransfer;
    getPosition().createWorldToLocal(currentPoslocalTransfer);
    osg::Vec3d diffLocal = diffW * osg::Matrixd::rotate(currentPoslocalTransfer.getRotate());
    diffLocal.normalize();

    osg::Quat diffRotate;
    diffRotate.makeRotate(osg::Vec3d(0, 1, 0), diffLocal);
    //heading----------------------------------------------------------------------------------------------
    osg::Vec3d northVec(0, 1, 0);//in local
    osg::Vec3d headingVecLocal = northVec * osg::Matrixd::rotate(osg::inDegrees(-heading), osg::Z_AXIS);
    headingVecLocal.normalize();

    headingVecLocal.z() = diffLocal.z();

    osg::Matrixd posLocalToWorld;
    posGeo.createLocalToWorld(posLocalToWorld);
    osg::Vec3d headingVecW = headingVecLocal * osg::Matrixd::rotate(posLocalToWorld.getRotate());

    osg::Quat headingRotate;
    headingRotate.makeRotate(osg::Vec3d(0, 1, 0), headingVecLocal);
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

    //    osgEarth::Symbology::Style st = getStyle();
    //    double y =  st.getOrCreate<osgEarth::Symbology::ModelSymbol>()->scaleY().value().eval();
    //    qDebug()<<"getScale:"<<y;

    //    //draw line------------------------------------------------
    //    osg::Vec3Array* keyPoint = new osg::Vec3Array;
    //    keyPoint->push_back(currentPosW);
    //    keyPoint->push_back(posW);
    //    keyPoint->push_back(posEstimateW);
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawLine(keyPoint, 1.0));
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(currentPosW));
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(posW));
    //    getMapNode()->getParent(0)->getParent(0)->addChild(drawCordination(posEstimateW));

    //----------------------------------------------------

}

void Aircraft::stop()
{
    mIsStop = true;
    if(mAnimationPathCallback != nullptr)
        mAnimationPathCallback->getAnimationPath()->clear();
    removeEffect();
}


void Aircraft::setTruckModel(osgEarth::Annotation::ModelNode *truckModel)
{
    mTruckModel = truckModel;
}
osgEarth::Annotation::ModelNode *Aircraft::getTruckModel() const
{
    return mTruckModel;
}

void Aircraft::setInformation(QString info)
{
    mInformation = info;
    mUIHandle->iwUpdateData(this, mInformation);
}

void Aircraft::iw2D3DButtonClicked()
{
//    qDebug()<<"iw2D3DButtonClicked";
    mMapController->goToPosition(getPosition(), 200);
}

void Aircraft::iwRouteButtonClicked()
{
//    mIsRoute = true;
//    qDebug()<<"iwRouteButtonClicked";
    mMapController->getRoot()->addChild(drawLine(mLocationPoints, 1.0));


    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [=](){
        mMapController->getRoot()->addChild(drawLine(mTempLocationPoints, 1.0));
    });
    timer->start(200);

}

void Aircraft::iwFollowButtonClicked()
{
//    qDebug()<<"iwFollowButtonClicked";
    mMapController->setTrackNode(getGeoTransform());
}

void Aircraft::iwMoreButtonClicked()
{
    qDebug()<<"iwMoreButtonClicked";
}

void Aircraft::onModeChanged(bool is3DView)
{
    mIs3d = is3DView;
    select(mIsSelected);
}

void Aircraft::mousePressEvent(QMouseEvent *event, bool onModel)
{
    BaseModel::mousePressEvent(event, onModel);
    if(onModel)
    {
        mUIHandle->iwSetReceiverObject(this);
        mUIHandle->iwShow(this, UIHandle::InfoWidgetType::Airplane);
        mUIHandle->iwUpdateData(this, mInformation);
    }
    else
        mMapController->untrackNode();
}

void Aircraft::curentPosition(osgEarth::GeoPoint pos)
{
    BaseModel::curentPosition(pos);

//    if(mIsRoute)
//    {
        osg::Vec3d currentPosW;
        pos.toWorld(currentPosW);
        mTempLocationPoints->push_back(currentPosW);
//    }
}

void Aircraft::addEffect(double emitterDuration)
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

void Aircraft::removeEffect()
{
    //remove fire---------------------------------------------
    getMapNode()->removeChild(mFire->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mFire);
    //remove smoke--------------------------------------------
    getMapNode()->removeChild(mSmoke->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mSmoke);
}
