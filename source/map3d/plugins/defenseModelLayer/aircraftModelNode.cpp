#include "aircraftModelNode.h"
#include "contextMenu.h"
#include "aircraftInformation.h"
#include "draw.h"
#include "systemModelNode.h"

#include <osgEarth/Registry>
#include <osgGA/EventVisitor>
#include <osgGA/EventHandler>
#include <osgViewer/View>
#include <osgEarth/IntersectionPicker>
#include <osgEarthAnnotation/PlaceNode>
#include <osgEarthAnnotation/LabelNode>
#include <osgEarthAnnotation/GeoPositionNodeAutoScaler>
#include <osgEarthAnnotation/AnnotationUtils>
#include <osgEarthAnnotation/ImageOverlay>
#include <osg/Depth>
#include <osg/Material>
#include<osg/Switch>

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QTimer>
#include "defenseModelNodeAutoScaler.h"


#include "defenseModelLayer.h"

const float RANGE3D = std::numeric_limits<float>::max();
const int NUM_LATEST_POINT = 100;

osg::ref_ptr<osg::Node> AircraftModelNode::mNode3DRef;

AircraftModelNode::AircraftModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent)
    :DefenseModelNode(mapControler, parent)
{

    mQmlEngine = qmlEngine;
    mMapController = mapControler;
    mIs3D = mMapController->getMode();

    mUIHandle = uiHandle;
    if (!mNode3DRef.valid()) {
        mNode3DRef = osgDB::readRefNodeFile("../data/models/aircraft/boeing-747.osgb");
    }
    if (!mNode3DRef)
    {
        return;
    }

    mNode3D = new osg::Group;
    mNode3D->addChild(mNode3DRef.get());

    //create switch node for root--------------------------------------------------------------------
    mRootNode = new osg::LOD;
    mNode2D = new osg::Switch;

    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
//    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
//    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 1;
//    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale() = 1700;
    this->setCullingActive(false);
    this->addCullCallback(new DefenseModelNodeAutoScaler(2.5, 1, 500));

    //    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    //    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_DRAPE;

    setStyle(rootStyle);
    //--create icon Nodes---------------------------------------------------------------------------
    osg::ref_ptr<osg::StateSet> geodeStateSet = new osg::StateSet();
    geodeStateSet->setAttributeAndModes(new osg::Depth(osg::Depth::ALWAYS, 0, 1, false), 1);

    m2DIcon = osgDB::readImageFile("../data/models/aircraft/images/aircraft.png");
    if(m2DIcon)
        m2DIcon->scaleImage(100, 100, m2DIcon->r());
    mSelect2DIcon = new osg::Image;
    mSelect2DIcon->copySubImage(0, 0, 0, m2DIcon);
//    create2DImageColore(osgEarth::Color::Red);

//    m2DIcon = osgDB::readImageFile("../data/models/aircraft/aircraft.png");
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(m2DIcon, osg::Vec2s(0,0), 0, 0, 0.4);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
    //    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    //    geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(1.0), osg::StateAttribute::ON);
    redGeode->setStateSet(geodeStateSet);
    redGeode->addDrawable(redImageDrawable);

//    mSelect2DIcon->copySubImage(0, 0, 0, m2DIcon);
    //mSelect2DIcon->scaleImage(25, 32, mSelect2DIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(mSelect2DIcon, osg::Vec2s(0,0), 0, 0, 0.4);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
    yellowGeode->setStateSet(geodeStateSet);
    yellowGeode->addDrawable(yellowImageDrawable);

    mNode2D->addChild(yellowGeode, false);
    mNode2D->addChild(redGeode, true);

    //    setModelColor(mInformation.Identification);
    //--create lable-----------------------------------------------------------------------------
    osgEarth::Symbology::Style labelStyle;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->alignment() = osgEarth::Symbology::TextSymbol::ALIGN_CENTER_CENTER;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->fill()->color() = osgEarth::Symbology::Color::White;
    labelStyle.getOrCreate<osgEarth::Symbology::TextSymbol>()->size() = 14;
    //osg::Image* lableImage = osgDB::readImageFile("../data/models/text-background.png");
    updateOrCreateLabelImage();
    mLableNode = new osgEarth::Annotation::PlaceNode("",labelStyle, mLabelImage);
    //    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3(0, 0, 2));
    //    mLableNode->getGeoTransform()->setPosition(osg::Vec3(0, 0, 2));
    getGeoTransform()->addChild(mLableNode);
    mLableNode->setNodeMask(false);
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
    connect(mMapController, &MapController::modeChanged, this, &AircraftModelNode::onModeChanged);
    //----------------------------
    mRouteLine = new LineNode(mapControler);
    mRouteLine->setPointVisible(false);
    mRouteLine->setTessellation(10);
    mRouteLine->setColor(osgEarth::Color::Purple);
    mRouteLine->setWidth(5);

    mLatestPointLine = new LineNode(mapControler);
    mLatestPointLine->setPointVisible(true);
    mLatestPointLine->setPointColor(osgEarth::Color::Blue);
    mLatestPointLine->setColor(osgEarth::Color::Purple);
    mLatestPointLine->setWidth(5);
    mLatestPointLine->setPointWidth(15);

    mTempLine = new LineNode(mapControler);
    mTempLine->setPointVisible(false);
    mTempLine->setColor(osgEarth::Color::Purple);
    mTempLine->setWidth(5);
}

