#include "aircraft.h"
#include "aircraftcontextmenumodel.h"
#include "draw.h"

#include <QDebug>
#include <osgEarth/Registry>
#include <osgGA/EventVisitor>
#include <osgGA/EventHandler>
#include <osgViewer/View>
#include <osgEarth/IntersectionPicker>
#include <QMouseEvent>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QTimer>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarthAnnotation/ImageOverlay>
#include <osg/Material>
#include<osg/Switch>


const float RANGE3D = std::numeric_limits<float>::max();

Aircraft::Aircraft(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent)
    :BaseModel(mapControler->getMapNode(), parent)
{
    mQmlEngine = qmlEngine;
    mMapController = mapControler;
    mIs3D = mMapController->getMode();

    mUIHandle = uiHandle;
    mNode3D = osgDB::readRefNodeFile("../data/models/aircraft/boeing-747.osgb");
    if (!mNode3D)
    {
        //todo show massage here
        return;
    }

    //create switch node for root--------------------------------------------------------------------
    mRootNode = new osg::LOD;
    mNode2D = new osg::Switch;

    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 1;
    //    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale() = 100;

    //    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    //    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_MAP;

    setStyle(rootStyle);
    //--create icon Nodes---------------------------------------------------------------------------
    osg::Image* redIcon = osgDB::readImageFile("../data/models/aircraft/aircraft_red.png");
    redIcon->scaleImage(25, 32, redIcon->r());
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(redIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
    //    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    //    geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(1.0), osg::StateAttribute::ON);
    redGeode->addDrawable(redImageDrawable);

    osg::Image* yellowIcon = osgDB::readImageFile("../data/models/aircraft/aircraft_yellow.png");
    yellowIcon->scaleImage(25, 32, yellowIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(yellowIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
    yellowGeode->addDrawable(yellowImageDrawable);

    mNode2D->addChild(yellowGeode, false);
    mNode2D->addChild(redGeode, true);
    //--create lable-----------------------------------------------------------------------------
    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;
    osg::Image* lableImage = osgDB::readImageFile("../data/models/text-background.png");
    mLableNode = new osgEarth::Annotation::PlaceNode(getName(),labelStyle, lableImage);
    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3(0, 0, 2));
    //    mLableNode->getGeoTransform()->setPosition(osg::Vec3(0, 0, 2));
    getGeoTransform()->addChild(mLableNode);
    //--add nods--------------------------------------------------------------------------------
    if(mIs3D)
    {
        mRootNode->addChild(mNode3D, 0, RANGE3D);
        mRootNode->addChild(mNode2D, RANGE3D, std::numeric_limits<float>::max());
    }
    else
    {
        mRootNode->addChild(mNode3D, 0, 0);
        mRootNode->addChild(mNode2D, 0, std::numeric_limits<float>::max());
    }
    //------------------------------------------------------------------------------------------
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
    mLocationPoints = new osg::Vec3Array();
    mTempLocationPoints = new osg::Vec3Array();

    //map mode changed-----------------------------------------------------------------------
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

void Aircraft::setInformation(AircraftInfo info)
{
    mInformation = info;
    QString txtInfo = QString::fromUtf8(mInformation.toJson().toJson(QJsonDocument::Compact));
    mUIHandle->iwUpdateData(this, txtInfo);
}

void Aircraft::goOnTrack()
{
    mMapController->setTrackNode(getGeoTransform());
    //mMapController->goToPosition(getPosition(), 200);
}

void Aircraft::showInfoWidget()
{
    mUIHandle->iwSetReceiverObject(this);
    mUIHandle->iwShow(this, UIHandle::InfoWidgetType::Airplane);
    QString txtInfo = QString::fromUtf8(mInformation.toJson().toJson(QJsonDocument::Compact));
    mUIHandle->iwUpdateData(this, txtInfo);
}

void Aircraft::iw2D3DButtonClicked()
{
    //    qDebug()<<"iw2D3DButtonClicked";
//    goOnTrack();
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
    mIs3D = is3DView;
    if(mIs3D)
    {
        mRootNode->setRange(0, 0, RANGE3D);
        mRootNode->setRange(1, RANGE3D, std::numeric_limits<float>::max());

        auto style = getStyle();
        style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 1;
        setStyle(style);
    }
    else
    {
        mRootNode->setRange(0, 0, 0);
        mRootNode->setRange(1,0, std::numeric_limits<float>::max());
        auto style = getStyle();
        style.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 0;
        setStyle(style);
    }

    select(mIsSelected);
}

void Aircraft::frameEvent()
{
    osg::Vec3d wordPos;
    getPosition().toWorld(wordPos);
    float x, y;
    mMapController->worldToScreen(wordPos,x, y);
    mUIHandle->cmSetContextMenuPosition(mCurrentContextMenuItem, static_cast<int>(x), static_cast<int>(y));

    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( getPositionAttitudeTransform()->getBound().radius()/2, getPositionAttitudeTransform()->getBound().radius(), 2));
}

void Aircraft::mousePressEvent(QMouseEvent *event, bool onModel)
{
    BaseModel::mousePressEvent(event, onModel);
    if(event->button() == Qt::LeftButton)
    {
        if(onModel)
        {
           showInfoWidget();
            event->accept();
        }
        else
            mMapController->untrackNode();
    }
    if(event->button() == Qt::RightButton) {
        QQmlComponent *comp = new QQmlComponent(mQmlEngine);
        QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
            qDebug() << comp->errorString();

            if (comp->status() == QQmlComponent::Ready) {
                mCurrentContextMenuItem = static_cast<QQuickItem*>(comp->create(nullptr));
                AircraftContextMenumodel *model = new AircraftContextMenumodel;
                for(auto detectSystem: mInformation.DetectionSystems)
                    model->addRow(detectSystem);

                mCurrentContextMenuItem->setProperty("model", QVariant::fromValue<AircraftContextMenumodel*>(model));

                osg::Vec3d wordPos;
                getPosition().toWorld(wordPos);
                float x, y;
                QQmlEngine::setObjectOwnership(mCurrentContextMenuItem, QQmlEngine::JavaScriptOwnership);
                mMapController->worldToScreen(wordPos,x, y);
                mUIHandle->cmShowContextMenu(mCurrentContextMenuItem, static_cast<int>(x), static_cast<int>(y));
                connect(model, &AircraftContextMenumodel::itemClicked, this, &Aircraft::onContextmenuItemClicked);
            }

        });

        comp->loadUrl(QUrl("qrc:/modelplugin/AircraftContextmenuView.qml"));
    }
    if(!onModel && mCurrentContextMenuItem){
        mUIHandle->cmHideContextMenu(mCurrentContextMenuItem);
        mCurrentContextMenuItem = nullptr;
    }

}
void Aircraft::onContextmenuItemClicked(int index,  QString systemName){
    std::cout << index << ", " << systemName.toStdString() << std::endl;
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
