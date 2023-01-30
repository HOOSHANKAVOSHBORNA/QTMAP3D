#include "aircraftModelNode.h"
#include "contextMenu.h"
#include "aircraftInformation.h"
#include "draw.h"

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
#include <osg/Material>
#include<osg/Switch>

#include <QDebug>
#include <QMouseEvent>
#include <QPainter>
#include <QQmlComponent>
#include <QQmlEngine>
#include <QQuickItem>
#include <QTimer>



const float RANGE3D = std::numeric_limits<float>::max();

osg::ref_ptr<osg::Node> AircraftModelNode::mNode3DRef;

AircraftModelNode::AircraftModelNode(MapController *mapControler, QQmlEngine *qmlEngine, UIHandle *uiHandle, QObject *parent)
    :DefenseModelNode(mapControler, parent)
{
    mRouteLine = new Line(mapControler);
    mRouteLine->setLineClamp(false);
    mRouteLine->setLineColor(osgEarth::Color::Purple);
    mRouteLine->setLineWidth(6);

    mTempRouteLine = new Line(mapControler);
    mTempRouteLine->setLineClamp(false);
    mTempRouteLine->setLineColor(osgEarth::Color::Purple);
    mTempRouteLine->setLineWidth(6);

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

    mNode3D = mNode3DRef.get();

    //create switch node for root--------------------------------------------------------------------
    mRootNode = new osg::LOD;
    mNode2D = new osg::Switch;

    osgEarth::Symbology::Style  rootStyle;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->setModel(mRootNode);
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->autoScale() = true;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->minAutoScale() = 1;
    rootStyle.getOrCreate<osgEarth::Symbology::ModelSymbol>()->maxAutoScale() = 2000;

    //    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->clamping() = osgEarth::Symbology::AltitudeSymbol::CLAMP_TO_TERRAIN;
    //    rootStyle.getOrCreate<osgEarth::Symbology::AltitudeSymbol>()->technique() = osgEarth::Symbology::AltitudeSymbol::TECHNIQUE_MAP;

    setStyle(rootStyle);
    //--create icon Nodes---------------------------------------------------------------------------
    m2DIcon = osgDB::readImageFile("../data/models/aircraft/aircraft.png");
    if(m2DIcon)
        m2DIcon->scaleImage(25, 32, m2DIcon->r());
    mSelect2DIcon = new osg::Image;
    mSelect2DIcon->copySubImage(0, 0, 0, m2DIcon);
//    create2DImageColore(osgEarth::Color::Red);

//    m2DIcon = osgDB::readImageFile("../data/models/aircraft/aircraft.png");
    osg::Geometry* redImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(m2DIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  redGeode = new osg::Geode();
    //    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    //    geode->getOrCreateStateSet()->setAttribute(new osg::LineWidth(1.0), osg::StateAttribute::ON);
    redGeode->addDrawable(redImageDrawable);

//    mSelect2DIcon->copySubImage(0, 0, 0, m2DIcon);
    //mSelect2DIcon->scaleImage(25, 32, mSelect2DIcon->r());
    osg::Geometry* yellowImageDrawable = osgEarth::Annotation::AnnotationUtils::createImageGeometry(mSelect2DIcon, osg::Vec2s(0,0), 0, 0, 1);
    osg::ref_ptr<osg::Geode>  yellowGeode = new osg::Geode();
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
}

void AircraftModelNode::flyTo(const osg::Vec3d &pos, double heading, double speed)
{
    //speed = 1;
    if(mIsStop)
        return;
    //    heading = 30;
    osgEarth::GeoPoint posGeo(getMapNode()->getMapSRS(), pos);

    osg::Vec3d currentPosW;
    getPosition().toWorld(currentPosW);

    osg::Vec3d posW;
    posGeo.toWorld(posW);
    //---------------------------------------
    //    if(mLocationPoints->empty())
    //        mLocationPoints->push_back(currentPosW);
    //    mLocationPoints->push_back(posW);
    if(mRouteLine->getSize() > 0)
        mRouteLine->addPoint(getPosition().vec3d());
    mRouteLine->addPoint(posGeo.vec3d());
    mTempRouteLine->clearPoints();
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
    path->insert(1,osg::AnimationPath::ControlPoint(posW,diffRotate, getScale()));
    path->insert(2,osg::AnimationPath::ControlPoint(posEstimateW1, headingRotate, getScale()));
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

void AircraftModelNode::stop()
{
    mIsStop = true;
    if(mAnimationPathCallback != nullptr)
        mAnimationPathCallback->getAnimationPath()->clear();
    removeEffect();
}

void AircraftModelNode::setTruckModel(osgEarth::Annotation::ModelNode *truckModel)
{
    mTruckModel = truckModel;
}

osgEarth::Annotation::ModelNode *AircraftModelNode::getTruckModel() const
{
    return mTruckModel;
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
        mMapController->untrackNode();
        mMapController->removeNode(mRouteLine->getNode());
        mMapController->removeNode(mTempRouteLine->getNode());
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

void AircraftModelNode::curentPosition(osgEarth::GeoPoint pos)
{
    DefenseModelNode::curentPosition(pos);

    //    if(mIsRoute)
    //    {
    //    osg::Vec3d currentPosW;
    //    pos.toWorld(currentPosW);
    //    mTempLocationPoints->push_back(currentPosW);
    //    }
    mTempRouteLine->addPoint(pos.vec3d());
}

void AircraftModelNode::onGotoButtonClicked()
{
    //    goOnTrack();
    mMapController->goToPosition(getPosition(), 200, 0);
    mMapController->setTrackNode(getGeoTransform());
}

void AircraftModelNode::onRouteButtonToggled(bool check)
{
    //    mIsRoute = true;
    //    qDebug()<<"iwRouteButtonClicked";
    if(check)
    {
        mMapController->addNode(mRouteLine->getNode());
        mMapController->addNode(mTempRouteLine->getNode());
    }
    else
    {
        mMapController->removeNode(mRouteLine->getNode());
        mMapController->removeNode(mTempRouteLine->getNode());
    }

    //    mMapController->getRoot()->addChild(drawLine(mLocationPoints, 1.0));
    //    std::cout << check << std::endl;

    //    QTimer *timer = new QTimer(this);
    //    connect(timer, &QTimer::timeout, this, [=](){
    //        mMapController->getRoot()->addChild(drawLine(mTempLocationPoints, 1.0));
    //    });
    //    timer->start(200);

}

void AircraftModelNode::onTrackButtonToggled(bool check)
{
    //    qDebug()<<"iwFollowButtonClicked";
    std::cout << check << std::endl;
    if(check)
        mMapController->setTrackNode(getGeoTransform());
    else
        mMapController->untrackNode();
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

void AircraftModelNode::change2DImageColore(osgEarth::Color color)
{
    //--create icon Nodes---------------------------------------------------------------------------
    if(!m2DIcon)
        return;
    //m2DIcon->scaleImage(25, 32, m2DIcon->r());
    for(int i = 0; i < m2DIcon->s(); i++)
        for(int j = 0; j < m2DIcon->t(); j++)
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

    for(int i = 0; i < mSelect2DIcon->s(); i++)
        for(int j = 0; j < mSelect2DIcon->t(); j++)
        {
            osg::Vec4 pixColore = mSelect2DIcon->getColor(i, j);
            //qDebug()<< redIcon->getColor(i, j).r()<<","<< redIcon->getColor(i, j).g()<<","<<redIcon->getColor(i, j).b();
            if(pixColore.r()>0 || pixColore.g()>0 || pixColore.b()>0)
                mSelect2DIcon->setColor(color, i, j);
        }
}

void AircraftModelNode::showInfoWidget()
{
    //    mUIHandle->iwSetReceiverObject(this);
    //    mUIHandle->iwShow(this, UIHandle::InfoWidgetType::Airplane);
    //    QString txtInfo = QString::fromUtf8(mInformation.toJson().toJson(QJsonDocument::Compact));
    //    mUIHandle->iwUpdateData(this, txtInfo);
    //    QQmlComponent *comp = new QQmlComponent(mQmlEngine);
    //    QObject::connect(comp, &QQmlComponent::statusChanged, [this, comp](){
    //        qDebug() << comp->errorString();

    //        if (comp->status() == QQmlComponent::Ready) {
    //            QQuickItem *item = static_cast<QQuickItem*>(comp->create(nullptr));
    //            InfoModel *model = new InfoModel;

    //            model->setAircraftInfo(mInformation);
    //            item->setProperty("model", QVariant::fromValue<InfoModel*>(model));
    //            QQmlEngine::setObjectOwnership(item, QQmlEngine::JavaScriptOwnership);

    //            connect(model, &InfoModel::gotoButtonClicked, this, &AircraftModelNode::onGotoButtonClicked);
    //            connect(model, &InfoModel::routeButtonClicked, this, &AircraftModelNode::onRouteButtonToggled);
    //            connect(model, &InfoModel::trackButtonClicked, this, &AircraftModelNode::onTrackButtonToggled);
    //            mUIHandle->iwShow(item);
    //        }

    //    });

    //    comp->loadUrl(QUrl("qrc:/modelplugin/InfoView.qml"));
    mAircraftinformation = new AircraftInformation(mQmlEngine, mUIHandle, mInformation, this);
    connect(mAircraftinformation->getInfo(), &AircraftInfoModel::gotoButtonClicked, this, &AircraftModelNode::onGotoButtonClicked);
    connect(mAircraftinformation->getInfo(), &AircraftInfoModel::routeButtonClicked, this, &AircraftModelNode::onRouteButtonToggled);
    connect(mAircraftinformation->getInfo(), &AircraftInfoModel::trackButtonClicked, this, &AircraftModelNode::onTrackButtonToggled);
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
    getMapNode()->addChild(mFire->getParticleSystem());
    //add smoke----------------------------------------------------------------------------------------------------
    osgEarth::Registry::shaderGenerator().run(mSmoke);// for textures or lighting
    getPositionAttitudeTransform()->addChild(mSmoke);
    mSmoke->setEmitterDuration(emitterDuration);
    mSmoke->setParticleDuration(5);
    osgEarth::Registry::shaderGenerator().run(mSmoke->getParticleSystem());// for textures or lighting
    getMapNode()->addChild(mSmoke->getParticleSystem());
}

void AircraftModelNode::removeEffect()
{
    //remove fire---------------------------------------------
    getMapNode()->removeChild(mFire->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mFire);
    //remove smoke--------------------------------------------
    getMapNode()->removeChild(mSmoke->getParticleSystem());
    getPositionAttitudeTransform()->removeChild(mSmoke);
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