void AircraftModelNode::flyTo(osgEarth::GeoPoint posGeo, double heading, double /*speed*/)
{
    //speed = 1;
//    if(mIsStop)
//        return;
    //    heading = 30;
//    osgEarth::GeoPoint posGeo(getMapNode()->getMapSRS(), posGeo);
    posGeo.transformInPlace(mMapController->getMapSRS());
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
    diffRotate.makeRotate(osg::Vec3d(0, 1, 0), diffLocal);
    //heading----------------------------------------------------------------------------------------------
    osg::Vec3d northVec(0, 1, 0);//in local
    osg::Vec3d headingVecLocal = northVec * osg::Matrixd::rotate(osg::inDegrees(-heading), osg::Z_AXIS);
    headingVecLocal.normalize();

    headingVecLocal.z() = diffLocal.z();

    osg::Matrixd posLocalToWorld;
    posGeo.createLocalToWorld(posLocalToWorld);
    //osg::Vec3d headingVecW = headingVecLocal * osg::Matrixd::rotate(posLocalToWorld.getRotate());

    osg::Quat headingRotate;
    headingRotate.makeRotate(osg::Vec3d(0, 1, 0), headingVecLocal);
    //-------------------------------------------------------------------------------------------------------
//    osg::Vec3d posEstimateW1 = posW + (headingVecW * 100.0);
//    osg::Vec3d posEstimateW = posW + (headingVecW * 100000.0);
//    //    qDebug()<<"estimatePos"<<estimatePos.z();
//    double timeEstimate = (posEstimateW - posW).length() / speed;

    osg::AnimationPath* path = new osg::AnimationPath();
    path->setLoopMode(osg::AnimationPath::NO_LOOPING);

    path->insert(0, osg::AnimationPath::ControlPoint(currentPosW,getPositionAttitudeTransform()->getAttitude(),getScale()));
    path->insert(0.001,osg::AnimationPath::ControlPoint(posW,headingRotate, getScale()));
    //path->insert(0.1,osg::AnimationPath::ControlPoint(posW,diffRotate, getScale()));
    //path->insert(0.2,osg::AnimationPath::ControlPoint(posEstimateW1, headingRotate, getScale()));
    //path->insert(timeEstimate,osg::AnimationPath::ControlPoint(posEstimateW, headingRotate, getScale()));

    mAnimationPathCallback = new ModelAnimationPathCallback();
    mAnimationPathCallback->setAnimationPath(path);
    setUpdateCallback(mAnimationPathCallback);

    //--lines-------------------------------------
    if(mRouteLine->getSize() <= 0)
    {
        mRouteLine->addPoint(getPosition());
        mLatestPointLine->addPoint(getPosition());
    }
    mLatestPointLine->addPoint(mCurrentFlyPoint);
    if(mLatestPointLine->getSize() >= NUM_LATEST_POINT)
    {
        mLatestPointLine->removeFirstPoint();
    }
    if(std::abs(mCurrentHeading - heading) < 5)
        mRouteLine->removePoint();
    mRouteLine->addPoint(mCurrentFlyPoint);

    mCurrentHeading = heading;
    mCurrentFlyPoint = posGeo;
    mTempLine->clear();
}

void AircraftModelNode::stop()
{
    mIsStop = true;
    if(mAnimationPathCallback != nullptr)
        mAnimationPathCallback->getAnimationPath()->clear();
    removeEffect();
}

void AircraftModelNode::setInformation(AircraftInfo info)
{
    //    if(mInformation.Identification != info.Identification)
    mInformation = info;
    //    QString txtInfo = QString::fromUtf8(mInformation.toJson().toJson(QJsonDocument::Compact));

    if(mAircraftinformation)
        mAircraftinformation->updateAircraft(info);
    //    mUIHandle->iwUpdateData(this, txtInfo);
    //------------------------------------------------------
    updateOrCreateLabelImage();
    changeModelColor(mInformation.Identification);
}

AircraftInfo AircraftModelNode::getInformation()
{
    return mInformation;
}

void AircraftModelNode::goOnTrack()
{
    //mMapController->goToPosition(getPosition(), 200, 0);
    mMapController->setTrackNode(getGeoTransform());
}

void AircraftModelNode::onLeftButtonClicked(bool val)
{
    select(val);
    if(val)
    {
        showInfoWidget();
    }
    else
    {
        mMapController->untrackNode(getGeoTransform());
        removeNodeFromLayer(mRouteLine);
        removeNodeFromLayer(mLatestPointLine);
        removeNodeFromLayer(mTempLine);
    }
    if(mCurrentContextMenu){
        mCurrentContextMenu->hideMenu();
        mCurrentContextMenu = nullptr;
    }
}

void AircraftModelNode::frameEvent()
{
    if (mCurrentContextMenu) {
        osg::Vec3d wordPos;
        getPosition().toWorld(wordPos);
        float x, y;
        mMapController->worldToScreen(wordPos,x, y);
        mCurrentContextMenu->updatePosition(static_cast<int>(x), static_cast<int>(y));
    }
    //------------------------------------
    //    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( getPositionAttitudeTransform()->getBound().radius()/2, getPositionAttitudeTransform()->getBound().radius(), 2));
    mLableNode->getPositionAttitudeTransform()->setPosition(osg::Vec3( 0, 0, 0));
    //------------------------------------
    if(mTempLine->getSize() > 1)
        mTempLine->removePoint();
    mTempLine->addPoint(getPosition());
}

void AircraftModelNode::mousePressEvent(QMouseEvent *event, bool onModel)
{
    DefenseModelNode::mousePressEvent(event, onModel);
    if(event->button() == Qt::LeftButton)
    {
        onLeftButtonClicked(onModel);
        if(onModel)
            event->accept();
    }
    if(event->button() == Qt::RightButton) {
        mCurrentContextMenu = new ContextMenu(mQmlEngine, mUIHandle, this);
        for(auto detectSystem: mInformation.DetectionSystems)
            mCurrentContextMenu->addRow(detectSystem);

        connect(mCurrentContextMenu->getModel(), &ContextMenumodel::itemClicked, this, &AircraftModelNode::onContextmenuItemClicked);
        osg::Vec3d wordPos;
        getPosition().toWorld(wordPos);
        float x, y;
        mMapController->worldToScreen(wordPos,x, y);
        mCurrentContextMenu->show(static_cast<int>(x), static_cast<int>(y));
        event->accept();
    }
    if(!onModel && mCurrentContextMenu){
        mCurrentContextMenu->hideMenu();
        mCurrentContextMenu = nullptr;
    }

}

SystemModelNode *AircraftModelNode::getAssignment(int number) const
{
    if(mAssignmentMap.contains(number))
        return mAssignmentMap[number];
    return nullptr;
}

void AircraftModelNode::addAssignment(int number, SystemModelNode *assignmentModelNode)
{
    mAssignmentMap[number] = assignmentModelNode;
}

void AircraftModelNode::removeAssignment(int number)
{
    mAssignmentMap.remove(number);
}

void AircraftModelNode::acceptAssignment(int number, bool value)
{
    if(!value)
        removeAssignment(number);
}

void AircraftModelNode::clearAssignments(int exceptNumber)
{
    for(auto number: mAssignmentMap.keys())
    {
        if(exceptNumber != number)
        {
            removeAssignment(number);
        }
    }
}

bool AircraftModelNode::hasAssignment()
{
    return mAssignmentMap.count() > 0;
}

QMap<int, SystemModelNode *> AircraftModelNode::getAssignments() const
{
    return mAssignmentMap;
}

void AircraftModelNode::onGotoButtonClicked()
{
    //    goOnTrack();
    mMapController->goToPosition(getPosition(), 400, 0);
    mMapController->setTrackNode(getGeoTransform());
}

void AircraftModelNode::onRouteButtonToggled(bool check)
{
    if(check)
    {
        addNodeToLayer(mRouteLine);
        addNodeToLayer(mTempLine);
    }
    else
    {
        removeNodeFromLayer(mRouteLine);
        removeNodeFromLayer(mTempLine);
    }

}

void AircraftModelNode::onLatestPointsToggled(bool check) {
    if (check)
    {
//        mLatestPointLine->setPointVisibilty(true);
        addNodeToLayer(mLatestPointLine);
        addNodeToLayer(mTempLine);
    }
    else
    {
//        mLatestPointLine->setPointVisibilty(false);
        removeNodeFromLayer(mLatestPointLine);
        removeNodeFromLayer(mTempLine);
    }
}

void AircraftModelNode::onTrackButtonToggled(bool check)
{
    std::cout << check << std::endl;
    if(check)
        mMapController->setTrackNode(getGeoTransform());
    else
        mMapController->untrackNode(getGeoTransform());
}

void AircraftModelNode::onModeChanged(bool is3DView)
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

    //    select(mIsSelected);
}

void AircraftModelNode::onContextmenuItemClicked(int index,  QString systemName)
{
    std::cout << index << ", " << systemName.toStdString() << std::endl;
}

void AircraftModelNode::changeModelColor(AircraftInfo::Identify identify)
{
    osgEarth::Color color;
    switch (identify) {
    case AircraftInfo::F:
        color = osgEarth::Color::Green;
        break;
    case AircraftInfo::K:
        color = osgEarth::Color::Yellow;
        break;
    case AircraftInfo::Z:
        color =  osg::Vec4(1.0, 0.5, 0.0, 1.0);
        break;
    case AircraftInfo::X:
        color = osgEarth::Color::Red;
        break;
    case AircraftInfo::U:
        color = osgEarth::Color::White;
        break;
    case AircraftInfo::H:
        color = osgEarth::Color::Red;
        break;
    }
    mModelColor = color;
    //    if(!mIs3D)
    change2DImageColore(mModelColor);
    select(mIsSelected);//for change color
}

void AircraftModelNode::showInfoWidget()
{
    mAircraftinformation = new AircraftInformation(mQmlEngine, mUIHandle, mInformation, this);
    connect(mAircraftinformation->getInfo(), &AircraftInfoModel::gotoButtonClicked, this, &AircraftModelNode::onGotoButtonClicked);
    connect(mAircraftinformation->getInfo(), &AircraftInfoModel::routeButtonClicked, this, &AircraftModelNode::onRouteButtonToggled);
    connect(mAircraftinformation->getInfo(), &AircraftInfoModel::trackButtonClicked, this, &AircraftModelNode::onTrackButtonToggled);
    connect(mAircraftinformation->getInfo(), &AircraftInfoModel::latestPointsClicked, this, &AircraftModelNode::onLatestPointsToggled);
    mAircraftinformation->show();
}

void AircraftModelNode::addEffect(double emitterDuration)
{
    //add fire-----------------------------------------------------------------------------------------------------
    osgEarth::Registry::shaderGenerator().run(mFire);// for textures or lighting
    getPositionAttitudeTransform()->addChild(mFire);
    mFire->setEmitterDuration(emitterDuration);
    mFire->setParticleDuration(0.2);
    osgEarth::Registry::shaderGenerator().run(mFire->getParticleSystem());// for textures or lighting
    addNodeToLayer(mFire->getParticleSystem());
    //add smoke----------------------------------------------------------------------------------------------------
    osgEarth::Registry::shaderGenerator().run(mSmoke);// for textures or lighting
    getPositionAttitudeTransform()->addChild(mSmoke);
    mSmoke->setEmitterDuration(emitterDuration);
    mSmoke->setParticleDuration(5);
    osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());// for textures or lighting
    addNodeToLayer(mSmoke->getParticleSystem());
}

void AircraftModelNode::removeEffect()
{
    //remove fire---------------------------------------------
    removeNodeFromLayer(mFire->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mFire);
    //remove smoke--------------------------------------------
    removeNodeFromLayer(mSmoke->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mSmoke);
}

bool AircraftModelNode::addNodeToLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(AIRCRAFTS_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->addChild(node);
        }
    }
}

bool AircraftModelNode::removeNodeFromLayer(osg::Node *node)
{
    auto layer = mMapController->getMapNode()->getMap()->getLayerByName(AIRCRAFTS_LAYER_NAME);
    if (layer) {
        osg::Group *group = dynamic_cast<osg::Group*>(layer->getNode());
        if (group) {
            group->removeChild(node);
        }
    }
}

void AircraftModelNode::change2DImageColore(osgEarth::Color color)
{
    //--create icon Nodes---------------------------------------------------------------------------
    if(!m2DIcon)
        return;
    //m2DIcon->scaleImage(25, 32, m2DIcon->r());
    unsigned width = static_cast<unsigned>(m2DIcon->s());
    unsigned height = static_cast<unsigned>(m2DIcon->t());
    for(unsigned i = 0; i < width; i++)
        for(unsigned j = 0; j < height; j++)
        {
            osg::Vec4 pixColore = m2DIcon->getColor(i, j);
            //qDebug()<< redIcon->getColor(i, j).r()<<","<< redIcon->getColor(i, j).g()<<","<<redIcon->getColor(i, j).b();
            if(pixColore.a()>0)
                m2DIcon->setColor(color, i, j);
        }
    //-----------------------------------------------------------------------------------------------
    //selectIcon->scaleImage(25, 32, selectIcon->r());
    color /= 2;
    color.a() = 1;

    width = static_cast<unsigned>(m2DIcon->s());
    height = static_cast<unsigned>(m2DIcon->t());

    for(unsigned i = 0; i < width; i++)
        for(unsigned j = 0; j < height; j++)
        {
            osg::Vec4 pixColore = mSelect2DIcon->getColor(i, j);
            //qDebug()<< redIcon->getColor(i, j).r()<<","<< redIcon->getColor(i, j).g()<<","<<redIcon->getColor(i, j).b();
            if(pixColore.r()>0 || pixColore.g()>0 || pixColore.b()>0)
                mSelect2DIcon->setColor(color, i, j);
        }
}


void AircraftModelNode::updateOrCreateLabelImage()
{
    if (!mRenderTargetImage) {
        mRenderTargetImage = new QImage(
                    LABEL_IMAGE_WIDTH,
                    LABEL_IMAGE_HEIGHT,
                    QImage::Format_RGBA8888
                    );
    }

    if (!mLabelImage) {
        mLabelImage = new osg::Image;
    }

    {
        mRenderTargetImage->fill(QColor(Qt::transparent));
        QPainter painter(mRenderTargetImage);
        painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);


        static const QBrush backgroundBrush = QBrush(QColor(30, 30, 30, int(255 * 0.3f)));

        static const QFont textFont("SourceSansPro", 12, QFont::Normal);
        static const QPen  textPen(QColor(255, 255, 255));

        painter.setPen(Qt::NoPen);
        painter.setBrush(backgroundBrush);

        painter.drawRoundedRect(
                    mRenderTargetImage->rect(),
                    8,8);

        painter.setPen(textPen);
        painter.setFont(textFont);
        painter.drawText(QRect(0, 0, LABEL_IMAGE_WIDTH, 30),
                         Qt::AlignCenter,
                         QString::number(mInformation.TN));


        static const QPen linePen(QColor(255, 255, 255),
                                  1,
                                  Qt::PenStyle::DashLine
                                  );

        painter.setPen(linePen);
        painter.setBrush(Qt::NoBrush);
        painter.drawLine(0, 35, LABEL_IMAGE_WIDTH, 35);


        painter.setPen(textPen);
        painter.setFont(textFont);
        painter.drawText(QRect(10, 40, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "IFFCode:");
        painter.drawText(QRect(10, 40, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         mInformation.IFFCode);


        painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "CallSign:");
        painter.drawText(QRect(10, 70, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         mInformation.CallSign);


        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "M-Radar:");
        painter.drawText(QRect(10, 100, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         mInformation.MasterRadar);

        painter.drawText(QRect(10, 130, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignLeft | Qt::AlignVCenter,
                         "I-Method:");
        painter.drawText(QRect(10, 130, LABEL_IMAGE_WIDTH-20, 30),
                         Qt::AlignRight | Qt::AlignVCenter,
                         mInformation.IdentificationMethod);


    }
    *mRenderTargetImage = mRenderTargetImage->mirrored(false, true);

    mLabelImage->setImage(LABEL_IMAGE_WIDTH,
                          LABEL_IMAGE_HEIGHT,
                          1,
                          GL_RGBA,
                          GL_RGBA,
                          GL_UNSIGNED_BYTE,
                          mRenderTargetImage->bits(),
                          osg::Image::AllocationMode::NO_DELETE);
}


